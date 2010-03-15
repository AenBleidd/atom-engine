#include "gamefs.h"

AtomFS::AtomFS() {
// Generate crypt key
  wake_key[0] = 0xCEF2F7E0;
  wake_key[1] = 0xFFEDE8E5;
  wake_key[2] = 0x20C2E0EC;
  wake_key[3] = 0xEFE8F0E0;
  GenKey(wake_key[0], wake_key[1], wake_key[2], wake_key[3]);
// create root directory
  root = new TREE_FOLDER;
  char name[] = "/\0";
  root->name = name;
#ifdef _FSMAN_
  root->flag |= ff_rw;
#else
  root->flag |= ff_ro;
#endif //_FSMAN_
  root->tree_folder = 0;
  root->tree_file = 0;
// Mounting...
#ifndef _FSMAN_
  if (Mount("mount") == -1) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_MOUNT_FS);
    throw ERROR_MOUNT_FS;
  }
#endif // _FSMAN_
}
AtomFS::~AtomFS() {
// delete root directory
  delete root;
}
// Table generation
void AtomFS::GenKey(unsigned int k0, unsigned int k1, unsigned int k2, unsigned int k3)
{
  long x, z;
  int p ;
  static long tt[10]= {
                       0x726a8f3bL, // table
                       0xe69a3b5cL,
                       0xd3c71fe5L,
                       0xab3c73d2L,
                       0x4d3a8eb3L,
                       0x0396d6e8L,
                       0x3d4c2f7aL,
                       0x9ee27cf3L, } ;
  wake_table[0] = k0;
  wake_table[1] = k1;
  wake_table[2] = k2;
  wake_table[3] = k3;
  for (p=4 ; p<256 ; p++)
  {
    x=wake_table[p-4]+wake_table[p-1] ;  // fill t
    wake_table[p]= (x>>3) ^ tt[(unsigned char)(x&7)] ;
  }
  for (p=0 ; p<23 ; p++)
    wake_table[p]+=wake_table[p+89] ; 		  // mix first entries
  x=wake_table[33] ; z=wake_table[59] | 0x01000001L ;
  z=z&0xff7fffffL ;
  for (p=0 ; p<256 ; p++) {//change top byte to
    x=(x&0xff7fffffL)+z ;  // a permutation etc
    wake_table[p]=(wake_table[p] & 0x00ffffffL) ^ x ;
  }
  wake_table[256]=wake_table[0] ;
  unsigned char y=(unsigned char)(x);
  for (p=0 ; p<256 ; p++) { // further change perm.
    wake_table[p]=wake_table[y=(unsigned char)(wake_table[p^y]^y)];  // and other digits
    wake_table[y]=wake_table[p+1] ;
  }
}
#ifdef _FSMAN_
void AtomFS::Crypt(unsigned int *data, int lenght, unsigned int k[4], unsigned int r[4], unsigned int *t)
{
  int d;
  unsigned int r1,r2,r3,r4,r5,r6, *e,m=0x00ffffff ;
  r3=k[0] ; r4=k[1] ; r5=k[2] ; r6=k[3] ;
  if (lenght<0) d= -1 ; else d=1 ;
  e=data+lenght ;
  while (data-e) {
    r1 = *data ;
    r2 = r1^r6 ;
    *data = r2 ; // Change into r1 for decoding.
    data += d ;
    r3 = r3+r2 ;
    r3 = (r3>>8&m)^wake_table[r3&255] ;
    r4 = r4+r3 ;
    r4 = (r4>>8&m)^wake_table[r4&255] ;
    r5 = r5+r4 ;
    r5 = (r5>>8&m)^wake_table[r5&255] ;
    r6 = r6+r5 ;
    r6 = (r6>>8&m)^wake_table[r6&255] ;
  }
  r[0] =r3 ; r[1]=r4 ; r[2]=r5 ; r[3]=r6 ;
}
#endif // _FSMAN_
void AtomFS::Decrypt (unsigned int *data, int lenght, unsigned int k[4], unsigned int r[4], unsigned int *t)
{
  int d;
  unsigned int r1,r2,r3,r4,r5,r6, *e,m=0x00ffffff ;
  r3=k[0] ; r4=k[1] ; r5=k[2] ; r6=k[3] ;
  if (lenght<0) d= -1 ; else d=1 ;
  e=data+lenght ;
  while (data-e) {
    r1 = *data ;
    r2 = r1^r6 ;
    *data = r2 ;
    data += d ;
    r3 = r3+r1 ;
    r3 = (r3>>8&m)^wake_table[r3&255] ;
    r4 = r4+r3 ;
    r4 = (r4>>8&m)^wake_table[r4&255] ;
    r5 = r5+r4 ;
    r5 = (r5>>8&m)^wake_table[r5&255] ;
    r6 = r6+r5 ;
    r6 = (r6>>8&m)^wake_table[r6&255] ;
  }
    r[0] =r3 ; r[1]=r4 ; r[2]=r5 ; r[3]=r6 ;
}
int AtomFS::Mount (char* filename) {
  atomlog.DebugMessage("Begin mounting filesystem...");
  atomlog.DebugMessage("Reading mount file...");
// open configuration file for reading
  FILE *conf = fopen(filename, "rb");
// error while opening the file
  if (conf == 0) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
    return -1;
  }
  long size = 0;
