#include "gamefs.h"
#include "strings.h"
#ifdef _CRC_CHECK_
#include "crc32.h"
#endif  // _CRC_CHECK_
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
    atomlog->LogMessage("Binary file is closed\n");
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
