#include "gamefs.h"
#include "strings.h"
#ifdef _CRC_CHECK_
#include "crc32.h"
#endif  // _CRC_CHECK_
#ifdef UNIX
#include "errno.h"
#endif  // UNIX
#ifdef WINDOWS
FILE* fmemopen(void *s, size_t len, const char *modes) {
// Now actionally modes do nothing. Mode is always "r"
  FILE *file = new FILE;
#ifdef _MSC_VER
  file->_flag = _IOREAD | _IOSTRG;
// if we open it for writing
/*
  if (strcmp(modes, "w") == 0 || strcmp(modes, "wb") == 0)
	file->_flag |= _IOWRT;
*/
  file->_base = (char*)s;
  file->_ptr = file->_base;
  file->_cnt = len;
#endif  // _MSC_VER
#ifdef __BORLANDC__
  file->flags = _F_READ | _F_BIN;
  file->buffer = (unsigned char*)s;
  file->curp = file->buffer;
  file->bsize = len;
#endif  // __BORLANDC__
  return file;
}
#endif  // WINDOWS
FILE* AtomFS::Open(char *name, TREE_FOLDER *current) {
// Parse the path
  TREE_FILE *curfile = FindFileFromPath(atomlog, root, current,
                                        ParsePath(atomlog, name));
  if (curfile == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_PATH);
    return 0;
  }
// Get info about this file
// file that we create
  if (curfile->id == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    atomlog->LogMessage("Binary file is closed");
    return 0;
  }
// find the data
  if (fseek(curfile->id, curfile->offset + sizeof(HEADER), SEEK_SET) != 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return 0;
  }
// real data size
  uint64_t t = (curfile->size & ~3) / 4;
  if (curfile->size % 4 != 0)
    t++;
  t *= 4;
/*  uint8_t *buffer = new uint8_t[t];
// read the data
  if (fread(buffer, 1, t, curfile->id) != t) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    delete [] buffer;
    return 0;
  }
// Check the file
#ifdef _CRC_CHECK_
  if (curfile->crc != GenCRC32(buffer, t)) {
// Wrong crc
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_CRC32);
    delete [] buffer;
    return 0;
  }
#endif  // _CRC_CHECK_
// Decrypt
  uint64_t crypt = 0;
  if ((curfile->bytescrypt == 0xFFFF) ||
      (curfile->bytescrypt > curfile->size)) {
// decrypt all file
    crypt = curfile->size;
  } else {
    crypt = curfile->bytescrypt;
  }
// Decryption...
  uint32_t r[4];
// TODO(Lawliet): Check this!
  uint32_t *decrypt = new uint32_t[t/4];
  memcpy(decrypt, buffer, t);
  delete [] buffer;
  buffer = 0;
  t = (crypt & ~3) / 4;
  if (crypt % 4 != 0)
    t++;
  if (curfile->key != 0)
    Decrypt(decrypt, t, curfile->key, r, curfile->table);
  else
    Decrypt(decrypt, t, wake_key, r, curfile->table);
  uint8_t *decrypted = new uint8_t[curfile->size];
  memcpy(decrypted, decrypt, curfile->size);
  delete [] decrypt;
  decrypt = 0;*/
/******************* BEGIN NEW BLOCK **********************/
  uint8_t *buffer = new uint8_t[curfile->size];
  uint8_t *temp = new uint8_t[MAX_READ_LEN];
  uint32_t *tempbuf = new uint32_t[MAX_READ_LEN/4];
  uint64_t readed = 0;
  uint64_t reading = 0;
  uint64_t crc = 0;
  uint64_t crypt;
  uint64_t crypted = 0;
  uint64_t crypting = 0;
  uint32_t r[4];
  if ((curfile->bytescrypt == 0xFFFF) ||
      (curfile->bytescrypt > curfile->size)) {
// decrypt all file
    crypt = curfile->size;
  } else {
    crypt = curfile->bytescrypt;
  }
  if (curfile->key != 0) {
    r[0] = curfile->key[0];
    r[1] = curfile->key[1];
    r[2] = curfile->key[2];
    r[3] = curfile->key[3];        
  } else {
    r[0] = wake_key[0];
    r[1] = wake_key[1];
    r[2] = wake_key[2];
    r[3] = wake_key[3];         
  }
// begin reading the file
  while (readed <= t) {
    reading = t - readed;
    if (reading > MAX_READ_LEN)
      reading = MAX_READ_LEN;
// read the data
    if (fread(temp, 1, reading, curfile->id) != reading) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
      delete [] buffer;
      delete [] temp;
      delete [] tempbuf;
      return 0;
    }
// calculating crc
#ifdef _CRC_CHECK_
    crc = GenCRC32(temp, reading, crc);
#endif  // _CRC_CHECK_
    if (crypted <= crypt) {
      memcpy(tempbuf, temp, reading);
      crypting = crypt - crypted;
      crypting = (crypting & ~3) / 4;
      if (crypting % 4 != 0)
        crypting++;
// Decryption...
      Decrypt(tempbuf, crypting, r, r, curfile->table);
      memcpy(buffer + crypted, tempbuf, crypting * 4);
      crypted += crypting * 4;
    } else {
      memcpy(buffer + readed, temp, reading);
    }
    readed += reading;
  }
// Check the file
#ifdef _CRC_CHECK_
  if (curfile->crc != crc) {
// Wrong crc
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_CRC32);
    delete [] buffer;
    delete [] temp;
    delete [] tempbuf;
    return 0;
  }
#endif  // _CRC_CHECK_
  delete [] temp;
  delete [] tempbuf;
  temp = 0;
  tempbuf = 0;   
