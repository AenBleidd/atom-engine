#include "gamefs.h"
#include "strings.h"
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
  record->name = new char[record->namelen+1];
  strncpy(record->name, shortname, record->namelen);
  record->name[record->namelen] = '\0';
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
           "File %s was successfully written\n", shortname);
  atomlog->DebugMessage(atomlog->MsgBuf);
  return 0;
}
#endif  // _FSMAN_
