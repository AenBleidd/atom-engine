#include "navigate.h"
#include <string.h>
int32_t AtomFS::Navigate(void) {
  bool flag = false;
  const uint32_t buffer_len = 1024;
  char *buffer = 0;
// configure FS
  TREE_FOLDER *curfolder = root;
  while (flag != true) {
    fprintf(stderr, "%s", "atomfs-> ");
    buffer = new char[buffer_len];
// get the command
    fgets(buffer, buffer_len, stdin);
// parse it
    ARGUMENTS *args = ParseArgs(atomlog, buffer);
// look the commands
    if ((args == 0) || (args->count == 0)) {
      atomlog->SetLastWrn(WARNING_CORE_FS, WARNING_WRONG_COMMAND);
      snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s\n",
               "Wrong syntax or unknown command");
      atomlog->DebugMessage(atomlog->MsgBuf);
      fprintf(stderr, atomlog->MsgBuf);
      ShowHelp(atomlog);
    } else if (strcmp(args->output[0], "help") == 0) {
      ShowHelp(atomlog);
    } else if ((strcmp(args->output[0], "exit") == 0) && (args->count == 1)) {
      flag = true;
    } else if ((strcmp(args->output[0], "ls") == 0) && (args->count == 1)) {
      List(atomlog, curfolder, 0);
    } else if ((strcmp(args->output[0], "dir") == 0) && (args->count == 1)) {
      List(atomlog, curfolder, 1);
    } else if ((strcmp(args->output[0], "cd") == 0) && (args->count == 2)) {
      ARGUMENTS *path = ParsePath(atomlog, args->output[1]);
      if (path == 0) {
        atomlog->SetLastWrn(WARNING_CORE_FS, WARNING_WRONG_COMMAND);
        snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s\n",
                 "Wrong syntax or unknown command");
        atomlog->DebugMessage(atomlog->MsgBuf);
        fprintf(stderr, atomlog->MsgBuf);
        ShowHelp(atomlog);
      } else {
        curfolder = ChDir(atomlog, root, curfolder, path);
        for (int32_t i = 0; i < path->count; i++)
          if(path->output[i] != 0)
            delete [] path->output[i];
        delete [] path->output;
        delete path;
        path = 0;
      }
    } else if ((strcmp(args->output[0], "cp") == 0) && (args->count == 3)) {
      Copy(atomlog, this, curfolder, args->output[1], args->output[2]);
    } else {
      atomlog->SetLastWrn(WARNING_CORE_FS, WARNING_WRONG_COMMAND);
      snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s\n",
               "Wrong syntax or unknown command");
      atomlog->DebugMessage(atomlog->MsgBuf);
      fprintf(stderr, atomlog->MsgBuf);
      ShowHelp(atomlog);
    }
// Cleaning
    if (args != 0) {
      for (int32_t i = 0; i < args->count; i++)
        if (args->output[i] != 0)
          delete [] args->output[i];
      delete [] args->output;
      delete args;
      args = 0;
    }
    delete [] buffer;
    buffer = 0;
  }
  return 0;
}

void ShowHelp(AtomLog *atomlog) {
  char help[] = "Available commands:\n\
\tcd [name]\t\tmove to the 'name' directory\n\
\tcp [name1] [name2]\tcopy file 'name1' to 'name2' to disc\n\
\tdir\t\t\tlist current directory's contents more verbose\n\
\texit\t\t\tquit navigation mode\n\
\thelp\t\t\tshow this help\n\
\tls\t\t\tlist current directory's contents\n";

  atomlog->DebugMessage(help);
  fprintf(stderr, help);

  return;
}
int32_t List(AtomLog *atomlog, TREE_FOLDER *curfolder, uint8_t mode) {
  int8_t flag, permission;
  char priority[3];
  double size = 0;
  const char *sizeformat[] = {" B", "KB", "MB", "GB", "TB", "PB"};
  TREE_FOLDER *tempfolder = curfolder;
  TREE_FILE *tempfile = curfolder->tree_file;
// begin with folders
  if (tempfolder->tree_folder != 0) {
    tempfolder = tempfolder->tree_folder;
    snprintf(priority, 3, "%s", "--");
    flag = 'd';
    if (mode == 0) {
      snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s\n",
               tempfolder->name);
    } else if (mode == 1) {
      if (tempfolder->flag == ff_ro)
        permission = 'r';
      else
        permission = 'w';
      snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%d%d%s %8d  B %s\n",
                flag, permission, priority, 0, tempfolder->name);
    }
    atomlog->DebugMessage(atomlog->MsgBuf);
    fprintf(stderr, atomlog->MsgBuf);
    while (tempfolder->next_folder != 0) {
      tempfolder = tempfolder->next_folder;
      if (mode == 0) {
        snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s\n",
                 tempfolder->name);
      } else if (mode == 1) {
        if (tempfolder->flag == ff_ro)
          permission = 'r';
        else
          permission = 'w';
        snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE,
                  "%d%d%s %8d  B %s\n",
                  flag, permission, priority, 0, tempfolder->name);
    }
      atomlog->DebugMessage(atomlog->MsgBuf);
      fprintf(stderr, atomlog->MsgBuf);
    }
  }
