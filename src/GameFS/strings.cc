#include "strings.h"
ARGUMENTS* ParsePath(AtomLog *atomlog, char *input) {
  ARGUMENTS args;
  args.count = 0;
  args.output = 0;
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
// Set all pointer to zero
  for (int i = 0; i < tempcount; i++)
    tempoutput[i] = 0;
// count of real argunents
  args.count = 0;
// check for first root
  if (input[0] == '/') {
// we find first root
    args.count = 1;
    unsigned char s = 2;
    tempoutput[0] = new char[s];
    snprintf(tempoutput[0], s, "%s", "/");
  }
  int j = 0;
// Lets parse it!
  int len = strlen(input);
  for (int i = args.count; i < len; i++) {
    if ((input[i] == '/') || (i == len - 1)) {
// check for error
      if (input[i - 1] == '/') {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_MOUNTPOINT);
// Release allocated memory
        for (int z = 0; z < tempcount; z++)
          if (tempoutput[z] != 0)
            delete [] tempoutput[z];
        delete [] tempoutput;
        return 0;
      }
// end of the argument
      tempoutput[args.count][j] = '\0';
      args.count++;
      j = 0;
    } else {
      if (j == 0)
// new argument
        tempoutput[args.count] = new char[len - i + 2];
// copy symbol
      tempoutput[args.count][j++] = input[i];
    }
  }
// Copy all arguments
  args.output = new char*[args.count];
  int size = 0;
  for (int i = 0; i < args.count; i++) {
    size = strlen(tempoutput[i]) + 1;
    args.output[i] = new char[size];
    snprintf(args.output[i], size, "%s", tempoutput[i]);
  }
// Release allocated memory
  for (int i = 0; i < tempcount; i++)
    if (tempoutput[i] != 0)
      delete [] tempoutput[i];
  delete [] tempoutput;
// Return the count of arguments
  return &args;
}
ARGUMENTS* ParseArgs(AtomLog *atomlog, char *input) {
  ARGUMENTS args;
  args.count = 0;
  args.output = 0;
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
// Set all pointer to zero
  for (int i = 0; i < tempcount; i++)
    tempoutput[i] = 0;
// count of real argunents
  args.count = 0;



// Copy all arguments
  args.output = new char*[args.count];
  int size = 0;
  for (int i = 0; i < args.count; i++) {
    size = strlen(tempoutput[i]) + 1;
    args.output[i] = new char[size];
    snprintf(args.output[i], size, "%s", tempoutput[i]);
  }
// Release allocated memory
  for (int i = 0; i < tempcount; i++)
    if (tempoutput[i] != 0)
      delete [] tempoutput[i];
  delete [] tempoutput;
// Return the count of arguments
  return &args;
}
