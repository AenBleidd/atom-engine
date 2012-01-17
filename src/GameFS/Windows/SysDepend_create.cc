#include "../gamefs.h"
#include "../strings.h"
#ifdef _FSMAN_
// Remove folder links
int32_t AtomFS::dot_exclude(const WIN32_FIND_DATA *dir) {
  if ((strcmp(dir->cFileName, ".") == 0) ||
      (strcmp(dir->cFileName, "..") == 0))
    return 0;
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
    WIN32_FIND_DATA st;
    HANDLE hf;
// go to the subfolder
    const uint32_t size = strlen(ch) + 3;
    char *tmp = new char[size];
    snprintf(tmp, size, "%s\\*", ch);
    hf = FindFirstFile(tmp, &st);
    if (hf == INVALID_HANDLE_VALUE) {  // there is some error
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
      fclose(bin);
      FindClose(hf);
      delete [] tmp;
      tmp = 0;
      return -1;
    }
    do {
      if ((st.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) &&
          (dot_exclude(&st) == 1)) {
        const uint32_t fsize = sizeof(ch) + sizeof(st.cFileName) + 2;
        curdir = new char[fsize];
        snprintf(curdir, fsize, "%s\\%s", ch, st.cFileName);
        if (FolderScan(curdir, record, bin, level+1) == -1) {
          delete [] curdir;
          curdir = 0;
          return -1;
        }
        delete [] curdir;
        curdir = 0;
      }
      else if (dot_exclude(&st) == 1) {
        curdir = st.cFileName;
        int64_t fsize = (st.nFileSizeHigh * (MAXDWORD+1)) + st.nFileSizeLow;
        snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE,
                 "Writing file %s (%ld bytes)\n", curdir, fsize);
        atomlog->DebugMessage(atomlog->MsgBuf);
        const uint32_t st_size = strlen(ch) + strlen(st.cFileName) + 2;
        curdir = new char[st_size];
        snprintf(curdir, st_size, "%s\\%s", ch, st.cFileName);
        if (Write(curdir, record, bin, st.cFileName) == -1) {
          delete [] curdir;
          curdir = 0;
          return -1;
        }
        delete [] curdir;
        curdir = 0;
      }
    } while (FindNextFile(hf, &st)!= 0);
    FindClose(hf);
    delete [] tmp;
    tmp = 0;
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
    WIN32_FIND_DATA st;
    HANDLE hf;
    hf = FindFirstFile(input[i], &st);
    if (hf == INVALID_HANDLE_VALUE) {  // there is some error
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
      fclose(binfile);
      FindClose(hf);
      return -1;
    }
    FindClose(hf);
    if (st.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
      if (FolderScan(input[i], root, binfile, 0) == -1) {
		      fclose(binfile);
        return -1;
      }
    }
    else {
      int64_t size = (st.nFileSizeHigh * (MAXDWORD+1)) + st.nFileSizeLow;
      snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE,
        "Writing file %s (%ld bytes)\n", st.cFileName, size);
      atomlog->DebugMessage(atomlog->MsgBuf);
      if (Write(input[i], root, binfile, st.cFileName) != 0) {
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
