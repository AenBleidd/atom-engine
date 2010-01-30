#include "gamefs.h"
AtomFS::AtomFS() {
// create root directory
  root = new TREE_FOLDER;
  char name[] = "/\0";
  root->name = name;
#ifdef _FSMAN_
  root->flag |= ff_rw;
#else
  root->flag |= ff_ro;
#endif //_FSMAN_
  root->tree_folder = 0;
  root->tree_file = 0;
// Mounting...
  if (Mount("mount") == -1) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_MOUNT_FS);
    throw ERROR_MOUNT_FS;
  }
}
AtomFS::~AtomFS() {
// delete root directory
  delete root;
}
int AtomFS::Mount (char* filename) {
/*  atomlog.DebugMessage("Begin mounting filesystem...");
  atomlog.DebugMessage("Reading mount file...");
// open configuration file for reading
  FILE *conf = fopen(filename, "rb");
// error while opening the file
  if (conf == 0) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
    return -1;
  }
  long size = 0;
// get file size
  if (fseek(conf, 0, SEEK_END) != 0) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  size = ftell(conf);
  if (size == -1L) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  rewind(conf);
// reading the file
  char *buf = new char [size];
  if (fread(buf, 1, size, conf) != size) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    delete [] buf;
    return -1;
  }
// close configuration file
  fclose(conf);
  atomlog.DebugMessage("Mount file was successfully readed.");
// parsing...
  long pos = 0;
  unsigned int tsize = 0;
// flags
  bool end = false;
  bool fileflag = false;
  bool folderflag = false;
  bool priorflag = false;
  bool quotes = false;
  while (end != true) {
    atomlog.DebugMessage("Cycle");
// if it is a comment
    if (buf[pos] == 0x23) {
      atomlog.DebugMessage("Comment");
      while (buf[pos] != 0x0A || pos < size) { pos++; }
// last comment
      if (pos == size) { end = true; atomlog.DebugMessage("Last comment"); break; };
    }
// this is end
    else if (pos == size || pos > size) end = true;
// reading configuration
// if it is quotes
    else  if (buf[pos] == 0x22) {
        atomlog.DebugMessage("Quotes");
        tsize = 0;
        while (buf[pos++] != 0x22 || pos < size) tsize++;
        pos++;
        quotes = true;
        if (pos == size && buf[pos] != 0x22) {
          atomlog.SetLastErr(ERROR_CORE_FS, ERROR_PARSE_MOUNT_FILE_QUOTES);
          delete [] buf;
          return -1;
        }
      }
// if it is space
      else if (buf[pos] == 0x20 || buf[pos] == 0x0A) { pos++; }
// yeah! it's some symbol here!
      else {
        atomlog.DebugMessage("Symbol");
        tsize = 0;
        while (buf[pos] != 0x20 || buf[pos] != 0x0A) { pos++; tsize++; }
}
// rewind to read the string
      if (quotes == true) pos -= tsize+1;
      else pos -= tsize;
      quotes = false;
      char *file;
      char *mountpoint;
      char *prior;
// it's file
      if (fileflag == false) {
        atomlog.DebugMessage("File");
        file = new char [tsize];
        for (int i = 0; i < tsize+1; i++)
          file[i] = buf[pos++];
        file[tsize] = '\0';
        pos++;
        fileflag = true;
      }
// it's mountpoint
      else if (folderflag == false) {
        atomlog.DebugMessage("Mount point");
        mountpoint = new char [tsize];
        for (int i = 0; i < tsize+1; i++)
          mountpoint[i] = buf[pos++];
        mountpoint[tsize] = '\0';
        folderflag = true;
      }
// it's priority
      else if (priorflag == false) {
        atomlog.DebugMessage("Priority");
// too big priority or some another string
        if (tsize > 255) {
          atomlog.SetLastErr(ERROR_CORE_FS, ERROR_PARSE_MOUNT_FILE_PRIORITY);
          delete [] buf;
          return -1;
        }
        prior = new char [tsize];
        for (int i = 0; i < tsize+1; i++)
          prior[i] = buf[pos++];
        prior[tsize] = '\0';
        int num = atoi(prior);
        if (num < 0 || num > 255) {
          atomlog.SetLastErr(ERROR_CORE_FS, ERROR_PARSE_MOUNT_FILE_PRIORITY);
          delete [] buf;
          return -1;
        }
        fileflag = false, folderflag = false;
// mount this file
        if (Mount(file, mountpoint, (unsigned char)num) == -1) { delete [] buf; return -1; }
      }
  }
// cleaning
  delete [] buf;*/
  return 0;
}
// Mount single file
int AtomFS::Mount (char* filename, char* mountfolder, unsigned char priority) {
  char buffer[1000];
  char *pbuf = buffer;
  sprintf(pbuf, "Mounting %s to %s with %i", filename, mountfolder, priority);
  atomlog.DebugMessage(pbuf);
  return 0;
}