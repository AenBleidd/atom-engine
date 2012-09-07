#include "../gamefs.h"
#include "../strings.h"
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
#ifdef _FSMAN_
int AtomFS::Save(FILE *input, char *output) {
  if (input == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    atomlog->DebugMessage("File pointer can't be equal 0\n");
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
    atomlog->DebugMessage("Output filename can't be empty\n");
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
      uint32_t result = GetCurrentDirectory(pathlen, curworkdir);
      if (result == 0) {
// strange error
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FOLDER);
        snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s: %i\n",
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
    }
    for (int32_t i = 0; i < args->count - 1; i++) {
      if (SetCurrentDirectory(args->output[i]) == 0) {
// there is no such directory.
// try to make this directory
        if ((CreateDirectory(args->output[i], NULL) == 0) &&
            (GetLastError() == ERROR_ALREADY_EXISTS)) {
// we can't create directory cause we have another file with this name
          atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FOLDER);
          snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s\n",
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
        } else if (SetCurrentDirectory(args->output[i]) == 0) {
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
    if (SetCurrentDirectory(curworkdir) == 0) {
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
    atomlog->DebugMessage("Couldn't open destination file\n");
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
