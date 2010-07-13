#include "gamefs.h"
AtomFS::AtomFS(AtomLog *log, unsigned int *key) {
  atomlog = log;
  wake_key = 0;
// If class being created without predefined key
  if (key != 0) {
    wake_key = key;
    GenKey(wake_key[0], wake_key[1], wake_key[2], wake_key[3]);
  }
// create root directory
  root = new TREE_FOLDER;
  char name[] = "/\0";
  root->name = name;
#ifdef _FSMAN_
  root->flag |= ff_rw;
#else
  root->flag |= ff_ro;
#endif  // _FSMAN_
  root->tree_folder = 0;
  root->tree_file = 0;
// Mounting...
#ifndef _FSMAN_
  if (Mount("mount") == -1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_MOUNT_FS);
    throw ERROR_MOUNT_FS;
  }
#endif  // _FSMAN_
}
AtomFS::~AtomFS() {
// delete root directory
  delete root;
}
// Table generation
void AtomFS::GenKey(unsigned int k0, unsigned int k1,
                    unsigned int k2, unsigned int k3) {
  long x, z;
  int p;
  static long tt[10]= {
                       0x726a8f3bL,  // table
                       0xe69a3b5cL,
                       0xd3c71fe5L,
                       0xab3c73d2L,
                       0x4d3a8eb3L,
                       0x0396d6e8L,
                       0x3d4c2f7aL,
                       0x9ee27cf3L, };
  wake_table[0] = k0;
  wake_table[1] = k1;
  wake_table[2] = k2;
  wake_table[3] = k3;
  for (p = 4; p < 256; p++) {
    x = wake_table[p-4] + wake_table[p-1];  // fill t
    wake_table[p] = (x>>3) ^ tt[(unsigned char)(x&7)];
  }
  for (p = 0; p < 23; p++)
    wake_table[p] += wake_table[p+89];  // mix first entries
  x = wake_table[33];
  z = wake_table[59] | 0x01000001L;
  z = z&0xff7fffffL;
  for (p = 0; p < 256; p++) {  // change top byte to
    x = (x&0xff7fffffL) + z;  // a permutation etc
    wake_table[p] = (wake_table[p] & 0x00ffffffL) ^ x;
  }
  wake_table[256] = wake_table[0];
  unsigned char y = (unsigned char)(x);
  for (p = 0; p < 256; p++) {  // further change perm.
// and other digits
    wake_table[p] = wake_table[y = (unsigned char)(wake_table[p^y]^y)];
    wake_table[y] = wake_table[p+1];
  }
}
#ifdef _FSMAN_
void AtomFS::Crypt(unsigned int *data, int lenght,
                   unsigned int k[4], unsigned int r[4], unsigned int *t) {
  int d;
  unsigned int r1, r2, r3, r4, r5, r6, *e, m = 0x00ffffff;
  r3 = k[0], r4 = k[1], r5 = k[2], r6 = k[3];
  if (lenght < 0)
    d = -1;
  else
    d = 1;
  e = data + lenght;
  while (data-e) {
    r1 = *data;
    r2 = r1^r6;
    *data = r2;  // Change into r1 for decoding.
    data += d;
    r3 = r3 + r2;
    r3 = (r3>>8&m)^wake_table[r3&255];
    r4 = r4+r3;
    r4 = (r4>>8&m)^wake_table[r4&255];
    r5 = r5+r4;
    r5 = (r5>>8&m)^wake_table[r5&255];
    r6 = r6+r5;
    r6 = (r6>>8&m)^wake_table[r6&255];
  }
  r[0] = r3, r[1] = r4, r[2] = r5, r[3] = r6;
}
#endif  // _FSMAN_
void AtomFS::Decrypt(unsigned int *data, int lenght, unsigned int k[4],
                     unsigned int r[4], unsigned int *t) {
  int d;
  unsigned int r1, r2, r3, r4, r5, r6, *e, m = 0x00ffffff;
  r3 = k[0], r4 = k[1], r5 = k[2], r6 = k[3];
  if (lenght < 0)
    d = -1;
  else
    d = 1;
  e = data + lenght;
  while (data-e) {
    r1 = *data;
    r2 = r1^r6;
    *data = r2;
    data += d;
    r3 = r3+r1;
    r3 = (r3>>8&m)^wake_table[r3&255];
    r4 = r4+r3;
    r4 = (r4>>8&m)^wake_table[r4&255];
    r5 = r5+r4;
    r5 = (r5>>8&m)^wake_table[r5&255];
    r6 = r6+r5;
    r6 = (r6>>8&m)^wake_table[r6&255];
  }
    r[0] = r3, r[1] = r4, r[2] = r5, r[3] = r6;
}
int AtomFS::Mount(char* filename) {
  atomlog->DebugMessage("Begin mounting filesystem...");
  atomlog->DebugMessage("Reading mount file...");
// open configuration file for reading
  FILE *conf = fopen(filename, "rb");
// error while opening the file
  if (conf == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
    return -1;
  }
  long size = 0;
// get file size
  if (fseek(conf, 0, SEEK_END) != 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  size = ftell(conf);
  if (size == -1L) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  rewind(conf);
// reading the file
  char *buf = new char[size];
  if (fread(buf, 1, size, conf) != size) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    delete [] buf;
    return -1;
  }
// close configuration file
  fclose(conf);
  atomlog->DebugMessage("Mount file was successfully readed.");
// parsing...
  long pos = 0;
  unsigned int tsize = 0;
// flags
  bool end = false;
  bool fileflag = false;
  bool folderflag = false;
  bool priorflag = false;
  bool quotes = false;
// reading configuration
  while (end != true) {
// this is end
    if (pos == size || pos > size) {
      end = true;
      break;
    }
// if it is space
    else if ((buf[pos] == 0x20 || buf[pos] == 0x0A) && pos < size)  // NOLINT
      pos++;
// if it is a comment
    if (buf[pos] == 0x23) {
      while (buf[pos] != 0x0A) pos++;
// last comment
      if (pos == size || pos == (size-1)) {
        end = true;
        break;
      }
      else pos++;  // NOLINT
    }
// if it is quotes
    if (buf[pos] == 0x22) {
        tsize = 0;
        pos++;
        while (buf[pos] != 0x22 && pos <= size) {
          pos++;
          tsize++;
        }
        quotes = true;
        if (pos == size && buf[pos] != 0x22) {
          atomlog->SetLastErr(ERROR_CORE_FS, ERROR_PARSE_MOUNT_FILE_QUOTES);
          delete [] buf;
          return -1;
        }
      } else {  // yeah! it's some symbol here!
        tsize = 0;
        while (buf[pos] != 0x20 && buf[pos] != 0x0A) {
          pos++;
          tsize++;
        }
      }
// rewind to read the string
      pos -= tsize;
      char *file;
      char *mountpoint;
      char *prior;
// it's file
      if (fileflag == false) {
        file = new char[tsize];
        for (int i = 0; i < tsize+1; i++)
          file[i] = buf[pos++];
        file[tsize] = '\0';
        fileflag = true;
      }
// it's mountpoint
      else if (folderflag == false) {  // NOLINT
        mountpoint = new char[tsize];
        for (int i = 0; i < tsize+1; i++)
          mountpoint[i] = buf[pos++];
        mountpoint[tsize] = '\0';
        folderflag = true;
      }
// it's priority
      else if (priorflag == false) {  // NOLINT
// too big priority or some another string
        if (tsize > 255) {
          atomlog->SetLastErr(ERROR_CORE_FS, ERROR_PARSE_MOUNT_FILE_PRIORITY);
          delete [] buf;
          return -1;
        }
        prior = new char[tsize];
        for (int i = 0; i < tsize+1; i++)
          prior[i] = buf[pos++];
        prior[tsize] = '\0';
        int num = atoi(prior);
        if (num < 0 || num > 255) {
          atomlog->SetLastErr(ERROR_CORE_FS, ERROR_PARSE_MOUNT_FILE_PRIORITY);
          delete [] buf;
          return -1;
        }
        fileflag = false, folderflag = false;
// mount this file
        if (Mount(file, mountpoint, (unsigned char)num) == -1) {
          delete [] buf;
          return -1;
        }
      }
      if (quotes == true) quotes = false;
  }
// cleaning
  delete [] buf;
  return 0;
}
// Mount single file
int AtomFS::Mount(char* filename, char* mountfolder, unsigned char priority) {
  const unsigned short s = 1000;
  char *pbuf = new char[s];
  snprintf(pbuf, s, "Mounting %s to %s with %i",
           filename, mountfolder, priority);
  atomlog->DebugMessage(pbuf);
  delete [] pbuf;
  return 0;
}
#ifdef _FSMAN_
unsigned long int datsize = 0;  // size of data in file
unsigned long int binsize = 0;  // size of bin in file
#endif  // _FSMAN_
#ifdef _FSMAN_
// Remove folder links
#ifdef UNIX
inline static int dot_exclude(const struct dirent64 *dir) {
  if ((dir->d_name[0] == '.') && (dir->d_name[1] == 0)) return 0;
  if ((dir->d_name[0] == '.') && (dir->d_name[1] == '.') &&
      (dir->d_name[2] == 0)) return 0;
  return 1;
}
#else
inline static int dot_exclude(const WIN32_FIND_DATA *dir) {
  if ((strcmp(dir->cFileName, ".") == 0) || (strcmp(dir->cFileName, "..") == 0))
    return 0;
  return 1;
}
#endif  // UNIX
#endif  // _FSMAN_
#ifdef _FSMAN_
int AtomFS::FolderScan(char *ch, FILE *dat, FILE *bin, int level = 0) {
  if (ch != NULL) {
// Write folder info
    RECORD record;
    record.flag = flag_folder;
    record.namelen = strlen(ch);
    record.name = ch;
    record.size = 0;
    record.offset = 0;
    record.crc = 0;
    if (fwrite(&record.flag, sizeof(record.flag), 1, dat) != 1) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
      return -1;
    }
    if (fwrite(&record.namelen, sizeof(record.namelen), 1, dat) != 1) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
      return -1;
    }
    if (fwrite(&record.name, 1, record.namelen, dat) != record.namelen) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
      return -1;
    }