// get file size
  if (fseek(conf, 0, SEEK_END) != 0) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  size = ftell(conf);
  if (size == -1L) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    return -1;
  }
  rewind(conf);
// reading the file
  char *buf = new char [size];
  if (fread(buf, 1, size, conf) != size) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    delete [] buf;
    return -1;
  }
// close configuration file
  fclose(conf);
  atomlog.DebugMessage("Mount file was successfully readed.");
// parsing...
  long pos = 0;
  unsigned int tsize = 0;
// flags
  bool end = false;
  bool fileflag = false;
  bool folderflag = false;
  bool priorflag = false;
  bool quotes = false;
// reading configuration
  while (end != true) {
// this is end
    if (pos == size || pos > size) {
      end = true;
      break;
    }
// if it is space
    else if ((buf[pos] == 0x20 || buf[pos] == 0x0A) && pos < size) pos++;
// if it is a comment
    if (buf[pos] == 0x23) {
      while (buf[pos] != 0x0A) pos++;
// last comment
      if (pos == size || pos == (size-1)) {
        end = true;
        break;
      }
      else pos++;
    }
// if it is quotes
    if (buf[pos] == 0x22) {
        tsize = 0;
        pos++;
        while (buf[pos] != 0x22 && pos <= size) {
          pos++;
          tsize++;
        }
        quotes = true;
        if (pos == size && buf[pos] != 0x22) {
          atomlog.SetLastErr(ERROR_CORE_FS, ERROR_PARSE_MOUNT_FILE_QUOTES);
          delete [] buf;
          return -1;
        }
      }
// yeah! it's some symbol here!
      else {
        tsize = 0;
        while (buf[pos] != 0x20 && buf[pos] != 0x0A) {
          pos++;
          tsize++;
        }
      }
// rewind to read the string
      pos -= tsize;
      char *file;
      char *mountpoint;
      char *prior;
// it's file
      if (fileflag == false) {
        file = new char [tsize];
        for (int i = 0; i < tsize+1; i++)
          file[i] = buf[pos++];
        file[tsize] = '\0';
        fileflag = true;
      }
// it's mountpoint
      else if (folderflag == false) {
        mountpoint = new char [tsize];
        for (int i = 0; i < tsize+1; i++)
          mountpoint[i] = buf[pos++];
        mountpoint[tsize] = '\0';
        folderflag = true;
      }
// it's priority
      else if (priorflag == false) {
// too big priority or some another string
        if (tsize > 255) {
          atomlog.SetLastErr(ERROR_CORE_FS, ERROR_PARSE_MOUNT_FILE_PRIORITY);
          delete [] buf;
          return -1;
        }
        prior = new char [tsize];
        for (int i = 0; i < tsize+1; i++)
          prior[i] = buf[pos++];
        prior[tsize] = '\0';
        int num = atoi(prior);
        if (num < 0 || num > 255) {
          atomlog.SetLastErr(ERROR_CORE_FS, ERROR_PARSE_MOUNT_FILE_PRIORITY);
          delete [] buf;
          return -1;
        }
        fileflag = false, folderflag = false;
// mount this file
        if (Mount(file, mountpoint, (unsigned char)num) == -1) { delete [] buf; return -1; }
      }
      if (quotes == true) quotes = false;
  }
