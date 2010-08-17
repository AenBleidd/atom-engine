#include "gamefs.h"
#include "strings.h"
#ifdef _CRC_CHECK_
#include "crc32.h"
#endif  // _CRC_CHECK_
#ifdef UNIX
#include "errno.h"
#endif  // UNIX
FILE* AtomFS::Open(char *name, TREE_FOLDER *current) {
// Parse the path
  ARGUMENTS *args = ParsePath(atomlog, name);
  TREE_FILE *curfile = FindFileFromPath(atomlog, root, current,
                                        ParsePath(atomlog, name));
  if (curfile == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_PATH);
    return 0;
  }
// Get info about this file
// file that we create
  FILE file;
  if (curfile->id == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    atomlog->LogMessage("Binary file is closed");
    return 0;
  }
// find the data
  if (fseek(curfile->id, curfile->offset, SEEK_SET) != 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return 0;
  }
// data buffer
  char *buffer = new char[curfile->size];
// read the data
  if (fread(buffer, curfile->size, 1, curfile->id) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    delete [] buffer;
    return 0;
  }
// Check the file
#ifdef _CRC_CHECK_
  if (curfile->crc != GenCRC32((unsigned char*)buffer, curfile->size)) {
// Wrong crc
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_CRC32);
    delete [] buffer;
    return 0;
  }
#endif  // _CRC_CHECK_
// Decrypt
  unsigned long int crypt = 0;
  if ((curfile->bytescrypt == 0xFFFF) || (curfile->bytescrypt > curfile->size)) {
// decrypt all file
    crypt = curfile->size;
  } else {
    crypt = curfile->bytescrypt;
  }
// Decryption...
  unsigned int *decryptbuf = new unsigned int[crypt];
  for (unsigned long int i = 0; i < crypt; i++)
    decryptbuf[i] = (unsigned int)buffer[i];
  unsigned int r[4];
// TODO(Lawliet): Check this!
  Decrypt(decryptbuf, crypt, ((curfile->key != 0)?curfile->key:wake_key),
          r, curfile->table);
  for (unsigned long int i = 0; i < crypt; i++)
    buffer[i] = (char)decryptbuf[i];
// TODO(Lawliet): Check this!
// Create the file
#ifdef UNIX
// defined in libio.h
  file._flags = _IO_MAGIC |  _IO_NO_WRITES;
  file._IO_read_base = buffer;
  file._IO_read_ptr = file._IO_read_base;
  file._IO_read_end = buffer + curfile->size - 1;
#else
  file._flag = _IOREAD | _IOSTRG;
  file._base = buffer;
  file._ptr = file._base;
  file._cnt = curfile->size;
#endif  // UNIX
  FILE *pfile = new FILE;
  *pfile = file;
  return pfile;
}
void AtomFS::Close(FILE *file) {
// save the pointer
  FILE *t = file;
// close the file
  fclose(file);
// release memory (if we really need this)
  delete t;
  return;
}
#ifdef _FSMAN_
int AtomFS::Save(FILE *input, char *output) {
  if (input == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s",
             "File pointer can't be equal 0");
    atomlog->DebugMessage(atomlog->MsgBuf);
    return -1;
  }
  rewind(input);
// check filesize
  if (fseek(input, 0, SEEK_END) != 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  unsigned long int size = ftell(input) - 1;
  if (size == -1L) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  rewind(input);
// Check the way where to save file
// Parse the path
  ARGUMENTS *args = ParsePath(atomlog, output);
  if ((args == 0) || (args->count == 0)) {
// wrong path
    atomlog->SetLastErr(ERROR_CORE_FS,ERROR_WRITE_FILE);
    snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s",
             "Output filename can't be empty");
    atomlog->DebugMessage(atomlog->MsgBuf);
    for (int i = 0; i < args->count; i++)
      if(args->output[i] != 0)
        delete [] args->output[i];
    delete [] args->output;
    delete args;
    args = 0;
    return -1;
  }
  if (args->count > 1) {
// Save current directory
    char *curworkdir;
    unsigned long int pathlen = 1024;
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
          for (int i = 0; i < args->count; i++)
            if(args->output[i] != 0)
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
#else
      unsigned int result = GetCurrentDirectory(pathlen, curworkdir);
      if (result == 0) {
// strange error
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FOLDER);
        snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s: %i",
                 "Can't get current folder. Error code is", GetLastError());
        atomlog->DebugMessage(atomlog->MsgBuf);
        delete [] curworkdir;
        for (int i = 0; i < args->count; i++)
          if(args->output[i] != 0)
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
#endif  // UNIX
    }
    for (int i = 0; i < args->count - 1; i++) {
#ifdef UNIX
// check the directory
      if (chdir(args->output[i]) != 0) {
#else
      if (SetCurrentDirectory(args->output[i]) == 0) {
#endif  // UNIX
// there is no such directory.
// try to make this directory
#ifdef UNIX
        if ((mkdir(args->output[i], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH )
            != 0) && (errno == EACCES)) {
#else
        if ((CreateDirectory(args->output[i], NULL) == 0) &&
            (GetLastError() == ERROR_ALREADY_EXISTS)) {
#endif  // UNIX
// we can'r create directory cause we have another file with this name
          atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FOLDER);
          snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s",
                   "Can't open or create directory: there is a file \
                   with the same name");
          atomlog->DebugMessage(atomlog->MsgBuf);
// release memory
          delete [] curworkdir;
          for (int z = 0; z < args->count; z++)
            if (args->output[z] != 0)
              delete [] args->output[z];
          delete [] args->output;
          delete args;
          args = 0;
          return -1;
#ifdef UNIX
        } else if (chdir(args->output[i]) != 0) {
#else
        } else if (SetCurrentDirectory(args->output[i]) == 0) {
#endif  // UNIX
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
#else
    if (SetCurrentDirectory(curworkdir) == 0) {
#endif // UNIX
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FOLDER);
      delete [] curworkdir;
      for (int z = 0; z < args->count; z++)
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
  for (int i = 0; i < args->count; i++)
    if(args->output[i] != 0)
      delete [] args->output[i];
  delete [] args->output;
  delete args;
  args = 0;
// open destination file
  FILE *out = fopen(output, "wb");
  if (out == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
    snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s",
              "Couldn't open destination file");
    atomlog->DebugMessage(atomlog->MsgBuf);
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