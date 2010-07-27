#include "strings.h"
int ParsePath(AtomLog *atomlog, char *input, char **output) {
// Check for empty string
  if ((input == 0) || (strlen(input) == 0)) {
    atomlog->SetLastWrn(WARNING_CORE_FS, WARNING_EMPTY_STRING);
    return 0;
  }
  int tempcount;
// find potential maximum count af arguments
  if ((strlen(input) % 2) == 0) {
    tempcount = strlen(input) / 2;
  } else {
    tempcount = (strlen(input) + 1 ) / 2;
  }
// allocate memory for the potential maximum count of arguments
  char **tempoutput = new char*[tempcount];


// Release allocated memory
  delete [] tempoutput;
  return 0;
}
int ParseArgs(AtomLog *atomlog, char *input, char **output) {
  if ((input == 0) || (strlen(input) == 0)) {
    atomlog->SetLastWrn(WARNING_CORE_FS, WARNING_EMPTY_STRING);
    return 0;
  }
  return 0;
}
