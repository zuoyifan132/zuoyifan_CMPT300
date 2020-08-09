#ifndef _PRINT_H_
#define _PRINT_H_

void print_permission(int st_mode);
void getAndPrintUserName(uid_t uidNum, int uid);
void getAndPrintGroup(gid_t grpNum, int gid);
int check_file_directory_invalid(char* path);
char* identify_soft_link(char* path, char* buf);
void print_info_base_on_file(char* path, _Bool i, _Bool l, int inode, int nlink, int uid, int gid, int size);
void print_info_base_on_path(char* path, struct dirent* dp, _Bool i, _Bool l, int inode, int nlink, int uid, int gid, int size);
void dfs_print(_Bool i, _Bool l, char* path, int inode, int nlink, int uid, int gid, int size);
void print_with_option(_Bool i, _Bool l, _Bool R, char* path, int inode, int nlink, int uid, int gid, int size);
void print_no_option(char* dirname);

#endif