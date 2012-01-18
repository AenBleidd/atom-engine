#include "../gamefs.h"
#include "../strings.h"
#ifdef _FSMAN_
// Remove folder links
inline static int32_t dot_exclude(const struct dirent64 *dir) {
  if ((dir->d_name[0] == '.') && (dir->d_name[1] == 0)) return 0;
  if ((dir->d_name[0] == '.') && (dir->d_name[1] == '.') &&
      (dir->d_name[2] == 0)) return 0;
  return 1;
}
#endif  // _FSMAN_
#ifdef _FSMAN_
int32_t AtomFS::FolderScan(char *ch, RECORD *list, FILE *bin, int32_t level = 0) {
  if (ch != NULL) {
    if (list == 0) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_ADD_FILE);
      return -1;
    }
// find last record
    while (list->next != 0)
      list = list->next;
// Write folder info
    RECORD *record = new RECORD;
    list->next = record;
    record->flag = flag_folder;
    uint32_t name_size = 0;
    for (uint32_t i = strlen(ch) - 1; i != 0; i--) {
      if (ch[i] != '\\' && ch[i] != '/')
        name_size++;
      else
        break;
    }
    uint32_t pos = strlen(ch) - name_size;
    record->namelen = name_size + 1;
    record->name = new char[record->namelen];
    strncpy(record->name, ch + pos, record->namelen);
    record->name[record->namelen] = '\0';
    record->size = 0;
    record->offset = 0;
    record->crc = 0;
    record->next = 0;
// set new datasize
    datsize += (sizeof(record->flag) + sizeof(record->namelen) + record->namelen);
    snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE, "Write folder %s\n", record->name);
    atomlog->DebugMessage(atomlog->MsgBuf);
    char *curdir = 0;
// TODO(Lawliet): Check this string if first call chdir(ch) is NULL
    if (chdir(ch) != 0) return -1;
    struct dirent64 **eps;
    struct stat64 st;
    int32_t n;
    n = scandir64("./", &eps, dot_exclude, alphasort64);
    if (n >= 0) {
      int32_t cnt;
      for (cnt = 0; cnt < n; ++cnt) {
        if (eps[cnt]->d_type == DT_DIR) {  // it is a folder
          curdir = eps[cnt]->d_name;
          if (FolderScan(curdir, record, bin, level+1) == -1) {
            return -1;
          }
        }
        if (eps[cnt]->d_type == DT_REG) {  // it is a file
          stat64(eps[cnt]->d_name, &st);
          curdir = eps[cnt]->d_name;  // but it is a file not folder
          int64_t fsize = (int64_t)st.st_size;
          snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE,
                   "Writing file %s (%ld bytes)\n", curdir, fsize);
          atomlog->DebugMessage(atomlog->MsgBuf);
// TODO(Lawliet): Check this string for correct file name written to the filetable
          if (Write(curdir, record, bin, curdir) == -1) {
            return -1;
          }
        }
      }
// End of folder
      if (record == 0) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_ADD_FILE);
        return -1;
      }
      while (record->next != 0)
        record = record->next;
      RECORD *eof = new RECORD;
      record->next = eof;
      eof->flag = flag_eoc;
      eof->namelen = 0;
      eof->name = 0;
      eof->size = 0;
      eof->offset = 0;
      eof->crc = 0;
      eof->next = 0;