// set new datasize
    char *buf = 0;
    datsize += (sizeof(record.flag) + sizeof(record.namelen) + record.namelen);
    const unsigned short s = strlen(ch) + 14;
    buf = new char[s];
    snprintf(buf, s, "Write folder %s", ch);
    atomlog->DebugMessage(buf);
    delete [] buf;
    buf = 0;
    char *curdir = 0;
#ifdef UNIX
// TODO(Lawliet): Check this string if first call chdir(ch) is NULL
    if (chdir(ch) != 0) return 0;
    struct dirent64 **eps;
    struct stat64 st;
    int n;
    n = scandir64("./", &eps, dot_exclude, alphasort64);
    if (n >= 0) {
      int cnt;
      for (cnt = 0; cnt < n; ++cnt) {
#else
      WIN32_FIND_DATA st;
      HANDLE hf;
// go to the subfolder
      const unsigned int size = strlen(ch) + 3;
      char *tmp = new char[size];
      snprintf(tmp, size, "%s\\*", ch);
      hf = FindFirstFile(tmp, &st);
      if (hf == INVALID_HANDLE_VALUE) {  // there is some error
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
        fclose(dat);
        fclose(bin);
        FindClose(hf);
        return -1;
      }
      do {
#endif  // UNIX
#ifdef UNIX
        if (eps[cnt]->d_type == DT_DIR) {  // it is a folder
          curdir = eps[cnt]->d_name;
#else
        if ((st.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) &&
            (dot_exclude(&st) == 1)) {
          const unsigned int fsize = sizeof(ch) + sizeof(st.cFileName) + 2;
          curdir = new char[fsize];
          snprintf(curdir, fsize, "%s\\%s", ch, st.cFileName);
#endif  // UNIX
          if (FolderScan(curdir, dat, bin, level+1) == -1) return -1;
#ifndef UNIX
          delete [] curdir;
          curdir = 0;
#endif  // UNIX
// End of catalogue
// TODO(Lawliet): Check recurse in this function
          record.flag = flag_eoc;
          record.namelen = 0;
          record.name = 0;
          record.size = 0;
          record.offset = 0;
          record.crc = 0;
          if (fwrite(&record.flag, sizeof(record.flag), 1, dat) != 1) {
            atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
            return -1;
          }
// Update datsize
          datsize += sizeof(record.flag);
        }
#ifdef UNIX
        if (eps[cnt]->d_type == DT_REG) {  // it is a file
          stat64(eps[cnt]->d_name, &st);
          curdir = eps[cnt]->d_name; // but it is a file not folder
          const unsigned short sk = strlen(eps[cnt]->d_name) + 45;
          long int fsize = (long int)st.st_size;
#else
        else if (dot_exclude(&st) == 1) {
          const unsigned short sk = strlen(st.cFileName) + 45;
          curdir = st.cFileName;
          long int fsize = (st.nFileSizeHigh * (MAXDWORD+1)) + st.nFileSizeLow;
#endif  // UNIX
          buf = new char[sk];
          snprintf(buf, sk, "Writing file %s (%ld bytes)", curdir, fsize);
          atomlog->DebugMessage(buf);
          delete [] buf;
          buf = 0;
#ifndef UNIX
          const unsigned int st_size = strlen(ch) + strlen(st.cFileName) + 2;
          curdir = new char[st_size];
          snprintf(curdir, st_size, "%s\\%s", ch, st.cFileName);
#endif  // UNIX
          if (Write(curdir, dat, bin) == -1) return -1;
#ifndef UNIX
          delete [] curdir;
          curdir = 0;
#endif  // UNIX
        }
#ifdef UNIX
      }
#else
      } while (FindNextFile(hf, &st)!= 0);
      FindClose(hf);
      delete [] tmp;
      tmp = 0;
#endif  // UNIX
#ifdef UNIX
    for (cnt = 0; cnt < n; ++cnt) free(eps[cnt]);
    free(eps);
    } else {
      const unsigned short s = strlen(eps[n]->d_name) + 28;
      buf = new char[s];
      snprintf(buf, s, "Couldn't open the directory %s",  eps[n]->d_name);
      atomlog->LogMessage(buf);
      delete [] buf;
      buf = 0;
      return -1;
    }
  chdir("..");
#endif  // UNIX
  }
  return 0;
}
#endif  // _FSMAN_
#ifdef _FSMAN_
int AtomFS::Write(char *in,  FILE *dat, FILE *bin) {
  FILE *file = fopen(in, "rb");
  if (in == NULL) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
    return -1;  // nothing was written
  }
