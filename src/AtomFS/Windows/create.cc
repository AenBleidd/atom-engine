#include <AtomFS/atomfs.h>
bool AtomFS::Create(char **input, uint64_t count, char *filename) {
  if ((input == 0 && count > 0) || (input != 0 && count == 0))
  {
    atomlog->ErrorMessage(AtomFSClassName, "Input file count not equal input file names");
    return false;
  }
  if (input == 0 && count == 0)
  {
    atomlog->WarningMessage(AtomFSClassName,"No input files. Output file will not contain any data only header ");
  }
  AtomFSHeader *header = new AtomFSHeader(0, magic, version, 0);
  uint64_t filetableAddress = 15; // size of the header
  // before we will create the new file we must rename existing one
  char *tempfilename = filename;
  uint32_t i = 0, j = 0, k = 0, s = 0;
  HANDLE hOutputFile = CreateFile(tempfilename, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  if (hOutputFile != INVALID_HANDLE_VALUE) {
    CloseHandle(hOutputFile);
    while (true) {
      j = i++;
      k = 0;
      while (j > 9)  j /= 10, k++;
      s = strlen(filename) + k + 5; // 5 is magic constant for leading zeros if k < 3, dot and last zero
      tempfilename = new char[s];
      snprintf(tempfilename, s, "%s.%03d", filename, j);
      hOutputFile  = CreateFile(tempfilename, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
      if (hOutputFile == INVALID_HANDLE_VALUE) {
        if (MoveFileEx(filename, tempfilename, MOVEFILE_WRITE_THROUGH) == true) {
          break;
        }
      }
      else {
        CloseHandle(hOutputFile);
      }
    }
  }
  hOutputFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hOutputFile == INVALID_HANDLE_VALUE) {
    atomlog->ErrorMessage(AtomFSClassName, "Can't create output file on disk");
    return false;
  }
  CloseHandle(hOutputFile);
  return true;
}