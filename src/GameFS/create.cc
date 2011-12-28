#include "gamefs.h"
#include "strings.h"
#ifdef _FSMAN_
uint64_t datsize = 0;  // size of data in file
uint64_t binsize = 0;  // size of bin in file
#endif  // _FSMAN_
#ifdef _FSMAN_
// Remove folder links
#ifdef UNIX
inline static int32_t dot_exclude(const struct dirent64 *dir) {
  if ((dir->d_name[0] == '.') && (dir->d_name[1] == 0)) return 0;
  if ((dir->d_name[0] == '.') && (dir->d_name[1] == '.') &&
      (dir->d_name[2] == 0)) return 0;
  return 1;
}
#endif  // UNIX
#ifdef WINDOWS
inline static int32_t dot_exclude(const WIN32_FIND_DATA *dir) {
  if ((strcmp(dir->cFileName, ".") == 0) ||
      (strcmp(dir->cFileName, "..") == 0))
    return 0;
  return 1;
}
#endif  // WINDOWS
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
    record->namelen = strlen(ch);
    record->name = ch;
    record->size = 0;
    record->offset = 0;
    record->crc = 0;
    record->next = 0;
// set new datasize
    datsize += (sizeof(record->flag) + sizeof(record->namelen) + record->namelen);
    snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE, "Write folder %s\n", ch);
    atomlog->DebugMessage(atomlog->MsgBuf);
    char *curdir = 0;
#ifdef UNIX
// TODO(Lawliet): Check this string if first call chdir(ch) is NULL
    if (chdir(ch) != 0) return -1;
    struct dirent64 **eps;
    struct stat64 st;
    int32_t n;
    n = scandir64("./", &eps, dot_exclude, alphasort64);
    if (n >= 0) {
      int32_t cnt;
      for (cnt = 0; cnt < n; ++cnt) {
#endif  // UNIX
#ifdef WINDOWS
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
#endif  // WINDOWS
#ifdef UNIX
        if (eps[cnt]->d_type == DT_DIR) {  // it is a folder
          curdir = eps[cnt]->d_name;
#endif  // UNIX
#ifdef WINDOWS
        if ((st.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) &&
            (dot_exclude(&st) == 1)) {
          const uint32_t fsize = sizeof(ch) + sizeof(st.cFileName) + 2;
          curdir = new char[fsize];
          snprintf(curdir, fsize, "%s\\%s", ch, st.cFileName);
#endif  // WINDOWS
          if (FolderScan(curdir, record, bin, level+1) == -1) {
#ifdef WINDOWS
            delete [] curdir;
            curdir = 0;
#endif // WINDOWS
            return -1;
          }
#ifdef WINDOWS
          delete [] curdir;
          curdir = 0;
#endif  // WINDOWS
        }
#ifdef UNIX
        if (eps[cnt]->d_type == DT_REG) {  // it is a file
          stat64(eps[cnt]->d_name, &st);
          curdir = eps[cnt]->d_name;  // but it is a file not folder
          int64_t fsize = (int64_t)st.st_size;
#endif  // UNIX
#ifdef WINDOWS
        else if (dot_exclude(&st) == 1) {
          curdir = st.cFileName;
          int64_t fsize = (st.nFileSizeHigh * (MAXDWORD+1)) + st.nFileSizeLow;
#endif  // WINDOWS
          snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE,
                   "Writing file %s (%ld bytes)\n", curdir, fsize);
          atomlog->DebugMessage(atomlog->MsgBuf);
#ifdef WINDOWS
          const uint32_t st_size = strlen(ch) + strlen(st.cFileName) + 2;
          curdir = new char[st_size];
          snprintf(curdir, st_size, "%s\\%s", ch, st.cFileName);
#endif  // WINDOWS
          if (Write(curdir, record, bin, st.cFileName) == -1) {
#ifdef WINDOWS
            delete [] curdir;
            curdir = 0;
#endif  // WINDOWS
            return -1;
          }
#ifdef WINDOWS
          delete [] curdir;
          curdir = 0;
#endif  // WINDOWS
        }
#ifdef UNIX
      }
#endif  // UNIX
#ifdef WINDOWS
      } while (FindNextFile(hf, &st)!= 0);
      FindClose(hf);
      delete [] tmp;
      tmp = 0;
#endif  // WINDOWS
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
#ifdef UNIX
    for (cnt = 0; cnt < n; ++cnt) free(eps[cnt]);
    free(eps);
    } else {
      snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE,
               "Couldn't open the directory %s\n",  eps[n]->d_name);
      atomlog->LogMessage(atomlog->MsgBuf);
      return -1;
    }
  chdir("..");
#endif  // UNIX
  }
  return 0;
}
#endif  // _FSMAN_
#ifdef _FSMAN_
int32_t AtomFS::Write(char *in, RECORD *list, FILE *bin, char *shortname) {
  if (list == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_ADD_FILE);
    return -1;
  }