// cleaning
  delete [] buf;
  return 0;
}
// Mount single file
int AtomFS::Mount (char* filename, char* mountfolder, unsigned char priority) {
  char buffer[1000];
  char *pbuf = buffer;
  sprintf(pbuf, "Mounting %s to %s with %i", filename, mountfolder, priority);
  atomlog.DebugMessage(pbuf);
  return 0;
}
#ifdef _FSMAN_
unsigned long int datasize = 0; // size of data in file
// Remove folder links
#ifdef UNIX
inline static int dot_exclude (const struct dirent64 *dir)
{
  if ((dir->d_name[0] == '.') && (dir->d_name[1] == 0)) return 0;
  if ((dir->d_name[0] == '.') && (dir->d_name[1] == '.')&& (dir->d_name[2] == 0)) return 0;
  return 1;
}
#endif // UNIX
LIST* AtomFS::FolderScan(char *ch, LIST* element, FILE *out, int level=0)
{
  LIST *prev = element;
  long int res=0;
  element->size = 0;
  element->link = 0;
  if (ch !=NULL)
  {
#ifdef UNIX
    if (chdir(ch) !=0) return element;
    struct dirent64 **eps;
    struct stat64 st;
    int n;
    n = scandir64 ("./", &eps, dot_exclude, alphasort64);
    if (n >= 0)
    {
      int cnt;
      for (cnt = 0; cnt < n; ++cnt)
      {
        int i=0;
        while (i<(level-1))
        {
          // printf (" | ");

          atomlog.DebugMessage(" | ");

          i++;
        }
        if (eps[cnt]->d_type == DT_DIR)
        {
          // printf("%-30s\n",eps[cnt]->d_name);

          char buf[65535];
          sprintf(buf, "%-30s\n", eps[cnt]->d_name);
          atomlog.DebugMessage(buf);

          element = new LIST;
          element->record.flag = folder;
          element->record.namelen = strlen(eps[cnt]->d_name);
          element->record.name = eps[cnt]->d_name;
          element->record.size = 0;
          element->record.offset = 0;
          element->record.crc = 0;

          prev->link = element;
          element=FolderScan(eps[cnt]->d_name, element, out, level+1);
          res+=element->size;
          prev = element;
// End of catalogue
          element = new LIST;
          element->record.flag = eoc;
          element->record.namelen = 0;
          element->record.name = 0;
          element->record.size = 0;
          element->record.offset = 0;
          element->record.crc = 0;
          prev->link = element;
          prev = element;
        }
        if (eps[cnt]->d_type==DT_REG)
        {
          if (stat64(eps[cnt]->d_name, &st) == 0) res+=(long int)st.st_size;

          char buf[65535];
          sprintf(buf, "%-30s   %ld\n", eps[cnt]->d_name, (long int)st.st_size);
          atomlog.DebugMessage(buf);

          // printf("%-30s   %ld\n",eps[cnt]->d_name, (long int)st.st_size);

          element = new LIST;
          element->record.flag = file;
          element->record.namelen = strlen(eps[cnt]->d_name);
          element->record.name = eps[cnt]->d_name;
          element->record.size = st.st_size;
        }
      }
    for (cnt = 0; cnt < n; ++cnt) free(eps[cnt]);
    free(eps);
    }
    else
    atomlog.LogMessage("Couldn't open the directory");
  chdir("..");
#endif // UNIX
  }
  element->size = res;
  element->link = 0;
  return element;
}
void AtomFS::Write (FILE *out, char *in, LIST *element) {
  FILE *file = fopen(in, "rb");
  if (in == NULL)
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_OPEN_FILE);
// check filesize
#ifndef UNIX
  if(fseek(file, 0, SEEK_END) != 0) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(file);
    return;
  }
  element->record.size = ftell(file);
  if (element->record.size == -1L) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(file);
    return;
  }
  rewind(file);