// Collect info about file
  RECORD record;
  record.flag = flag_file;
  record.namelen = strlen(in);
  record.name = in;
// check filesize
  if (fseek(file, 0, SEEK_END) != 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(file);
    return -1;
  }
  record.size = ftell(file) - 1;
  if (record.size == -1L) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(file);
    return -1;
  }
  rewind(file);
  unsigned long int crc = mask;
// encrypt
  unsigned long int count;
  if ((bytescrypt == 0xFFFF) || (bytescrypt > record.size))
    count = record.size;
  else
    count = bytescrypt;
  unsigned int *buf = new unsigned int[count];
  if (fread(buf, 1, count, file) != count) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(file);
    delete [] buf;
    return -1;
  }
  unsigned int r[4];
  Crypt(buf, count, wake_key, r, wake_table);
  unsigned long int t = count;
// write to disk crypted data
  if (fwrite(buf, 1, count, bin) != count) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(file);
    delete [] buf;
    return -1;
  }
  unsigned int *tempbuf = buf;
  while (t--)
    crc = crc32table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
  delete [] tempbuf;
  tempbuf = 0;
  buf = 0;
// writing to disk and calculating crc
  for (unsigned long int i = count-1; i < record.size; i++) {
    if (fread(&t, 1, 1, file) != 1) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
      fclose(file);
      return -1;
    }
    crc = crc32table[(crc ^ t) & 0xFF] ^ (crc >> 8);
    if (fwrite(&t, 1, 1, bin) != 1) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
      fclose(file);
      return -1;
    }
  }
  fflush(file);
  fclose(file);
  record.crc = crc ^ mask;
  record.offset = datsize;