// find last record
  while (list->next != 0)
    list = list->next;  
  FILE *file = fopen(in, "rb");
  if (file == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
    return -1;  // nothing was written
  }
  if (shortname == 0) {
// we need to get real name of the file without its path
    uint32_t pos = 0;
    for (int i = 0; i < strlen(in); i++)
      if (in[i] == '\\' || in[i] == '/')
	    pos = i+1;
    if (pos < strlen(in) + 1) {
      shortname = new char[strlen(in) - pos];
      strcpy(shortname, in+pos);
    }
  }
// Collect info about file
  RECORD *record = new RECORD;
  list->next = record;
  record->flag = flag_file;
  record->namelen = strlen(shortname);
  record->name = shortname;
  record->next = 0;
// check filesize
  if (fseek(file, 0, SEEK_END) != 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(file);
    return -1;
  }
  record->size = ftell(file);
  if (record->size == -1L) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(file);
    return -1;
  }
  rewind(file);
// encrypt
  uint64_t count;
  if ((bytescrypt == 0xFFFF) || (bytescrypt > record->size))
    count = record->size;
  else
    count = bytescrypt;
  count = (count & ~3) / 4;
  if (count % 4 != 0)
    count++;
  uint64_t alreadyRead = 0;
  uint64_t currentRead = 0;
  uint64_t crypted = 0;
  uint64_t crypting = 0;
  uint64_t realcrypting = 0;
  uint64_t crc = 0;
// Alloc buffer for reading
  uint8_t *buf = new uint8_t[MAX_READ_LEN];
  uint32_t *tempbuf = new uint32_t[MAX_READ_LEN / 4];
  uint32_t r[4];
// TODO(Lawliet): Optimize this
  r[0] = wake_key[0];
  r[1] = wake_key[1];
  r[2] = wake_key[2];
  r[3] = wake_key[3];
  uint64_t t = 0;
  uint64_t tempsize = record->size;
  while (alreadyRead < record->size) {
    if ((record->size - alreadyRead) >= MAX_READ_LEN)
      currentRead = MAX_READ_LEN;
    else
      currentRead = record->size - alreadyRead;
// Read the piece of file
    if (fread(buf, 1, currentRead, file) != currentRead) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
      fclose(file);
      delete [] buf;
      delete [] tempbuf;
      return -1;
    }
    alreadyRead += currentRead;
    t = (currentRead & ~3) / 4;
    if (currentRead % 4 != 0)
      t++;
// copy this piece of file
    memcpy(tempbuf, buf, currentRead);
    if ((count - crypted) >= MAX_READ_LEN)
      crypting = MAX_READ_LEN;
    else
      crypting = count - crypted;
    realcrypting = (crypting & ~3) / 4;
    if (crypting % 4 != 0)
      realcrypting++;
    Crypt(tempbuf, realcrypting, r, r, wake_table);
    crypted += crypting;
    t *= 4;
// copy crypted piece of file
    memcpy(buf, tempbuf, t);
// calculating crc
    crc = GenCRC32(buf, t, crc);
// write to disk crypted data
    if (fwrite(buf, 1, t, bin) != t) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
      fclose(file);
      delete [] buf;
      delete [] tempbuf;
      return -1;
    }
// Show progress
    snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "Written: %3.f %%\r",
             (double)alreadyRead / (double)record->size * 100);
    atomlog->LogMessage(atomlog->MsgBuf);
  }
  delete [] buf;
  buf = 0;
  delete [] tempbuf;
  tempbuf = 0;
// TODO(Lawliet): We really need this string?
  fflush(file);
  fclose(file);
  record->crc = crc;
  record->offset = binsize;
// Write data
  datsize += (sizeof(record->flag) + sizeof(record->namelen) + record->namelen +
             sizeof(record->size) + sizeof(record->offset) + sizeof(record->crc));

  binsize += t;
  snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE,
           "File %s was successfully written\n", in);
  atomlog->DebugMessage(atomlog->MsgBuf);
  return 0;
}
#endif  // _FSMAN_
#ifdef _FSMAN_
int32_t AtomFS::Create(char **input, uint32_t count, char *file,
                   uint16_t encrypt, uint32_t *key, uint8_t type) {
  FILE *binfile, *bintempfile;
  datsize = 0;
// set random number generator
  srand(time(NULL));
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
#ifdef UNIX
    struct stat64 st;
    stat64(input[i], &st);
    if (S_ISDIR(st.st_mode) == true) {  // it is a folder
#endif  // UNIX
#ifdef WINDOWS
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
#endif  // WINDOWS
      if (FolderScan(input[i], root, binfile, 0) == -1) {
		      fclose(binfile);
        return -1;
      }
    }
#ifdef UNIX
    else if (S_ISREG(st.st_mode) == true) {  // it is a file
      int64_t size = (int64_t)st.st_size;
#endif  // UNIX
#ifdef WINDOWS
    else {
      int64_t size = (st.nFileSizeHigh * (MAXDWORD+1)) + st.nFileSizeLow;
#endif  // WINDOWS
      snprintf(atomlog->MsgBuf, MSG_BUFFER_SIZE,
        "Writing file %s (%ld bytes)\n", input[i], size);
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