// end with files
  if (tempfile != 0) {
    flag = 'f';
    uint8_t i = 0;
    if (mode == 0) {
      snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s\n",
                tempfile->name);
    } else if (mode == 1) {
      if (tempfolder->flag == ff_ro)
        permission = 'r';
      else
        permission = 'w';
      snprintf(priority, 3, "%02x", tempfile->priority);
      size = tempfile->size;
      i = 0;
      while (size > 1023) {
        size /= 1024;
        i++;
      }
      snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE,
               "%d%d%s %8.f %s %s\n",
               flag, permission, priority, size, sizeformat[i],
               tempfile->name);
    }
    atomlog->DebugMessage(atomlog->MsgBuf);
    fprintf(stderr, atomlog->MsgBuf);
    while (tempfile->tree_file != 0) {
      tempfile = tempfile->tree_file;
      if (mode == 0) {
        snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%s\n",
                 tempfile->name);
      } else if (mode == 1) {
        if (tempfolder->flag == ff_ro)
          permission = 'r';
        else
          permission = 'w';
        snprintf(priority, 3, "%02x", tempfile->priority);
        size = tempfile->size;
        i = 0;
        while (size > 1023) {
          size /= 1024;
          i++;
        }
        snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE,
                "%d%d%s %8.f %s %s\n", flag, permission, priority, size,
                sizeformat[i], tempfile->name);
      }
      atomlog->DebugMessage(atomlog->MsgBuf);
      fprintf(stderr, atomlog->MsgBuf);
    }
  }
  return 0;
}
TREE_FOLDER* ChDir(AtomLog *atomlog, TREE_FOLDER *root,
                   TREE_FOLDER *curfolder, ARGUMENTS *path) {
  TREE_FOLDER *cur = curfolder, *temp = curfolder;
  int32_t i = 0;
  bool found = false;
// check for the root
  if (strcmp(path->output[0], "/") == 0) {
    cur = root;
    i = 1;
  }
  for (i; i < path->count; i++) {
    if (strcmp(path->output[i], "..") == 0) {
      if (cur->parent_folder != 0) {
        cur = cur->parent_folder;
        i = 1;
      } else {
// if this is root
        atomlog->SetLastWrn(WARNING_CORE_FS, WARNING_INCORRECT_PATH);
        return curfolder;
      }
    } else {
      found = false;
      if (cur->tree_folder != 0) {
        temp = cur->tree_folder;
        if (strcmp(temp->name, path->output[i]) == 0) {
          cur = temp;
          found = true;
        }
      } else {
        atomlog->SetLastWrn(WARNING_CORE_FS, WARNING_INCORRECT_PATH);
        return curfolder;
      }
      while ((temp->next_folder != 0) && (found == false)) {
        temp = temp->next_folder;
        if (strcmp(temp->name, path->output[i]) == 0) {
          cur = temp;
          found = true;
        }
      }
      if (found == false) {
        atomlog->SetLastWrn(WARNING_CORE_FS, WARNING_INCORRECT_PATH);
        return curfolder;
      }
    }
  }
  return cur;
}
int32_t Copy(AtomLog *atomlog, AtomFS *atomfs, TREE_FOLDER *curfolder, char *in,
         char *out) {
// Open the file
  FILE *src = atomfs->Open(in, curfolder);
  if (src == 0) {
    atomlog->DebugMessage("Source file can not be opened");
    return -1;
  }
// Save the file
  if (atomfs->Save(src, out) != 0) {
    atomlog->DebugMessage( "File can not be written");
    atomfs->Close(src);
    return -1;
  }
// Close the file
  atomfs->Close(src);
  return 0;
}
