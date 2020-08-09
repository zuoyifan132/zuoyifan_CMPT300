#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "print.h"
#include "stringProcess.h"

#define MAX_LEN 100
typedef struct dirent** direntEntry;


// Print read/write/execute permission of file
void print_permission(int st_mode)
{
	printf( (S_ISDIR(st_mode)) ? "d" : "-");
    printf( (st_mode & S_IRUSR) ? "r" : "-");
    printf( (st_mode & S_IWUSR) ? "w" : "-");
    printf( (st_mode & S_IXUSR) ? "x" : "-");
    printf( (st_mode & S_IRGRP) ? "r" : "-");
    printf( (st_mode & S_IWGRP) ? "w" : "-");
    printf( (st_mode & S_IXGRP) ? "x" : "-");
    printf( (st_mode & S_IROTH) ? "r" : "-");
    printf( (st_mode & S_IWOTH) ? "w" : "-");
    printf( (st_mode & S_IXOTH) ? "x " : "- ");
}

// Get User name by UID
void getAndPrintUserName(uid_t uidNum, int uid)
{
    struct passwd *pw = getpwuid(uidNum);

    if (pw) {
    	printf("%*s ", uid, pw->pw_name);
    } else {
        printf("No name found for %u\n", uidNum);
    }
}

// Get group name by GID
void getAndPrintGroup(gid_t grpNum, int gid)
{
    struct group *grp = getgrgid(grpNum);

    if (grp) {
    	printf("%*s ", gid, grp->gr_name);
    } else {
        printf("No group name for %u found\n", grpNum);
    }
}
	
// directory(2) file(1) invalid(-1) 
int check_file_directory_invalid(char* path)
{
	DIR *dir_ptr;
	struct stat buf;

	if ((dir_ptr = opendir(path)) != NULL){
		closedir(dir_ptr);
		return 2;
	}
	if (stat(path, &buf) >= 0)
		return 1;
	closedir(dir_ptr);
	return -1;
}

// if path is softlink(return orginal file buf) otherwise return NULL
char* identify_soft_link(char* path, char* buf)
{
	int link = readlink(path, buf, MAX_LEN-1);
	buf[link] = '\0';

	if((link < 0) || (link >= MAX_LEN-1)){
		return NULL;
	}

	for (int i = link; i>= 0; i--){
		if (buf[i] == '/'){
			buf[i+1] = '\0';
			break;
		}
	}

	return buf;
}

// print the information based on file
void print_info_base_on_file(char* path, _Bool i, _Bool l, int inode, int nlink, int uid, int gid, int size)
{
	// Declare stat struct 
	struct stat buf;
	// Use to print time
	struct tm *time;
	char print_time[40];

	// Get the file or directory name 
	int error = lstat(path, &buf);

	// check stat failure
	if (error < 0){
		printf("print_info_base_on_path static error\n");
		exit(1);
	}

	// if -i is true, print inode
	if (i == true)
		printf("%*ld ", inode, buf.st_ino);

	// if -l is true, print long information
	if (l == true){
		// print file permission
		print_permission(buf.st_mode);
		// print number of hard link
		printf("%*ld ", nlink, buf.st_nlink);
		// print user name 
		getAndPrintUserName(buf.st_uid, uid);
		// print group name 
		getAndPrintGroup(buf.st_gid, gid);
		// print file size
		printf("%*ld ", size, buf.st_size);
		// print last time file modified
		time = localtime(&buf.st_mtime); 
		strftime(print_time, sizeof(print_time), "%b %2d %4Y %H:%M",time); 		
		printf("%s ", print_time);	
	}
	// print file or directoey name 
	char buf2[MAX_LEN];
	if (identify_soft_link(path, buf2))
		printf("%s -> %s\n", path, buf2);
	else	
		printf("%s\n",path);
}

// print the information based on ilR
void print_info_base_on_path(char* path, struct dirent* dp, _Bool i, _Bool l, int inode, int nlink, int uid, int gid, int size)
{
	// Declare stat struct 
	struct stat buf;
	// Use to print time
	struct tm *time;
	char print_time[40];

	// Get the fiel or directory name 
	int error = lstat(path, &buf);

	// check stat failure
	if (error < 0){
		printf("print_info_base_on_path static error\n");
		exit(1);
	}

	// filter all unwanted files and directories
	if (filter(dp->d_name))
	{
		// if -i is true, print inode
		if (i == true)
			printf("%*ld ", inode, buf.st_ino);

		// if -l is true, print long information
		if (l == true){
			// print file permission
			print_permission(buf.st_mode);
			// print number of hard link
			printf("%*ld ", nlink, buf.st_nlink);
			// print user name 
			getAndPrintUserName(buf.st_uid, uid);
			// print group name 
			getAndPrintGroup(buf.st_gid, gid);
			// print file size
			printf("%*ld ", size, buf.st_size);
			// print last time file modified
			time = localtime(&buf.st_mtime); 
			strftime(print_time, sizeof(print_time), "%b %2d %4Y %H:%M",time); 		
			printf("%s ", print_time);
		}

		// print file or directory name 
		char buf2[MAX_LEN];
		if (identify_soft_link(path, buf2))
			printf("%s -> %s\n", dp->d_name, buf2);
		else	
			printf("%s\n",dp->d_name);
	}
}

