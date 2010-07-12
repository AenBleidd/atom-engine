#include "../../preproc.h"

#include "../../GameFS/gamefs.h"
#include "../../AtomError/AtomError.h"

#include <stdio.h>

AtomLog *atomlog;
AtomFS *atomfs;

// 16-byte password input function
unsigned int* PassPrint (void) {
  unsigned int *key = new unsigned int[4]; // end key;
  char *input = new char[17]; // input string
// password input cycle
  while(true) {
    printf("%s:\n", "Print 16-symbol password");
    scanf("%16s", input);
    if (strlen(input) == 16)
      break;
    else {
      printf("Password is too short! Try again\n");
    }
  }
  input[16] = '\0';
// processing input string
  key[0] = (input[0] << 8) + (input[1] << 8) + (input[2] << 8) + input[3];
  key[1] = (input[4] << 8) + (input[5] << 8) + (input[6] << 8) + input[7];
  key[2] = (input[8] << 8) + (input[9] << 8) + (input[10] << 8) + input[11];
  key[3] = (input[12] << 8) + (input[13] << 8) + (input[14] << 8) + input[15];
  delete [] input;
  return key;
}

int main(int arg, char *argc[]) {
// input parameters processing
  atomlog = new AtomLog;
  const unsigned char versionlen = 30;
  char *version = new char[versionlen];
  snprintf(version, versionlen, "Start fsman version %s", _FSMAN_VERSION_);
  atomlog->LogMessage(version);
  delete [] version;
/*NOLINT*/  char help[] = "File System  Manager - utility to work with Atom File System\nOptions:\n\t-t, --test\t\tTest default file system with standart mount file\n\t-t, --test [mountfile]\tTest file system\n\t-n, --new\t\tCreate new file\n\t\t-o [output]\t\tOutput file name\n\t\t-i [input]\t\tInput file and/or folders\n\t\t-e [crypt bytes]\tCount of bytes to encrypt\n";
  try { atomfs = new AtomFS(atomlog); }
  catch(int i) { }
  if (arg == 1 || strcmp(argc[1], "-h") == 0 || \
      strcmp(argc[1], "--help") == 0 || strcmp(argc[1], "-help") == 0 || \
      strcmp(argc[1], "/?") == 0) {
    atomlog->DebugMessage(help);
    printf("%s", help);
  }
// Testing filesystem
  else if (strcmp(argc[1], "-t") == 0 || strcmp(argc[1], "--test") == 0) {
    atomlog->DebugMessage("Test file system");
    if (arg == 2) {
      atomfs->Mount("mount");
    }
    else if (arg == 3) {
      atomfs->Mount(argc[2]);
    } else {
      atomlog->DebugMessage(help);
      printf("%s", help);
    }
  }
// Create new file
  else if (strcmp(argc[1], "-n") == 0 || strcmp(argc[1], "--new") == 0) {
    if (arg > 7) {
      char **input = new char*[arg-7];
      int encrypted = 0;
      char *output = 0;
      unsigned short int encbytes = 0;
// Flags
      bool error = false;
      signed char flag_enc = 0;
      signed char flag_out = 0;
      signed char flag_in = 0;
// parse commandline
      for (int i = 2, j = 0; i < arg; i++) {
        if (error == false) {
// check all flags
// encrypt flag
          if ((strcmp(argc[i], "-e") == 0) && (flag_enc == 0) &&
              (flag_out != 1) && (flag_in != 1)) {
            flag_enc = 1;
// maybe this flag set after input files ?
            if (flag_in == 2)
              flag_in = -1;
          }
// outfile flag
          else if ((strcmp(argc[i], "-o") == 0) && (flag_out == 0) &&
                   (flag_enc != 1)) {
            flag_out = 1;
// maybe this flag set after input files ?
            if (flag_in == 2)
              flag_in = -1;
          }
// infile flag
          else if ((strcmp(argc[i], "-i") == 0) && (flag_in == 0) &&
                   (flag_enc != 1) && (flag_out != 1)) {
            flag_in = 1;
          }
// if here is not flag but smth else
// encrypt bytes
          else if (flag_enc == 1) {
// there can't be another value
            flag_enc = -1;
            encrypted = atoi(argc[i]);
// wrong diapasone
            if ((encrypted == 0) || (encrypted < -1) || (encrypted > 65535))
              error = true;
            else
// -1 or 65535 - all file will be encrypted (must be 0xFFFF)
              encbytes = encrypted;
          }
// output file
          else if (flag_out == 1) {
// there can't be two output files
            flag_out = -1;
            output = argc[i];
          }
// input files
          else if (flag_in > 0) {
// this is at least first input file
            flag_in = 2;
            input[j++] = argc[i];
          }
// some error
          else {
            error = true;
          }
        }
// if there is an error
        else {
          delete [] input;
          atomlog->DebugMessage(help);
          fprintf(stderr, "%s", help);
        }
      }
      unsigned char s = strlen(output) + 17;
      char *buf = new char[s];
      snprintf(buf, s, "Create new file %s", output);
      atomlog->DebugMessage(buf);
      delete [] buf;
// Key input
      unsigned int *key = PassPrint();
      atomfs->Create(input, arg-7, output, encbytes, key);
// Make clean
      delete [] key;
      delete [] input;
    } else {
      atomlog->DebugMessage(help);
      fprintf(stderr, "%s", help);
    }
  } else {
    atomlog->DebugMessage(help);
    fprintf(stderr, "%s", help);
  }
// cleaning
  delete atomfs;
  atomlog->LogMessage("End fsman...");
  delete atomlog;
  return 0;
}