/******************** END NEW BLOCK ***********************/
// TODO(Lawliet): Check this!
// Create the file
  FILE *pfile;
  pfile = fmemopen(buffer, curfile->size, "rb");

  return pfile;
}
void AtomFS::Close(FILE *file) {
// save the pointer
  FILE *t = file;
// close the file
  fclose(file);
// release memory (if we really need this)
// TODO(Lawliet): Check if we need to release buffer too
  delete t;
  return;
}
#ifdef _FSMAN_
int AtomFS::Save(FILE *input, char *output) {
  if (input == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    atomlog->DebugMessage("File pointer can't be equal 0");
    return -1;
  }
  rewind(input);
// check filesize
  if (fseek(input, 0, SEEK_END) != 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  uint64_t size = ftell(input);
  if (size == -1L) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  rewind(input);
// Check the path to save file
// Parse the path
  ARGUMENTS *args = ParsePath(atomlog, output);
  if ((args == 0) || (args->count == 0)) {
// wrong path
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    atomlog->DebugMessage("Output filename can't be empty");
    for (int32_t i = 0; i < args->count; i++)
      if (args->output[i] != 0)
        delete [] args->output[i];
    delete [] args->output;
    delete args;
    args = 0;
    return -1;
  }
  if (args->count > 1) {
// Save current directory
    char *curworkdir;
    uint64_t pathlen = 1024;
    bool bflag = false;
    while (bflag == false) {
      curworkdir = new char[pathlen];
#ifdef UNIX
// something wrong...
      if (getcwd(curworkdir, pathlen) == 0) {
// strange errorcode
        if (errno != ERANGE) {
          atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FOLDER);
          snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s: %i",
                   "Can't get current folder. Error code is", errno);
          atomlog->DebugMessage(atomlog->MsgBuf);
          delete [] curworkdir;
          for (int32_t i = 0; i < args->count; i++)
            if (args->output[i] != 0)
              delete [] args->output[i];
          delete [] args->output;
          delete args;
          args = 0;
          return -1;
        }
        delete [] curworkdir;
// set new buffer size
        pathlen *= 10;
      } else {
        bflag = true;
      }
#endif  // UNIX
#ifdef WINDOWS
      uint32_t result = GetCurrentDirectory(pathlen, curworkdir);
      if (result == 0) {
// strange error
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FOLDER);
        snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s: %i",
                 "Can't get current folder. Error code is", GetLastError());
        atomlog->DebugMessage(atomlog->MsgBuf);
        delete [] curworkdir;
        for (int32_t i = 0; i < args->count; i++)
          if (args->output[i] != 0)
            delete [] args->output[i];
        delete [] args->output;
        delete args;
        args = 0;
        return -1;
      } else if (result > pathlen) {
// set new buffer size
        delete [] curworkdir;
        pathlen = result;
      } else {
        bflag = true;
      }
#endif  // WINDOWS
    }
    for (int32_t i = 0; i < args->count - 1; i++) {
#ifdef UNIX
// check the directory
      if (chdir(args->output[i]) != 0) {
#endif  // UNIX
#ifdef WINDOWS
      if (SetCurrentDirectory(args->output[i]) == 0) {
#endif  // WINDOWS
// there is no such directory.
// try to make this directory
#ifdef UNIX
        if ((mkdir(args->output[i], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH )
            != 0) && (errno == EACCES)) {
#endif  // UNIX
#ifdef WINDOWS
        if ((CreateDirectory(args->output[i], NULL) == 0) &&
            (GetLastError() == ERROR_ALREADY_EXISTS)) {
#endif  // WINDOWS
// we can't create directory cause we have another file with this name
          atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FOLDER);
          snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s",
                   "Can't open or create directory: there is a file \
                   with the same name");
          atomlog->DebugMessage(atomlog->MsgBuf);
// release memory
          delete [] curworkdir;
          for (int32_t z = 0; z < args->count; z++)
            if (args->output[z] != 0)
              delete [] args->output[z];
          delete [] args->output;
          delete args;
          args = 0;
          return -1;
#ifdef UNIX
        } else if (chdir(args->output[i]) != 0) {
#endif  // UNIX
#ifdef WINDOWS
        } else if (SetCurrentDirectory(args->output[i]) == 0) {
#endif  // WINDOWS
// we create directory but can't set it as working... WTF??? O_o
          atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FOLDER);
// release memory
          delete [] curworkdir;
          for (int z = 0; z < args->count; z++)
            if (args->output[z] != 0)
              delete [] args->output[z];
          delete [] args->output;
          delete args;
          args = 0;
          return -1;
        }
      }
    }
// restore work directory
#ifdef UNIX
    if (chdir(curworkdir) != 0) {
#endif  // UNIX
#ifdef WINDOWS
    if (SetCurrentDirectory(curworkdir) == 0) {
#endif  // WINDOWS
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FOLDER);
      delete [] curworkdir;
      for (int32_t z = 0; z < args->count; z++)
        if (args->output[z] != 0)
          delete [] args->output[z];
      delete [] args->output;
      delete args;
      args = 0;
      return -1;
    }
// Release the memory
    delete [] curworkdir;
    curworkdir = 0;
  }
// Release the memory
  for (int32_t i = 0; i < args->count; i++)
    if (args->output[i] != 0)
      delete [] args->output[i];
  delete [] args->output;
  delete args;
  args = 0;
// open destination file
  FILE *out = fopen(output, "wb");
  if (out == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
    atomlog->DebugMessage("Couldn't open destination file");
    return -1;  // nothing was written
  }
  char *buffer = new char[size];
  if (fread(buffer, 1, size, input) != size) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(out);
    delete [] buffer;
    return -1;
  }
  if (fwrite(buffer, 1, size, out) != size) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(out);
    delete [] buffer;
    return -1;
  }
  delete [] buffer;
  buffer = 0;
  fclose(out);
  return 0;
}
#endif  // _FSMAN_
