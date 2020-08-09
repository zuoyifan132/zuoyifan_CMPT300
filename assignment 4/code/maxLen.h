#ifndef _MAXLEN_H_
#define _MAXLEN_H_

void not_recursive_maxLen(char* path, int* inode, int* nlink, int* uid, int* gid, int* size);
void recursive_maxLen(char* path, int* inode, int* nlink, int* uid, int* gid, int* size);
void file_maxLen(char* path, int* inode, int* nlink, int* uid, int* gid, int* size);

#endif