// Update datsize
          datsize += sizeof(eof->flag);
          snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s %s\n",
            "Leaving folder", ch);
          atomlog->DebugMessage(atomlog->MsgBuf);
    for (cnt = 0; cnt < n; ++cnt) free(eps[cnt]);
    free(eps);
    } else {
      snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE,
               "Couldn't open the directory %s\n",  eps[n]->d_name);
      atomlog->LogMessage(atomlog->MsgBuf);
      return -1;
    }
  chdir("..");
  }
  return 0;
}
#endif  // _FSMAN_                                                                                                  
#ifdef _FSMAN_
int32_t AtomFS::Create(char **input, uint32_t count, char *file,
                   uint16_t encrypt, uint32_t *key, uint8_t type) {
  FILE *binfile, *bintempfile;
  datsize = 0;
  binsize = 0;
// Generate crypt key
// If crypt key is predefined new key is ignored
  if (wake_key == 0) {
    wake_key = key;
    wake_table = GenKey(wake_key[0], wake_key[1], wake_key[2], wake_key[3]);
  }
// set encrypt bytes count
  if (encrypt > 4 && encrypt != 0xFFFF && encrypt % 4 != 0)
    bytescrypt = encrypt - (encrypt % 4) + 4;
  else
    bytescrypt = encrypt;
// Create name of the output files
  char *bin = file;
// check if output files exist
  binfile = fopen(bin, "r");
  if (binfile != NULL) {
// file exist
    fclose(binfile);
// rename the files
    int32_t i = 0, s = 0;
    bool flag = false;
    char *bintemp;
    while (flag != true) {
      int32_t j = i;
      int32_t k = 0;
      while (j > 9)  j /= 10, k++;
      s = strlen(bin) + k + 5;
      bintemp = new char[s];
// create filename
      snprintf(bintemp, s, "%s.%03d", bin, i);
      bintempfile = fopen(bintemp, "r");
      if (bintempfile == NULL) {
        if (rename(bin, bintemp) == 0)
          flag = true;
        else i++;
      } else {
        fclose(bintempfile);
        i++;
      }
      delete [] bintemp;
    }
  }
// open file for writing
  binfile = fopen(bin, "wb");

// Create file (NIGHTMARE!!!)
// set static header information
  HEADER header;
  header.magic = magic;
  AGUID *guid = GetAGUID();
  header.guid = *guid;
  delete guid;
  guid = 0;
  header.version = version;
  header.type = type;
  header.encoding = flag_ascii;
  header.crypt = bytescrypt;
// set variable header information
  header.bincount = 1;
  datsize = 0;
  header.filetable = 0;
// write empty header to file
  if (fwrite("0", sizeof(header), 1, binfile) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(binfile);
    return -1;
  }
// write root directory
  RECORD *root = new RECORD;
  root->next = 0;
  root->flag = flag_folder;
  root->namelen = 1;
  root->name = new char[root->namelen + 1];
  snprintf(root->name, root->namelen + 1, "%s", "/");
  root->size = 0;
  root->offset = 0;
  root->crc = 0;
// set new datasize
  datsize += (sizeof(root->flag)+sizeof(root->namelen)+root->namelen);
// Scanning...
  for (int32_t i = 0; i < count; i++) {
    struct stat64 st;
    stat64(input[i], &st);
    if (S_ISDIR(st.st_mode) == true) {  // it is a folder
      if (FolderScan(input[i], root, binfile, 0) == -1) {
		      fclose(binfile);
        return -1;
      }
    }
    else if (S_ISREG(st.st_mode) == true) {  // it is a file
      int64_t size = (int64_t)st.st_size;
      snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE,
        "Writing file %s (%ld bytes)\n", input[i], size);
      atomlog->DebugMessage(atomlog->MsgBuf);
      if (Write(input[i], root, binfile, input[i]) != 0) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(binfile);
        return -1;
      }
    }
  }
// Write end of folder flag
  RECORD *record = root;
  while (record->next != 0)
    record = record->next;
  RECORD *eof = new RECORD;
  record->next = eof;
  eof->next = 0;
  eof->flag = flag_eoc;
  eof->namelen = 0;
  eof->name = 0;
  eof->size = 0;
  eof->offset = 0;
  eof->crc = 0;
// Update datsize
  datsize += sizeof(eof->flag);
// Would we store the key?
// this is an addon?
  if (type == type_addon) {
// yes it is...
// write the flag
    if (fwrite(&flag_key, sizeof(flag_key), 1, binfile) != 1) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
      fclose(binfile);
      return -1;
    }
// write the key
    if (fwrite(wake_key, 4, 4, binfile) != 4) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
      fclose(binfile);
      return -1;
    }
  }
// find the address of future filetable
  header.filetable = ftell(binfile);
  if (header.filetable == -1L) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(binfile);
    return -1;
  }
// let collect all data to one file
  for (;;) {
    if (root == 0) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_ADD_FILE);
      fclose(binfile);
      return -1;
    }
    if (root->flag == flag_file) {
      if (fwrite(&root->flag, sizeof(root->flag), 1, binfile) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(binfile);
        return -1;
      }
      if (fwrite(&root->namelen, sizeof(root->namelen), 1, binfile) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(binfile);
        return -1;
      }
      if (fwrite(root->name, 1, root->namelen, binfile) != root->namelen) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(binfile);
        return -1;
      }
      if (fwrite(&root->size, sizeof(root->size), 1, binfile) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(binfile);
        return -1;
      }
      if (fwrite(&root->offset, sizeof(root->offset), 1, binfile) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(binfile);
        return -1;
      }
      if (fwrite(&root->crc, sizeof(root->crc), 1, binfile) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(binfile);
        return -1;
      }
    } else if (root->flag == flag_folder) {
      if (fwrite(&root->flag, sizeof(root->flag), 1, binfile) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(binfile);
        return -1;
      }
      if (fwrite(&root->namelen, sizeof(root->namelen), 1, binfile) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(binfile);
        return -1;
      }
      if (fwrite(root->name, 1, root->namelen, binfile) != root->namelen) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(binfile);
        return -1;
      }
    } else if (root->flag == flag_eoc) {
      if (fwrite(&root->flag, sizeof(root->flag), 1, binfile) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(binfile);
        return -1;
      }
    }
    if (root->next == 0)
      break;
    else
      root = root->next;
  }

//  delete [] tempbuf;
// rewind the file to write the header
  rewind(binfile);
// writing the header
  if (fwrite(&header, sizeof(header), 1, binfile) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(binfile);
    return -1;
  }
// we've finished the work with bin file
  fclose(binfile);
//  delete [] bin;
  bin = 0;
// end working with pak file
  return 0;
}
#endif  // _FSMAN_
