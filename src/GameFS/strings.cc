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
      tempoutput[args.count][j++] = input[i];
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
  ARGUMENTS *pargs = new ARGUMENTS;
  *pargs = args;
  return pargs;
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
// Lets do it!
  int len = strlen(input) - 1;
// symbols
  const unsigned char symbol_space = ' ';
  const unsigned char symbol_quote = '\'';
  const unsigned char symbol_doublequote = '\"';
  const unsigned char symbol_slash = '\\';
// TODO(Lawliet): Check this
  const unsigned char symbol_tab = '\t';
// flags
  bool arg = false;
  bool quote = false;
  bool doublequote = false;
  int j = 0;
  for (int i = 0; i < len; i++) {
    if ((input[i] == symbol_space) || (input[i] == symbol_tab)) {
      if (arg == true) {
// maybe it's the end of some argument ?
        if ((quote == false) && (doublequote == false)) {
// space outside quotes
          tempoutput[args.count++][j] = '\0';
          j = 0;
          arg = false;
        } else {
// just copy the symbol
          tempoutput[args.count][j++] = input[i];
        }
      }
    } else if(input[i] == symbol_slash) {
// we got slash here...
      if ((input[i + 1] == symbol_quote) || (input[i + 1] == symbol_slash) ||
          (input[i + 1] == symbol_doublequote) ||
          (input[i + 1] == symbol_space)) {
// just copy next symbol
        tempoutput[args.count][j++] = input[++i];
      } else {
// it is just slash
        if (arg == false) {
// create new argument
          tempoutput[args.count] = new char[len - i + 2];
          j = 0;
          arg = true;
        }
// copy the symbol
        tempoutput[args.count][j++] = input[i];
      }
    } else if (input[i] == symbol_quote) {
      if (quote == false) {
        if (i == 0) {
          if (i == len - 1) {
            atomlog->SetLastErr(ERROR_CORE_FS,ERROR_LOST_QUOTES);
// Release allocated memory
            for (int i = 0; i < tempcount; i++)
              if (tempoutput[i] != 0)
                delete [] tempoutput[i];
            delete [] tempoutput;
            return 0;
          }
          if ((input[i + 1] != symbol_space) && (input[i + 1] != symbol_tab)) {
// Quotes begin...
            quote = true;
            tempoutput[args.count] = new char[len - i + 2];
            arg = true;
          }
        } else if (((input[i - 1] == symbol_space) || (input[i - 1] == symbol_tab)) &&
            (input[i + 1] != symbol_space) && (input[i + 1] != symbol_tab)) {
// Quotes begin...
          quote = true;
          tempoutput[args.count] = new char[len - i + 2];
          arg = true;
        } else {
          atomlog->SetLastErr(ERROR_CORE_FS,ERROR_LOST_QUOTES);
// Release allocated memory
          for (int i = 0; i < tempcount; i++)
            if (tempoutput[i] != 0)
              delete [] tempoutput[i];
          delete [] tempoutput;
          return 0;
        }
      } else {
        if ((input[i + 1] == symbol_space) || (input[i + 1] == symbol_tab)) {
// Quotes end...
          quote = false;
          tempoutput[args.count++][j] = '\0';
          j = 0;
          arg = false;
        } else {
          atomlog->SetLastErr(ERROR_CORE_FS,ERROR_LOST_QUOTES);
// Release allocated memory
          for (int i = 0; i < tempcount; i++)
            if (tempoutput[i] != 0)
              delete [] tempoutput[i];
          delete [] tempoutput;
          return 0;
        }
      }
    } else if (input[i] == symbol_doublequote) {
      if (doublequote == false) {
        if (i == 0) {
          if (i == len - 1) {
            atomlog->SetLastErr(ERROR_CORE_FS,ERROR_LOST_QUOTES);
// Release allocated memory
            for (int i = 0; i < tempcount; i++)
              if (tempoutput[i] != 0)
                delete [] tempoutput[i];
            delete [] tempoutput;
            return 0;
          }
          if ((input[i + 1] != symbol_space) && (input[i + 1] != symbol_tab)) {
// Quotes begin...
            doublequote = true;
            tempoutput[args.count] = new char[len - i + 2];
            arg = true;
          }
        } else if (((input[i - 1] == symbol_space) || (input[i - 1] == symbol_tab)) &&
            (input[i + 1] != symbol_space) && (input[i + 1] != symbol_tab)) {
// Quotes begin...
          doublequote = true;
          tempoutput[args.count] = new char[len - i + 2];
          arg = true;
        } else {
          atomlog->SetLastErr(ERROR_CORE_FS,ERROR_LOST_QUOTES);
// Release allocated memory
          for (int i = 0; i < tempcount; i++)
            if (tempoutput[i] != 0)
              delete [] tempoutput[i];
          delete [] tempoutput;
          return 0;
        }
      } else {
        if((input[i + 1] == symbol_space) || (input[i + 1] == symbol_tab)) {
// Quotes end...
          doublequote = false;
          tempoutput[args.count++][j] = '\0';
          j = 0;
          arg = false;
        } else {
          atomlog->SetLastErr(ERROR_CORE_FS,ERROR_LOST_QUOTES);
// Release allocated memory
          for (int i = 0; i < tempcount; i++)
            if (tempoutput[i] != 0)
              delete [] tempoutput[i];
          delete [] tempoutput;
          return 0;
        }
      }
    } else if (i == len - 1) {
      if (quote == true) {
        if (input[i] != symbol_quote) {
          atomlog->SetLastErr(ERROR_CORE_FS,ERROR_LOST_QUOTES);
// Release allocated memory
          for (int z = 0; z < tempcount; i++)
            if (tempoutput[z] != 0)
              delete [] tempoutput[z];
          delete [] tempoutput;
          return 0;
        }
      } else if (doublequote == true) {
        if (input[i] != symbol_doublequote) {
          atomlog->SetLastErr(ERROR_CORE_FS,ERROR_LOST_QUOTES);
// Release allocated memory
          for (int i = 0; i < tempcount; i++)
            if (tempoutput[i] != 0)
              delete [] tempoutput[i];
          delete [] tempoutput;
          return 0;
        }
      } else {
// first symbol ?
        if (tempoutput[args.count] == 0) {
          tempoutput[args.count] = new char[len - i + 2];
          j = 0;
        }
// it is just a symbol
        tempoutput[args.count][j++] = input[i];
      }
      tempoutput[args.count++][j] = '\0';
      j = 0;
      arg = false;
    } else {
// it is a symbol
      if ((j == 0) && (arg == false)) {
// this is a first symbol
        tempoutput[args.count] = new char[len - i + 2];
        arg = true;
      }
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
  ARGUMENTS *pargs = new ARGUMENTS;
  *pargs = args;
  return pargs;
}
TREE_FILE* FindFileFromPath(AtomLog *atomlog, TREE_FOLDER *root,
                            TREE_FOLDER *current, ARGUMENTS *path) {
  TREE_FOLDER *curfolder = 0, *tempfolder = 0;
  TREE_FILE *curfile = 0;
  bool found = false;
  if (root == 0) {
// if root not exist
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_FUNCTION_ARGUMENTS);
    snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s",
             "Root folder must exist!");
    atomlog->DebugMessage(atomlog->MsgBuf);
    for (int i = 0; i < path->count; i++)
      if (path->output[i] != 0)
        delete [] path->output[i];
    delete [] path->output;
    delete path;
    path = 0;
    return 0;
  }
  if (current == 0)
    curfolder = root;
  else
    curfolder = current;
  int k = 0;
  if ((path != 0) && (path->count != 0)) {
    if (strcmp(path->output[0], "/") == 0) {
// if it is root
      curfolder = root;
      k = 1;
    } else {
      for (int i = k; i < path->count; i++) {
        found = false;
        if (i == path->count - 1) {
// it must be the file
          if (curfolder->tree_file == 0) {
            atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_PATH);
              for (int i = 0; i < path->count; i++)
                if (path->output[i] != 0)
                  delete [] path->output[i];
              delete [] path->output;
              delete path;
              path = 0;
              return 0;
          } else {
            curfile = curfolder->tree_file;
            if (strcmp(curfile->name, path->output[i]) == 0) {
              found = true;
            }
            while ((curfile->tree_file != 0) && (found == false)) {
              curfile = curfile->tree_file;
              if (strcmp(curfile->name, path->output[i]) == 0) {
                found = true;
              }
            }
            if (found == false) {
              atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_PATH);
              for (int i = 0; i < path->count; i++)
                if (path->output[i] != 0)
                  delete [] path->output[i];
              delete [] path->output;
              delete path;
              path = 0;
              return 0;
            }
          }
        } else {
// it is a folder
          if (curfolder->tree_folder != 0) {
            tempfolder = curfolder->tree_folder;
            if (strcmp(tempfolder->name, path->output[i]) == 0) {
              found = true;
              curfolder = tempfolder;
            }
          } else {
            atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_PATH);
            for (int i = 0; i < path->count; i++)
              if (path->output[i] != 0)
                delete [] path->output[i];
            delete [] path->output;
            delete path;
            path = 0;
            return 0;
          }
          while ((tempfolder->next_folder != 0) && (found == false)) {
            tempfolder = tempfolder->next_folder;
            if (strcmp(tempfolder->name, path->output[i]) == 0) {
              found = true;
              curfolder = tempfolder;
            }
          }
          if (found == false) {
            atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_PATH);
            for (int i = 0; i < path->count; i++)
              if (path->output[i] != 0)
                delete [] path->output[i];
            delete [] path->output;
            delete path;
            path = 0;
            return 0;
          }
        }
      }
    }
  } else {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_PATH);
    if (path != 0)
      delete path;
    return 0;
  }
  if (path != 0) {
    for (int i = 0; i < path->count; i++)
      if (path->output[i] != 0)
        delete [] path->output[i];
    delete [] path->output;
    delete path;
    path = 0;
  }
  return curfile;
}
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
  key[0] = 0, key[1] = 0, key[2] = 0, key[3] = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = i * 4; j < (i + 1) * 4; j++) {
      key[i] += input[j];
      if (j < (i + 1) * 4 - 1)
        key[i] <<= 8;
    }
  }
  fprintf(stderr, "%s %x-%x-%x-%x\n", "Password is", key[0], key[1], key[2], key[3]);
  delete [] input;
  return key;
}
#ifdef _FSMAN_
#define RANDOM(min,max) (rand()%(max-min+1)+min)
// Create temp file name
char* tempname(AtomLog *atomlog) {
  unsigned char a = 10, b = 20; // diapasone
  unsigned char count = RANDOM(a,b);
  char *name = new char[count];
  for (unsigned char i = 0; i < count; i++)
    name[i] = RANDOM(0x61, 0x7a);
  return name;
}
#endif  // _FSMAN_
