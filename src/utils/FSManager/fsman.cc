#include "../../preproc.h"

#include "../../GameFS/gamefs.h"
#include "../../GameFS/strings.h"
#include "../../AtomError/AtomError.h"

#include <stdio.h>

AtomLog *atomlog;
AtomFS *atomfs;

int main(int arg, char *argc[]) {
// input parameters processing
  uint32_t *key = 0;
  atomlog = new AtomLog("fsman");
  snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "Start fsman version %s\n",
           _FSMAN_VERSION_);
  atomlog->LogMessage(atomlog->MsgBuf);
  char *descr[] = {
    "Big-Endian",
    "Little-Endian",
    "Mixed-Big-Endian",
    "Mixed-Little-Endian"
  };
  int8_t byteorder = BYTEORDER;
  if ((byteorder >= 0) && (byteorder < 4)) {
    snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s %s\n",
             "Used byteorder is", descr[byteorder]);
    atomlog->DebugMessage(atomlog->MsgBuf);
  }

  char help[] = "File System Manager - utility to work with Atom \
File System\nOptions:\n\t-t, --test\t\tTest default file system with standart \
mount file\n\t-t, --test [mountfile]\tTest file system\n\t-n, --new\t\tCreate \
new file\n\t\t-o [output]\t\tOutput file name\n\t\t-i [input]\t\tInput file \
and/or folders\n\t\t-e [crypt bytes]\tCount of bytes to encrypt\n\t\t-t \
[type]\t\tType of new file (critical, standart, addon)\n\t-o, --open \
[input]\tOpen file for reading\n";
  try { atomfs = new AtomFS(atomlog); }
  catch(int32_t i) { }
  if (arg == 1 || strcmp(argc[1], "-h") == 0 || \
      strcmp(argc[1], "--help") == 0 || strcmp(argc[1], "-help") == 0 || \
      strcmp(argc[1], "/?") == 0) {
    atomlog->DebugMessage(help);
    fprintf(stderr, "%s", help);
  }
// Testing filesystem
  else if (strcmp(argc[1], "-t") == 0 || strcmp(argc[1], "--test") == 0) {
    atomlog->DebugMessage("Test file system\n");
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
      int32_t encrypted = 0;
      char *output = 0;
      uint16_t encbytes = 0xFFFF;
      uint8_t type = type_addon;
// Flags
      bool error = false;
      int8_t flag_enc = 0;
      int8_t flag_out = 0;
      int8_t flag_in = 0;
      int8_t flag_type = 0;
// parse commandline
      for (int32_t i = 2, j = 0; i < arg; i++) {
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
            else if (strcmp(argc[i], "standard") == 0) {
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
        if (type == 0)
          snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE,
                   "Create new standard file %s\n", output);
        else if (type == 1)
          snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE,
                   "Create new addon file %s\n", output);
        else if (type == 0xFF)
          snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE,
                   "Create new critical file %s\n", output);
        atomlog->DebugMessage(atomlog->MsgBuf);
// Key input
// key is not predefined
        if (key == 0)
          key = PassPrint();
// input files count
        int32_t param = arg - 9;
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
      snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "Open file %s\n",
               argc[2]);
// we need a key to open the file
/*      if (key == 0)
        key = PassPrint();*/
      if (atomfs->Mount(argc[2], "/", key) != -1)
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
  atomlog->LogMessage("End fsman...\n");
  delete atomlog;
  return 0;
}