// DFS for recursively print
void dfs_print(_Bool i, _Bool l, char* path, int inode, int nlink, int uid, int gid, int size)
{	
	struct dirent* dp;
	struct stat buf;
	direntEntry entryList;
	char* dirList[1024];
	int dirNum = 0;

	// Scan the dir and sort it lexicographically using alphasort
	int count = scandir(path, &entryList, 0, alphasort);

	// Scan dir failure 
	if (count <= 0){
		printf("dfs_print scandir failure\n");
		exit(1);
	}

	// Loop through all entries in the corresponding dir path
	printf("%s:\n", path);
	for(int k = 0; k < count; k++)
	{	
		dp = entryList[k];

		char temp[1024];
    	if(path[strlen(path)-1] == '/')
        	snprintf(temp, sizeof(temp), "%s%s", path, dp->d_name);
    	else
        	snprintf(temp, sizeof(temp), "%s/%s", path, dp->d_name);

		int error = lstat(temp, &buf);

		// check stat failure
		if (error < 0){
			printf("dfs_print static error");
			exit(1);
		}

		// if dp is a directory and is not hiden file, "." or ".." 
		if (S_ISDIR(buf.st_mode) && filter(dp->d_name)){
			char* newPath = (char*) malloc(1024*sizeof(char));
			snprintf(newPath, 1024, "%s/%s", path, dp->d_name);
			dirList[dirNum] = newPath;
			dirNum++;
			print_info_base_on_path(newPath, dp, i, l, inode, nlink, uid, gid, size);
		}

		// if dp is file and is not hiden file, "." or ".." 
		else if (!S_ISDIR(buf.st_mode) && filter(dp->d_name)){
			char newPath[1024];
			snprintf(newPath, sizeof(newPath), "%s/%s", path, dp->d_name);
			print_info_base_on_path(newPath, dp, i, l, inode, nlink, uid, gid, size);
		}

		free(dp);
	}

	// recursive call
	for (int p = 0; p < dirNum; p++){
		printf("\n");
		dfs_print(i, l, dirList[p], inode, nlink, uid, gid, size);
	}

	// clean up
	for (int q = 0; q < dirNum; q++)
		free(dirList[q]);
	free(entryList);
}

// Check if option conatin i or l or R and print base on the corresponding option
void print_with_option(_Bool i, _Bool l, _Bool R, char* path, int inode, int nlink, int uid, int gid, int size)
{
	struct dirent* dp;
	// Count number files and directories 
	int count;
	// entry lists contained in the specific path
	direntEntry entryList;

	// List subdirectories recursively
	if (R == true)
		dfs_print(i, l, path, inode, nlink, uid, gid, size);

	else{
		// Scan the dir and sort it lexicographically using alphasort
		count = scandir(path, &entryList, 0, alphasort);

		// Scan dir failure 
		if (count <= 0){
			printf("scandir failure\n");
			exit(1);
		}

		// Loop through all entries in the corresponding dir path
		for(int k = 0; k < count; k++)
		{	
			dp = entryList[k];
			char newPath[1024];
			snprintf(newPath, sizeof(newPath), "%s/%s", path, dp->d_name);
			print_info_base_on_path(newPath, dp, i, l, inode, nlink, uid, gid, size);
			free(dp);
		}
		free(entryList);
	}
}

// Print current directory files and directories
void print_no_option(char* dirname)
{
	DIR *dirPtr;
	struct dirent *dp;
 
	dirPtr = opendir(dirname);

	// Open file fail
	if(dirPtr == NULL){
		fprintf(stderr,"Error! can not open %s\n",dirname);
		exit(1);
	}

	while((dp = readdir(dirPtr)) != NULL)
		if (filter(dp->d_name)){
			// print file or directoey name 
			char buf2[MAX_LEN];
			if (identify_soft_link(dp->d_name, buf2))
				printf("%s -> %s\n", dp->d_name, buf2);
			else	
				printf("%s\n",dp->d_name);
		}
	closedir(dirPtr);
}