#endif // UNIX
  unsigned long int crc = mask;
// encrypt
  unsigned char *buf, count;
  if (bytescrypt < element->record.size)
    count = bytescrypt;
  else
    count = element->record.size;
  buf = new unsigned char [count];
  if(fread(buf, 1, count, file) != count) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
    fclose(file);
    delete [] buf;
    return;
  }
  unsigned int r[4];
  Crypt((unsigned int*)buf, count, wake_key, r, wake_table);
  unsigned char t = count;
// write to disk crypted data
  if(fwrite(buf, 1, count, out) != count) {
    atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
    fclose(file);
    delete [] buf;
    return;
  }
  while(t--)
    crc = crc32table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
// writing to dosk and calculating crc
  for (int i = count-1; i < element->record.size; i++) {
    if (fread(&t, 1, 1, file) != 1) {
      atomlog.SetLastErr(ERROR_CORE_FS, ERROR_READ_FILE);
      fclose(file);
      return;
    }
    crc = crc32table[(crc ^ t) & 0xFF] ^ (crc >> 8);
    if(fwrite(buf, 1, count, out) != count) {
      atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
      fclose(file);
      delete [] buf;
    return;
    }
  }
  element->record.crc = crc ^ mask;
  element->record.offset = datasize;
  datasize += element->record.size;
}

