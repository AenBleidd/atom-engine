#include "gamefs.h"
AtomFS::AtomFS(AtomLog *log, unsigned int *key) {
  if (log == 0) {
// What the ... ???
    throw -1;
  }
  atomlog = log;
  wake_key = 0;
// If class being created with predefined key
  if (key != 0) {
    wake_key = key;
    GenKey(wake_key[0], wake_key[1], wake_key[2], wake_key[3]);
  }
// create root directory
  root = new TREE_FOLDER;
  const unsigned char s = 2;
  char *name = new char[s];
  snprintf(name, s, "%s", "/");
  root->name = name;
#ifdef _FSMAN_
  root->flag |= ff_rw;
#else
  root->flag |= ff_ro;
#endif  // _FSMAN_
  root->tree_folder = 0;
  root->tree_file = 0;
  root->next_folder = 0;
  root->parent_folder = 0;
// Mounting...
#ifndef _FSMAN_
  if (Mount("mount") == -1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_MOUNT_FS);
    throw ERROR_MOUNT_FS;
  }
#endif  // _FSMAN_
}
AtomFS::~AtomFS() {
// delete root directory
  TREE_FOLDER *nextfolder = 0, *tempfolder = 0, *nearfolder = 0;
  TREE_FILE *nextfile = 0, *tempfile = 0, *temphead = 0, *tempcurrent = 0;
  nextfolder = root;
  while (true) {
// it may be the last element
    if ((nextfolder != 0) && (nextfolder->tree_folder == 0) &&
        (nextfolder->next_folder == 0) && (nextfolder->parent_folder == 0)) {
      nextfile = nextfolder->tree_file;
// let's delete all files. it may be easy
      while (nextfile != 0) {
        tempfile = nextfile;
        nextfile = tempfile->tree_file;
// ok, let's free some memory
// is the file is closed ?
        if (tempfile->file != 0) {
// Hm... it is stil opened. Let's close it! He-he.
          fclose(tempfile->file);
        }
// Save open file
        if (temphead == 0) {
          temphead = new TREE_FILE;
          temphead->tree_file = 0;
          tempcurrent = temphead;
          tempcurrent->id = tempfile->id;
          tempfile->id = 0;
        } else {
          tempcurrent = temphead;
          if (tempcurrent->id == tempfile->id) {
            tempfile->id = 0;
          } else {
            while (tempcurrent->tree_file != 0) {
              tempcurrent = tempcurrent->tree_file;
              if (tempcurrent->id == tempfile->id) {
                tempfile->id = 0;
                break;
              }
            }
          }
        }
// If we don't find it
        if (tempfile->id != 0) {
          tempcurrent->tree_file = new TREE_FILE;
          tempcurrent->id = tempfile->id;
          tempfile->id = 0;
          tempcurrent->tree_file = 0;
        }
// Delete filename
        delete [] tempfile->name;
// Delete file
        delete tempfile;
        tempfile = 0;
      }
// Delete folder
      delete [] nextfolder->name;
// deleting...
      delete nextfolder;
      nextfolder = 0;
      break;
    }
// check if this folder can be deleted
    if ((nextfolder != 0) && (nextfolder->tree_folder == 0) &&
         (nextfolder->next_folder == 0) && (nextfolder->parent_folder != 0)) {
      nextfile = nextfolder->tree_file;
// let's delete all files. it may be easy
      while (nextfile != 0) {
        tempfile = nextfile;
        nextfile = tempfile->tree_file;
// ok, let's free some memory
// is the file is closed ?
        if (tempfile->file != 0) {
// Hm... it is stil opened. Let's close it! He-he.
          fclose(tempfile->file);
        }
// Delete filename
        delete [] tempfile->name;
// Delete file
        delete tempfile;
        tempfile = 0;
      }
// Delete folder
      delete [] nextfolder->name;
// one level up
      tempfolder = nextfolder->parent_folder;
// I believe it will be zero
      tempfolder->tree_folder = 0;
// deleting...
      delete nextfolder;
      nextfolder = 0;
    }
// we will delete folders from end to begin
    while (nextfolder != 0) {
      tempfolder = nextfolder;
      nextfolder = nextfolder->tree_folder;
    }
    nearfolder = tempfolder->next_folder;
// find last folder in current folder
    while (nearfolder != 0) {
      tempfolder = nearfolder;
      nearfolder = nearfolder->next_folder;
    }
// this string must be very important
    nextfolder = tempfolder;
  }
  root = 0;
// Lets close all opened files!
  if (temphead->tree_file == 0) {
    if (temphead->id != 0) {
      fclose (temphead->id);
      temphead->id = 0;
    }
    delete temphead;
    temphead = 0;
  } else {
    while (temphead->tree_file != 0) {
      tempcurrent = temphead->tree_file;
      temphead = tempcurrent->tree_file;
      if (tempcurrent->id != 0) {
        fclose(tempcurrent->id);
        tempcurrent->id = 0;
      }
      delete tempcurrent;
      tempcurrent = 0;
    }
  }
}