// Write data
  if (fwrite(&record.flag, sizeof(record.flag), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    return -1;
  }
  if (fwrite(&record.namelen, sizeof(record.namelen), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    return -1;
  }
  if (fwrite(&record.name, 1, record.namelen, dat) != record.namelen) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    return -1;
  }
  if (fwrite(&record.size, sizeof(record.size), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    return -1;
  }
  if (fwrite(&record.offset, sizeof(record.offset), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    return -1;
  }
  if (fwrite(&record.crc, sizeof(record.crc), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    return -1;
  }
  datsize += (sizeof(record.flag) + sizeof(record.namelen) + record.namelen +
             sizeof(record.size) + sizeof(record.offset) + sizeof(record.crc));

  binsize += record.size;
  const unsigned short int s = strlen(in) + 31;
  char *mes = new char[s];
  snprintf(mes, s, "File %s was successfully written", in);
  atomlog->DebugMessage(mes);
  delete [] mes;
  return 0;
}
#endif  // _FSMAN_
#ifdef _FSMAN_
int AtomFS::Create(char **input, unsigned int count, char *file,
                   unsigned short int encrypt, unsigned int *key,
                   bool keyflag) {
  FILE *binfile, *datfile, *bintempfile, *dattempfile;
  datsize = 0;
// Generate crypt key
// If crypt key is predefined new key is ignored
  if(wake_key == 0) {
    wake_key = key;
    GenKey(wake_key[0], wake_key[1], wake_key[2], wake_key[3]);
  }
// set encrypt bytes count
  bytescrypt = encrypt;
// Create name of the output files
  char *bin, *dat;
  unsigned int namelen;
  namelen = strlen(file)+5;
  bin = new char[namelen];
  dat = new char[namelen];
  snprintf(bin, namelen, "%s.bin", file);
  snprintf(dat, namelen, "%s.dat", file);
// check if output files exist
  binfile = fopen(bin, "r");
  datfile = fopen(dat, "r");
  if ((binfile != NULL) || (datfile != NULL)) {  // files exist
    fclose(binfile);
    fclose(datfile);
// rename the files
    int i = 0, s = 0;
    bool flag = false;
    char *bintemp, *dattemp;
    while (flag != true) {
      int j = i;
      int k = 0;
      while (j > 9)  j /= 10, k++;
      s = strlen(bin) + k + 5;
      bintemp = new char[s];
      dattemp = new char[s];
// create filename
      snprintf(bintemp, s, "%s.%03i", bin, i);
      snprintf(dattemp, s, "%s.%03i", dat, i);
      bintempfile = fopen(bintemp, "r");
      dattempfile = fopen(dattemp, "r");
      if ((bintempfile == NULL) && (dattempfile == NULL)) {
        if ((rename(bin, bintemp) == 0) && (rename(dat, dattemp) == 0))
          flag = true;
        else i++; // NOLINT
      } else {
        fclose(bintempfile);
        fclose(dattempfile);
        i++;
      }
      delete [] bintemp;
      delete [] dattemp;
    }
  }
// open files for writing
  binfile = fopen(bin, "wb");
  datfile = fopen(dat, "wb");
// delete unuse filenames
  delete [] bin;
  bin = 0;
  delete [] dat;
  dat = 0;

// Create file (NIGHTMARE!!!)
// set static header information
  HEADER header;
  header.magic = magic;
  if (keyflag == false) {
    header.version = version;
  }
  else {
    header.version = addon_version;
    header.addon_key[0] = wake_key[0];
    header.addon_key[1] = wake_key[1];
    header.addon_key[2] = wake_key[2];
    header.addon_key[3] - wake_key[3];
  }
  header.crypt = bytescrypt;
// set variable header information
  header.bincount = 1;
  header.binsize = 0;
  header.datcount = 1;
  header.datsize = 0;
  datsize = sizeof(HEADER);
  if (keyflag == false)
    datsize -= sizeof(header.addon_key);
// write empty header to file
  if (fwrite("0", datsize, 1, datfile) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(datfile);
    fclose(binfile);
    return -1;
  }
// write root directory
  RECORD record;
  record.flag = flag_folder;
  record.namelen = 1;
  record.name = new char[record.namelen + 1];
  snprintf(record.name, record.namelen + 1, "%s", "/");
  record.size = 0;
  record.offset = 0;
  record.crc = 0;
  if (fwrite(&record.flag, sizeof(record.flag), 1, datfile) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(datfile);
        fclose(binfile);
        delete [] record.name;
        return -1;
  }
  if (fwrite(&record.namelen, sizeof(record.namelen), 1, datfile) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(datfile);
        fclose(binfile);
        delete [] record.name;
        return -1;
  }
  if (fwrite(&record.name, 1, record.namelen, datfile) != record.namelen) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(datfile);
        fclose(binfile);
        delete [] record.name;
        return -1;
  }
// set new datasize
  datsize += (sizeof(record.flag)+sizeof(record.namelen)+record.namelen);
  delete [] record.name;
  record.name = 0;
// Scanning...
  char *buf = 0;
  for (int i = 0; i < count; i++) {
#ifdef UNIX
    struct stat64 st;
    stat64(input[i], &st);
    if (S_ISDIR(st.st_mode) == true) {  // it is a folder
#else
    WIN32_FIND_DATA st;
    HANDLE hf;
    hf = FindFirstFile(input[i], &st);
    if (hf == INVALID_HANDLE_VALUE) {  // there is some error
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
      fclose(datfile);
      fclose(binfile);
      FindClose(hf);
      return -1;
    }
    FindClose(hf);
    if (st.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
#endif  // UNIX
      if (FolderScan(input[i], datfile, binfile, 0) == -1) {
        fclose(datfile);
        fclose(binfile);
        return -1;
      }
    }
#ifdef UNIX
    else if (S_ISREG(st.st_mode) == true) {  // it is a file
      long int size = (long int)st.st_size;
#else
    else {
      long int size = (st.nFileSizeHigh * (MAXDWORD+1)) + st.nFileSizeLow;
#endif  // UNIX
      const unsigned short s = strlen(input[i]) + 45;
      buf = new char[s];
      snprintf(buf, s, "Writing file %s (%ld bytes)",
               input[i], size);
      atomlog->DebugMessage(buf);
      delete [] buf;
      buf = 0;
      if (Write(input[i], datfile, binfile) != 0) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(datfile);
        fclose(binfile);
        return -1;
      }
    }
  }
// Write end of folder flag
  record.flag = flag_eoc;
  record.namelen = 0;
  record.name = 0;
  record.size = 0;
  record.offset = 0;
  record.crc = 0;
  if (fwrite(&record.flag, sizeof(record.flag), 1, datfile) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(datfile);
    fclose(binfile);
    return -1;
  }
// Update datsize
  datsize += sizeof(record.flag);
// We have worked with bin file. Lets close it
  fclose(binfile);
// rewind the file to write the header
  rewind(datfile);
// writing the header
  header.binsize = binsize;
  header.datsize = datsize;
  if (fwrite(&header.magic, sizeof(header.magic), 1, datfile) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(datfile);
    return -1;
  }
  if (fwrite(&header.version, sizeof(header.version), 1, datfile) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(datfile);
    return -1;
  }
  if (fwrite(&header.crypt, sizeof(header.crypt), 1, datfile) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(datfile);
    return -1;
  }
  if (fwrite(&header.bincount, sizeof(header.bincount), 1, datfile) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(datfile);
    return -1;
  }
  if (fwrite(&header.binsize, sizeof(header.binsize), 1, datfile) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(datfile);
    return -1;
  }
  if (fwrite(&header.datcount, sizeof(header.datcount), 1, datfile) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(datfile);
    return -1;
  }
  if (fwrite(&header.datsize, sizeof(header.datsize), 1, datfile) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(datfile);
    return -1;
  }
// TODO(Lawliet): Check this
  if (keyflag == true) {
    if (fwrite(&header.addon_key, sizeof(header.addon_key), 1, datfile) != 1) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
      fclose(datfile);
      return -1;
    }
  }
// end work with datfile
  fclose(datfile);
  return 0;
}
#endif  // _FSMAN_