int AtomFS::Create (char **input, unsigned int count, char *file) {
  FILE *outfile;
  datasize = 0;
// check if file utput file exists
  outfile = fopen(file, "r");
  if (outfile != NULL) { // file exists
// rename the file
    int i = 0;
    bool flag = false;
    char *temp;
    while(flag != true) {
      int j = i;
      int k = 0;
      while(j>9)  j /= 10, k++;
      temp = new char [strlen(file)+k+2];
// create filename
      sprintf(temp, "%s.%i\0", file, i);
      if (rename(file,temp) == 0) flag = true;
      else i++;
      delete [] temp;
    }
  }
  fclose(outfile);
// open file for writing
  outfile = fopen(file, "wb");
// Create file (NIGHTMARE!!!)
// set static header information
  HEADER *header = new HEADER;
  header->magic = magic;
  header->version = version;
// set variable header information to zero value
  header->filesize = 0;
  header->tablesize = 0;
// Linked list of records
  LIST *list, *head, *prev;
  list = new LIST;
  list->record.flag = folder;
  list->record.namelen = 1;
  list->record.name = "/";
  list->record.size = 0;
  list->record.offset = 0;
  list->record.crc = 0;
  head = list; // save the head of the list

// Scanning...
// TODO: Add check for inputs if it is folder or file
  for (int i = 0; i < count; i++) {
    list = FolderScan(input[i], list, outfile, 0);
  }
  prev = list;
  list = new LIST;
  list->record.flag = eoc;
  list->record.namelen = 0;
  list->record.name = 0;
  list->record.size = 0;
  list->record.offset = 0;
  list->record.crc = 0;
  list->link = NULL;
  prev->link = list;
// Writing the filetable
  rewind(outfile);
  unsigned long int tablesize = 0;
  list = head;
  unsigned short int len = 0;
  while(list->link != NULL) {
// write end of catalogue flag
    if (list->record.flag == eoc) {
      if(fwrite((const void*)list->record.flag, sizeof(list->record.flag), 1, outfile) != 1) {
        atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(outfile);
        return -1;
      }
      tablesize += sizeof(list->record.flag);
    }
// write a folder
    if (list->record.flag == folder) {
      if(fwrite((const void*)list->record.flag, sizeof(list->record.flag), 1, outfile) != 1) {
        atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(outfile);
        return -1;
      }
      if(fwrite((const void*)list->record.namelen, sizeof(list->record.namelen), 1, outfile) != 1) {
        atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(outfile);
        return -1;
      }
      len = strlen(list->record.name);
      if(fwrite((const void*)list->record.name, 1, len, outfile) != len) {
        atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(outfile);
        return -1;
      }
      tablesize += (sizeof(list->record.flag)+sizeof(list->record.namelen)+len);
    }
// write a file
    else {
    if(fwrite((const void*)list->record.flag, sizeof(list->record.flag), 1, outfile) != 1) {
        atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(outfile);
        return -1;
      }
      if(fwrite((const void*)list->record.namelen, sizeof(list->record.namelen), 1, outfile) != 1) {
        atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(outfile);
        return -1;
      }
      len = strlen(list->record.name);
      if(fwrite((const void*)list->record.name, 1, len, outfile) != len) {
        atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(outfile);
        return -1;
      }
      if(fwrite((const void*)list->record.size, sizeof(list->record.size), 1, outfile) != 1) {
        atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(outfile);
        return -1;
      }
      if(fwrite((const void*)list->record.offset, sizeof(list->record.offset), 1, outfile) != 1) {
        atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(outfile);
        return -1;
      }
      if(fwrite((const void*)list->record.crc, sizeof(list->record.crc), 1, outfile) != 1) {
        atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(outfile);
        return -1;
      }
      tablesize += (sizeof(list->record.flag)+sizeof(list->record.namelen)+
                    len+sizeof(list->record.size)+sizeof(list->record.offset)+
                    sizeof(list->record.crc));
    }
   list = list->link;
  }
// writing the header
  rewind(outfile);
  header->tablesize = tablesize;
  header->filesize = tablesize+datasize;
  if(fwrite((const void*)header, sizeof(header), 1, outfile) != 1) {
        atomlog.SetLastErr(ERROR_CORE_FS, ERROR_WRITE_FILE);
        fclose(outfile);
        return -1;
      }
// end work with file
  fclose(outfile);
  delete header;
  return 0;
}
#endif // _FSMAN_
/*
#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <time.h>

using namespace std;

bool DEBUG = FALSE;
char folder[2048];
char path[2048];
char size[5];
int nsize = 1024;
char tg[3] = {"Kb"};
int filecount = 0;
char foldername[1024];
char filename[3072];

struct LIST
{
       int number;
       char folder[3072];
       char path[3072];
       LIST *link;
};

struct STRING
{
       char folder[3072];
       char path[3072];
};

LIST *head, *list, *prev;
void ScanDir()
{
    WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	int n = 0;
	char file[3072], temp[3072], wildcard[3072];
	strcpy(path, folder);
	strcat(path, "\\*");

	hf=FindFirstFile(path, &FindFileData);
	if (hf!=INVALID_HANDLE_VALUE)
	{
		do
		{
            strcpy(path, folder);
            strcat(path, "\\*");
            if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0)
            {
             strcpy(temp, folder);
             strcat(temp, "\\");
			 strcpy(foldername, FindFileData.cFileName);
			 strcat(temp, foldername);
			 if (DEBUG == true)
			  cout << "folder: " << temp << "\n";
			 HANDLE handle;
			 strcpy(wildcard, temp);
			 strcat(wildcard, "\\*.txt");
			 handle=FindFirstFile(wildcard, &FindFileData);
			 if (handle!=INVALID_HANDLE_VALUE)
			 {
               do
               {
                 strcpy(file, temp);
                 strcat(file, "\\");
                 strcat(file, FindFileData.cFileName);
                 if (DEBUG == true)
			        cout << "file: " << file << "\n";
                 list = new LIST;
                 strcpy(list->folder, foldername);
                 strcpy(list->path, file);
                 list->number = filecount;
                 if (prev != NULL) prev->link = list;
			     if (head == NULL) head = list;
			     prev = list;
                 filecount++;
               }
             while(FindNextFile(handle, &FindFileData)!=0);
             FindClose(handle);
             }
            }
		}
		while (FindNextFile(hf,&FindFileData)!=0);
		FindClose(hf);
	}
}
*/