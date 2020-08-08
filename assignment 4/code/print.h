#ifndef _PRINT_H_
#define _PRINT_H_

void print_no_option();
void print_permission(int st_mode);
void getAndPrintUserName(uid_t uid);
void getAndPrintGroup(gid_t grpNum);
int check_file_directory_invalid(char* path);
void print_info_base_on_file(char* path, _Bool i, _Bool l);
void print_info_base_on_path(char* path, struct dirent* dp, _Bool i, _Bool l);
void dfs_print(_Bool i, _Bool l, char* path);
void print_with_option(_Bool i, _Bool l, _Bool R, char* path);

#endif