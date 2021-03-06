#include "gamefs.h"
AtomFS::AtomFS(AtomLog *log, unsigned int *key) {
  openalloc = 0;
  if (log == 0) {
// What the ... ???
    throw -1;
  }
  atomlog = log;
  ERROR_CORE_FS = atomlog->LoadStrings(ATOMERROR, gamefserrorcodes,
                                       gamefs_error_description);
  WARNING_CORE_FS = atomlog->LoadStrings(ATOMWARNING, gamefswarncodes,
                                       gamefs_warn_description);
  if (ERROR_CORE_FS == -1 || WARNING_CORE_FS == -1) {
// Why ???
    throw -1;
  }
// Check byteorder
  if (BYTEORDER == -1) {
    atomlog->SetLastErr(ERROR_CORE_FS, ERROR_WRONG_BYTEORDER);
    throw ERROR_WRONG_BYTEORDER;
  }
  wake_key = 0;
  wake_table = 0;
// If class being created with predefined key
  if (key != 0) {
    wake_key = key;
    wake_table = GenKey(wake_key[0], wake_key[1], wake_key[2], wake_key[3]);
  }
// set addon keys to zero...
  addon_key.addon_key = 0;
  addon_key.count = 0;
// create root directory
  root = new TREE_FOLDER;
  const uint8_t s = 2;
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
  TREE_FILE *nextfile = 0, *tempfile = 0;
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
// Free all memory and close all files
  OPENALLOC *tempalloc = 0;
  if (openalloc != 0) {
    do {
      if (tempalloc != 0)
        openalloc = tempalloc;
      tempalloc = 0;
      if (openalloc->file != 0)
        fclose(openalloc->file);
      tempalloc = openalloc->next;
      delete openalloc;
    } while (tempalloc != 0);
    openalloc = 0;
  }
// release memory used by keys
  if (addon_key.count != 0) {
    for (uint32_t i = 0; i < addon_key.count; i++) {
      delete [] addon_key.addon_key[i];
      delete [] addon_key.addon_table[i];
    }
    delete [] addon_key.addon_key;
    delete [] addon_key.addon_table;
    addon_key.addon_key = 0;
    addon_key.count = 0;
  }
// release memory used by main wake_table
// SIC! memory released by main wake_key MUST be released manually
  if (wake_table != 0)
    delete [] wake_table;
}
