#include "gamefs.h"
#include "strings.h"
#ifdef _CRC_CHECK_
#include "crc32.h"
#endif  // _CRC_CHECK_
FILE* AtomFS::Open(char *name) {
// Parse the path
  ARGUMENTS *args = ParsePath(atomlog, name);
  TREE_FILE *curfile = FindFileFromPath(atomlog, root, 0,
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
int AtomFS::Close(FILE *file) {
// save the pointer
  FILE *t = file;
// close the file
  fclose(file);
// release memory (if we really need this)
  delete t;
  return 0;
}
