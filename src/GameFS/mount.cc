#include "gamefs.h"
#include "strings.h"
int AtomFS::Mount(char* filename) {
  atomlog->DebugMessage("Begin mounting filesystem...");
  atomlog->DebugMessage("Reading mount file...");
// open configuration file for reading
  FILE *conf = fopen(filename, "rb");
// error while opening the file
  if (conf == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
    return -1;
  }
 long size = 0;
// get file size
  if (fseek(conf, 0, SEEK_END) != 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  size = ftell(conf);
  if (size == -1L) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  fclose(conf);
  conf = fopen(filename, "r");
// error while opening the file
  if (conf == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
    return -1;
  }
// reading the file
  char *buf = new char[size];
  ARGUMENTS *args;
  char *file = 0, *mountpoint = 0;
  while(fgets(buf, size, conf)) {
    args = ParseArgs(atomlog, buf);
    if (args == 0) {
      atomlog->SetLastErr(ERROR_CORE_FS,ERROR_PARSE_MOUNT_FILE);
      delete [] buf;
      return -1;
    }
// Lets look the arguments
    for (int i = 0; i < args->count; i++) {
      if (args->output[i][0] == '#') {
// it is a comment
        break;
      } else {
        if (file == 0) {
// it is a file... maybe...
          file = args->output[i];
        } else if (mountpoint == 0) {
// it is a mountpoint
          mountpoint = args->output[i];
          if (Mount(file, mountpoint) != 0) {
            atomlog->SetLastErr(ERROR_CORE_FS, ERROR_MOUNT_FS);
            snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "Error mount %s",
                     file);
            atomlog->LogMessage(atomlog->MsgBuf);
// Release memory
            for (int j = 0; j < args->count; j++)
              if (args->output[j] != 0)
                delete [] args->output[j];
            delete [] args->output;
            delete [] buf;
            return -1;
          }
        } else {
// some garbage
          atomlog->SetLastErr(ERROR_CORE_FS, ERROR_PARSE_MOUNT_FILE);
// Release memory
          for (int j = 0; j < args->count; j++)
            if (args->output[j] != 0)
              delete [] args->output[j];
          delete [] args->output;
          delete [] buf;
          return -1;
        }
      }
    }
// Release memory
    for (int i = 0; i < args->count; i++)
      if (args->output[i] != 0)
        delete [] args->output[i];
    delete [] args->output;
    args->count = 0;
    delete args;
    args = 0;
// Set flags to zero
    file = 0;
    mountpoint = 0;
  }
// closing file
  fclose(conf);
// cleaning
  delete [] buf;
  return 0;
}
// Mount single file
int AtomFS::Mount(char* filename, char* mountfolder) {
  OPENALLOC *tempalloc = 0, *prev = 0;
  if (openalloc != 0) {
    tempalloc = openalloc;
    while(tempalloc->next != 0) {
      tempalloc = tempalloc->next;
    }
    prev = tempalloc;
  }
  tempalloc = new OPENALLOC;
  tempalloc->file = 0;
  tempalloc->memory = 0;
  tempalloc->next = 0;
  if (openalloc == 0)
    openalloc = tempalloc;
  if (prev != 0)
    prev->next = tempalloc;
// pointer to wake_table
  unsigned int *table;
// Ok, let's do it!
// Check mount point
  if (mountfolder == 0) {
// we will mount to root directory
  mountfolder = "/\0";
  }
// Print some info...
  const unsigned short s = strlen(filename) + strlen(mountfolder) + 15;
  char *pbuf = new char[s];
  snprintf(pbuf, s, "Mounting %s to %s",
           filename, mountfolder);
  atomlog->DebugMessage(pbuf);
  delete [] pbuf;
// create name of the file
  unsigned short int namelen = strlen(filename) + 5;
  char *binfile = new char[namelen];
  char *datfile = new char[namelen];
  snprintf(binfile, s, "%s.bin", filename);
  snprintf(datfile, s, "%s.dat", filename);
// Try to open
  FILE *dat, *bin;
  dat = fopen(datfile, "rb");
  if (dat == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
    delete [] binfile;
    delete [] datfile;
    return -1;
  }
  bin = fopen(binfile, "rb");
  if (bin == 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
    delete [] binfile;
    delete [] datfile;
    fclose(dat);
    return -1;
  }
  tempalloc->file = bin;
// Cleaning...
  delete [] binfile;
  delete [] datfile;
// check filesize
  if (fseek(dat, 0, SEEK_END) != 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
  unsigned long int datfilesize = ftell(dat) - 1;
  if (datfilesize == -1L) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
  rewind(dat);
// Lets get start info about opened files
  HEADER header;
// Get magic signature
  if (fread(&header.magic, sizeof(header.magic), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
// Check this magic
  if (header.magic != magic) {
// Oops, we opened smth wrong...
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
// Magic!!!! *YAHOO*
// Get version
  if (fread(&header.version, sizeof(header.version), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
// Check version
  if (header.version != version) {
// Oops, version is wrong...
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
// Get archive type
  if (fread(&header.type, sizeof(header.type), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
// Check this type
  if ((header.type != type_critical) && (header.type != type_standart) &&
       (header.type != type_addon)) {
// Hmm... Strange type...
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
// Get crypt size
  if (fread(&header.crypt, sizeof(header.crypt), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }  // Nothing to check
// Get binary files count
// TODO(Lawliet): Add support for various count of bin files
  if (fread(&header.bincount, sizeof(header.bincount), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
// Get binary size
// TODO(Lawliet): Add support for checking the size various count of bin files
  if (fread(&header.binsize, sizeof(header.binsize), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
// Get data files count
// TODO(Lawliet): Add support for various count of dat files
  if (fread(&header.datcount, sizeof(header.datcount), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
// Get data size
// TODO(Lawliet): Add support for checking the size various count of dat files
  if (fread(&header.datsize, sizeof(header.datsize), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    return -1;
  }
// Check for key
  if (header.type == type_addon) {
// Ok, let's get the key...
    if (fread(&header.addon_key, sizeof(header.addon_key), 1, dat) != 1) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
      fclose(dat);
      fclose(bin);
      return -1;
    }
// Check wake_table
/*    if ((header.addon_key[0] == wake_key[0]) &&
        (header.addon_key[1] == wake_key[1]) &&
        (header.addon_key[2] == wake_key[2]) &&
        (header.addon_key[3] == wake_key[3])) {
      table = wake_table;
    } else {*/
      table = GenKey(header.addon_key[0], header.addon_key[1],
                     header.addon_key[2], header.addon_key[3]);
      tempalloc->memory = table;
    /*}*/
  }
// Header was parsed. Easiest part is done...

// Let's look the mountpont
// Slash flag
  bool slash = true;
// current pointer position within the string
// we begin the cycle from first element couse zero-element must be root
  unsigned int pos = 1;
// another counter
  unsigned int i = 0;
// Kyrie!
// Parse mountpoint
  ARGUMENTS *args = ParsePath(atomlog, mountfolder);
  if (args->count < 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_MOUNTPOINT);
    fclose(dat);
    fclose(bin);
    for (int i = 0; i < args->count; i++)
      if (args->output[i] != 0)
        delete [] args->output[i];
    delete [] args->output;
    return -1;
  }
// Check for the root directory
  if (strcmp(args->output[0], "/") != 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_MOUNTPOINT);
    fclose(dat);
    fclose(bin);
    for (int i = 0; i < args->count; i++)
      if (args->output[i] != 0)
        delete [] args->output[i];
    delete [] args->output;
    return -1;
  } else {
    delete [] args->output[0];
  }
  TREE_FOLDER *current = root;
  for (pos; pos < args->count; pos++) {
// ok, we have smth here...
// let's find this path
    if (current->tree_folder == 0) {
// Hm, the folder doesn't have any subfolder. Let's create it!
      current->tree_folder = new TREE_FOLDER;
      current->tree_folder->parent_folder = current;
      current = current->tree_folder;
#ifdef _FSMAN_
      current->flag |= ff_rw;
#else
      current->flag |= ff_ro;
#endif  // _FSMAN_
      current->tree_folder = 0;
      current->tree_file = 0;
      current->next_folder = 0;
// set he name of the folder
      unsigned int len = strlen(args->output[pos]);
      current->name = new char[len];
      snprintf(current->name, len, "%s", args->output[pos]);
    } else if (strcmp(current->tree_folder->name, args->output[pos]) == 0) {
// this is a folder we search for
      current = current->tree_folder;
    } else {
// my be we can find smth ?
      current = current->tree_folder;
      while (true) {
        if (current->next_folder == 0) {
// we have nothing
// we can create it here
          current->next_folder = new TREE_FOLDER;
          current->next_folder->parent_folder = current->parent_folder;
          current = current->next_folder;
#ifdef _FSMAN_
          current->flag |= ff_rw;
#else
          current->flag |= ff_ro;
#endif  // _FSMAN_
          current->tree_folder = 0;
          current->tree_file = 0;
          current->next_folder = 0;
// set he name of the folder
          unsigned int len = strlen(args->output[pos]);
          current->name = new char[len];
          snprintf(current->name, len, "%s", args->output[pos]);
// nothing to do anymore
          break;
        } else if (strcmp(current->name, args->output[pos]) == 0) {
// We find it!
          break;
        } else {
// may be next is the folder we search for ?
          current = current->next_folder;
        }
      }
    }
// Clean
    if (args->output[pos] != 0)
      delete [] args->output[pos];
  }
// In theory parsing the mountpoint is done...
// Some clean
  delete [] args->output;
  args->output = 0;
  delete args;
  args = 0;
// Mounting...
// First must be root directory
RECORD *temprecord;
temprecord = new RECORD;
// Get the flag
  if (fread(&temprecord->flag, sizeof(temprecord->flag), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    delete temprecord;
    return -1;
  }
// Check it
  if (temprecord->flag != flag_folder) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_FILE);
    fclose(dat);
    fclose(bin);
    delete temprecord;
    return -1;
  }
// Get namelen
  if (fread(&temprecord->namelen, sizeof(temprecord->namelen), 1, dat) != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    delete temprecord;
    return -1;
  }
// Check namelen. It must be equal 1
  if (temprecord->namelen != 1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_FILE);
    fclose(dat);
    fclose(bin);
    delete temprecord;
    return -1;
  }
// Get the name.
  temprecord->name = new char[temprecord->namelen + 1];
  if (fread(temprecord->name, 1, temprecord->namelen, dat) !=
      temprecord->namelen) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(dat);
    fclose(bin);
    delete [] temprecord->name;
    delete temprecord;
    return -1;
  }
  temprecord->name[temprecord->namelen] = '\0';
// Check the name. It must be root
  if (strcmp(temprecord->name, "/") != 0) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_FILE);
    fclose(dat);
    fclose(bin);
    delete [] temprecord->name;
    delete temprecord;
    return -1;
  }
// Clean
  delete [] temprecord->name;
  delete temprecord;
// flag for elements that we fing somewhere
  bool bfound = false;
  TREE_FILE *tempfile = 0;
// At least we will mount smth!
  while (true) {
// check for file end
    if (datfilesize == ftell(dat) - 1) {
      break;
    }
    temprecord = new RECORD;
// Get the flag
    if (fread(&temprecord->flag, sizeof(temprecord->flag), 1, dat) != 1) {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
      fclose(dat);
      fclose(bin);
      delete temprecord;
      return -1;
    }
// Check the flag
    if (temprecord->flag == flag_file) {
// Get namelen
      if (fread(&temprecord->namelen, sizeof(temprecord->namelen), 1, dat) !=
          1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
        fclose(dat);
        fclose(bin);
        delete temprecord;
        return -1;
      }
// Get the name.
      temprecord->name = new char[temprecord->namelen + 1];
      if (fread(temprecord->name, 1, temprecord->namelen, dat) !=
          temprecord->namelen) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
        fclose(dat);
        fclose(bin);
        delete [] temprecord->name;
        delete temprecord;
        return -1;
      }
      temprecord->name[temprecord->namelen] = '\0';
// Get the size
      if (fread(&temprecord->size, sizeof(temprecord->size), 1, dat) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
        fclose(dat);
        fclose(bin);
        delete [] temprecord->name;
        delete temprecord;
        return -1;
      }
// Get the offset
        if (fread(&temprecord->offset, sizeof(temprecord->offset), 1, dat) !=
            1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
        fclose(dat);
        fclose(bin);
        delete [] temprecord->name;
        delete temprecord;
        return -1;
      }
// Get crc
        if (fread(&temprecord->crc, sizeof(temprecord->crc), 1, dat) != 1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
        fclose(dat);
        fclose(bin);
        delete [] temprecord->name;
        delete temprecord;
        return -1;
      }
// Search fo the file
      bfound = false;
      if (current->tree_file == 0) {
        current->tree_file = new TREE_FILE;
        current->tree_file->parent_folder = current;
        current->tree_file->tree_file = 0;
        current->tree_file->name = temprecord->name;
        current->tree_file->size = temprecord->size;
        current->tree_file->offset = temprecord->offset;
        current->tree_file->flag = root->flag;
        current->tree_file->bytescrypt = header.crypt;
#ifdef _CRC_CHECK_
        current->tree_file->crc = temprecord->crc;
#endif  // _CRC_CHECK_
        current->tree_file->id = bin;
        current->tree_file->priority = header.type;
        current->tree_file->file = 0;
        current->tree_file->descriptor = 0;
        current->tree_file->key = 0;
        if (header.type == type_addon) {
          current->tree_file->key = header.addon_key;
        }
        current->tree_file->table = table;
// Clean
        delete temprecord;
        temprecord = 0;
      } else {
        if (strcmp(current->tree_file->name, temprecord->name) == 0) {
          snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE,
                   "Overwtiting the file %s", temprecord->name);
          atomlog->LogMessage(atomlog->MsgBuf);
          if ((tempfile->priority == type_critical) ||
              (header.type < tempfile->priority)) {
            atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OVERWRITE_DENIED);
            fclose(bin);
            fclose(dat);
            delete [] temprecord->name;
            delete temprecord;
            return -1;
          } else {
            atomlog->SetLastWrn(WARNING_CORE_FS, WARNING_OVERWRITE)
            tempfile->name = temprecord->name;
            tempfile->size = temprecord->size;
            tempfile->offset = temprecord->offset;
            tempfile->flag = root->flag;
            tempfile->bytescrypt = header.crypt;
#ifdef _CRC_CHECK_
            tempfile->crc = temprecord->crc;
#endif  // _CRC_CHECK_
            tempfile->id = bin;
            tempfile->priority = header.type;
            tempfile->file = 0;
            tempfile->tree_file->descriptor = 0;
            tempfile->tree_file->key = 0;
            if (header.type == type_addon) {
              tempfile->tree_file->key = header.addon_key;
            }
            tempfile->tree_file->table = table;
// Clean
            delete temprecord;
            temprecord = 0;
          }
          bfound = true;
        } else {
          tempfile = current->tree_file;
          while (tempfile->tree_file != 0) {
            if (strcmp(tempfile->name, temprecord->name) == 0) {
              snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE,
                       "Overwtiting the file %s", temprecord->name);
              atomlog->LogMessage(atomlog->MsgBuf);
              if ((tempfile->priority == type_critical) ||
                  (header.type < tempfile->priority)) {
                atomlog->SetLastErr(ERROR_CORE_FS, ERROR_OVERWRITE_DENIED);
                fclose(bin);
                fclose(dat);
                delete [] temprecord->name;
                delete temprecord;
                return -1;
              } else {
                atomlog->SetLastWrn(WARNING_CORE_FS, WARNING_OVERWRITE)
                tempfile->name = temprecord->name;
                tempfile->size = temprecord->size;
                tempfile->offset = temprecord->offset;
                tempfile->flag = root->flag;
                tempfile->bytescrypt = header.crypt;
#ifdef _CRC_CHECK_
                tempfile->crc = temprecord->crc;
#endif  // _CRC_CHECK_
                tempfile->id = bin;
                tempfile->priority = header.type;
                tempfile->file = 0;
                tempfile->tree_file->descriptor = 0;
                tempfile->tree_file->key = 0;
                if (header.type == type_addon) {
                  tempfile->tree_file->key = header.addon_key;
                }
                tempfile->tree_file->table = table;
// Clean
                delete temprecord;
                temprecord = 0;
              }
              bfound = true;
              break;
            } else {
              tempfile = tempfile->tree_file;
            }
          }
        }
        if (bfound == false) {
          tempfile->tree_file = new TREE_FILE;
          tempfile->tree_file->parent_folder = current;
          tempfile->tree_file->tree_file = 0;
          tempfile->tree_file->name = temprecord->name;
          tempfile->tree_file->size = temprecord->size;
          tempfile->tree_file->offset = temprecord->offset;
          tempfile->tree_file->flag = root->flag;
          tempfile->tree_file->bytescrypt = header.crypt;
#ifdef _CRC_CHECK_
          tempfile->tree_file->crc = temprecord->crc;
#endif  // _CRC_CHECK_
          tempfile->tree_file->id = bin;
          tempfile->tree_file->priority = header.type;
          tempfile->tree_file->file = 0;
          tempfile->tree_file->descriptor = 0;
          tempfile->tree_file->key = 0;
          if (header.type == type_addon) {
            tempfile->tree_file->key = header.addon_key;
          }
          tempfile->tree_file->table = table;
// Clean
          delete temprecord;
          temprecord = 0;
        }
      }
    }
    else if (temprecord->flag == flag_folder) {
// Get namelen
      if (fread(&temprecord->namelen, sizeof(temprecord->namelen), 1, dat) !=
          1) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
        fclose(dat);
        fclose(bin);
        delete temprecord;
        return -1;
      }
// Get the name.
      temprecord->name = new char[temprecord->namelen + 1];
      if (fread(temprecord->name, 1, temprecord->namelen, dat) !=
          temprecord->namelen) {
        atomlog->SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
        fclose(dat);
        fclose(bin);
        delete [] temprecord->name;
        delete temprecord;
        return -1;
      }
      temprecord->name[temprecord->namelen] = '\0';
// Search for this folder
      bfound = false;
      if (current->tree_folder == 0) {
// Set info to file system
      current->tree_folder = new TREE_FOLDER;
      current->tree_folder->parent_folder = current;
      current = current->tree_folder;
      current->name = temprecord->name;
      current->flag = root->flag;
      current->tree_folder = 0;
      current->tree_file = 0;
      current->next_folder = 0;
      delete temprecord;
      temprecord = 0;
      } else {
        if (strcmp(current->tree_folder->name, temprecord->name) == 0) {
// we found it
          bfound = true;
// Clean
          delete [] temprecord->name;
          delete temprecord;
          temprecord = 0;
          current = current->tree_folder;
        } else {
          current = current->tree_folder;
          while (current->next_folder != 0) {
            if (strcmp(current->next_folder->name, temprecord->name) == 0) {
// we found it
              bfound = true;
// Clean
              delete [] temprecord->name;
              delete temprecord;
              temprecord = 0;
              current = current->next_folder;
              break;
            }
            current = current->next_folder;
          }
        }
        if (bfound == false) {
// Set info to file system
          current->next_folder = new TREE_FOLDER;
          current->next_folder->parent_folder = current->parent_folder;
          current = current->next_folder;
          current->name = temprecord->name;
          current->flag = root->flag;
          current->tree_folder = 0;
          current->tree_file = 0;
          current->next_folder = 0;
          delete temprecord;
          temprecord = 0;
        }
      }
    }
    else if (temprecord->flag == flag_eoc) {
// this is the end of folder
// we will move to the one level up
// TODO(Lawliet): Add check for the superfluous of the flag
      if (current->parent_folder != 0) {
        current = current->parent_folder;
      }
      delete temprecord;
      temprecord = 0;
    } else {
      atomlog->SetLastErr(ERROR_CORE_FS, ERROR_INCORRECT_FILE);
      fclose(dat);
      fclose(bin);
      delete temprecord;
      return -1;
    }
  }
// Close data file
  fclose(dat);
  snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE,
           "%s was successfully mounted", filename);
  atomlog->DebugMessage(atomlog->MsgBuf);
  return 0;
}
