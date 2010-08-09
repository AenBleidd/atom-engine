#include "navigate.h"
#include <string.h>
int AtomFS::Navigate(void) {
  bool flag = false;
  const unsigned int buffer_len = 1024;
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
    } else if ((strcmp(args->output[0], "cd") == 0) && (args->count == 2)){
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
        for (int i = 0; i < path->count; i++)
          if(path->output[i] != 0)
            delete [] path->output[i];
        delete [] path->output;
        delete path;
        path = 0;
      }
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
      for (int i = 0; i < args->count; i++)
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
\tls\t\tlist current directory's contents\n\
\thelp\t\tshow this help\n\
\texit\t\tquit navigation mode\n";

  atomlog->DebugMessage(help);
  fprintf(stderr, help);

  return;
}
int List(AtomLog *atomlog, TREE_FOLDER *curfolder, unsigned char mode) {
  char flag, permission, priority[3];
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
      snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%c%c%s %8i  B %s\n",
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
        snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%c%c%s %8i  B %s\n",
                  flag, permission, priority, 0, tempfolder->name);
    }
      atomlog->DebugMessage(atomlog->MsgBuf);
      fprintf(stderr, atomlog->MsgBuf);
    }
  }
// end with files
  if (tempfile != 0) {
    flag = 'f';
    unsigned char i = 0;
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
      snprintf((char*)atomlog->MsgBuf, MSG_BUFFER_SIZE, "%c%c%s %8.f %s %s\n",
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
                "%c%c%s %8.f %s %s\n", flag, permission, priority, size,
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
  int i = 0;
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
