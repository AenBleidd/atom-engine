#include "../../preproc.h"

#include "../../GameFS/gamefs.h"
#include "../../AtomError/AtomError.h"

#include <stdio.h>

AtomLog *atomlog;
AtomFS *atomfs;

// 16-byte password input function
unsigned int* PassPrint(void) {
  unsigned int *key = new unsigned int[4];  // end key;
  char *input = new char[17];  // input string
// password input cycle
  while (true) {
    printf("%s:\n", "Print 16-symbol password");
    fgets(input, 17, stdin);
    if (strlen(input) == 16)
      break;
    else
      fprintf(stderr, "%s\n", "Password is too short! Try again");
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
  unsigned int *key = 0;
  atomlog = new AtomLog("fsman");
  const unsigned char versionlen = 30;
  char *version = new char[versionlen];
  snprintf(version, versionlen, "Start fsman version %s", _FSMAN_VERSION_);
  atomlog->LogMessage(version);
  delete [] version;
  char help[] = "File System Manager - utility to work with Atom \
File System\nOptions:\n\t-t, --test\t\tTest default file system with standart \
mount file\n\t-t, --test [mountfile]\tTest file system\n\t-n, --new\t\tCreate \
new file\n\t\t-o [output]\t\tOutput file name\n\t\t-i [input]\t\tInput file \
and/or folders\n\t\t-e [crypt bytes]\tCount of bytes to encrypt\n\t\t-t \
[type]\t\tType of new file (critical, standart, addon)\n\t-o, --open \
[input]\tOpen file for reading\n";
  try { atomfs = new AtomFS(atomlog); }
  catch(int i) { }
  if (arg == 1 || strcmp(argc[1], "-h") == 0 || \
      strcmp(argc[1], "--help") == 0 || strcmp(argc[1], "-help") == 0 || \
      strcmp(argc[1], "/?") == 0) {
    atomlog->DebugMessage(help);
    fprintf(stderr, "%s", help);
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
      fprintf(stderr, "%s", help);
    }
  }
// Create new file
  else if (strcmp(argc[1], "-n") == 0 || strcmp(argc[1], "--new") == 0) {
    if (arg > 9) {
      char **input = new char*[arg - 9];
      int encrypted = 0;
      char *output = 0;
      unsigned short int encbytes = 0;
      unsigned char type = 0;
// Flags
      bool error = false;
      signed char flag_enc = 0;
      signed char flag_out = 0;
      signed char flag_in = 0;
      signed char flag_type = 0;
// parse commandline
      for (int i = 2, j = 0; i < arg; i++) {
        if (error == false) {
// check all flags
// encrypt flag
          if ((strcmp(argc[i], "-e") == 0) && (flag_enc == 0) &&
              (flag_out != 1) && (flag_in != 1) && (flag_type != 1)) {
            flag_enc = 1;
// maybe this flag set after input files ?
            if (flag_in == 2)
              flag_in = -1;
          }
// outfile flag
          else if ((strcmp(argc[i], "-o") == 0) && (flag_out == 0) &&
                   (flag_enc != 1) && (flag_type != 1)) {
            flag_out = 1;
// maybe this flag set after input files ?
            if (flag_in == 2)
              flag_in = -1;
          }
// infile flag
          else if ((strcmp(argc[i], "-i") == 0) && (flag_in == 0) &&
                   (flag_enc != 1) && (flag_out != 1) && (flag_type != 1)) {
            flag_in = 1;
          }
// type flag
          else if ((strcmp(argc[i], "-t") == 0) && (flag_type == 0) &&
                   (flag_enc != 1) && (flag_out != 1) && (flag_in != 1)) {
            flag_type = 1;
            if (flag_in == 2)
              flag_in = -1;
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
// type
          else if (flag_type == 1) {
            flag_type = -1;
            if (strcmp(argc[i], "critical") == 0) {
              type = type_critical;
            }
            else if (strcmp(argc[i], "standart") == 0) {
              type = type_standart;
            }
            else if (strcmp(argc[i], "addon") == 0) {
              type = type_addon;
            } else {
// wrong type
              error = true;
            }
          } else {
// some error
            error = true;
          }
        } else {
// if there is an error
          atomlog->DebugMessage(help);
          fprintf(stderr, "%s", help);
        }
      }
      if (error == false) {
        unsigned char s = strlen(output) + 25;
        char *buf = new char[s];
        if (type == 0)
          snprintf(buf, s, "Create new standart file %s", output);
        else if (type == 1)
          snprintf(buf, s, "Create new addon file %s", output);
        else if (type == 0xFF)
          snprintf(buf, s, "Create new critical file %s", output);
        atomlog->DebugMessage(buf);
        delete [] buf;
// Key input
// key is not predefined
        if (key == 0)
          key = PassPrint();
// inpur files count
        int param = arg - 9;
        atomfs->Create(input, param, output, encbytes, key, type);
      }
// Make clean
      delete [] input;
    } else {
      atomlog->DebugMessage(help);
      fprintf(stderr, "%s", help);
    }
  }
  else if (strcmp(argc[1], "-o") == 0 || strcmp(argc[1], "--open") == 0) {
    if (arg == 3) {
      snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "Open file %s",
               argc[2]);
        atomfs->Mount(argc[2], "/");
        atomfs->Navigate();
    } else {
      atomlog->DebugMessage(help);
      fprintf(stderr, "%s", help);
    }
  } else {
    atomlog->DebugMessage(help);
    fprintf(stderr, "%s", help);
  }
// cleaning
  if (key != 0)
    delete [] key;
  delete atomfs;
  atomlog->LogMessage("End fsman...");
  delete atomlog;
  return 0;
}
