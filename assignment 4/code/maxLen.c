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

#include "maxLen.h"
#include "stringProcess.h"

#define MAX_LEN 100
typedef struct dirent** direntEntry;


// count number of digit the number has
int num_digit(long int num)
{
    int count = 0;

    while(num != 0)
    {
        num /= 10;
        ++count;
    }
 
    return count;
}

// comparator
void comparator(struct stat buf, int* inode, int* nlink, int* uid, int* gid, int* size)
{
	//find max inode len
	if (num_digit(buf.st_ino) > *inode)
	*inode = num_digit(buf.st_ino);

	//find max hard link len
	if (num_digit(buf.st_nlink) > *nlink)
	*nlink = num_digit(buf.st_nlink);

	// find max uid len
	struct passwd *pw = getpwuid(buf.st_uid);
    if (pw){
        if (strlen(pw->pw_name) > *uid)
        *uid = strlen(pw->pw_name);
    } 
    else
        printf("No name found for %u\n", buf.st_uid);

    // find max gid len
	struct group *grp = getgrgid(buf.st_gid);

    if (grp) {
       	if (strlen(grp->gr_name) > *gid)
        	*gid = strlen(grp->gr_name);
    } 
    else
        printf("No group name for %u found\n", buf.st_gid);

	// find max size len
	if (num_digit(buf.st_size) > *size)
		*size = num_digit(buf.st_size);
}

// used in print_with_option not recursive
void not_recursive_maxLen(char* path, int* inode, int* nlink, int* uid, int* gid, int* size)
{
	struct dirent* dp;
	// entry lists contained in the specific path
	direntEntry entryList;
	// Count number files and directories 
	int count;

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
		//print_info_base_on_path(newPath, dp, i, l);

		// Declare stat struct 
		struct stat buf;

		// Get the file or directory name 
		int error = lstat(newPath, &buf);

		// check stat failure
		if (error < 0){
			printf("not_recersive_maxLen static error\n");
			exit(1);
		}

		// filter all unwanted files and directories
		if (filter(dp->d_name))
			comparator(buf, inode, nlink, uid, gid, size);

		free(dp);
	}
	free(entryList);
}

// used in print_with_option recursive
void recursive_maxLen(char* path, int* inode, int* nlink, int* uid, int* gid, int* size)
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
		printf("recursive_maxLen scandir failure\n");
		exit(1);
	}

	// Loop through all entries in the corresponding dir path
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
			comparator(buf, inode, nlink, uid, gid, size);
		}

		// if dp is file and is not hiden file, "." or ".." 
		else if (!S_ISDIR(buf.st_mode) && filter(dp->d_name)){
			char newPath[1024];
			snprintf(newPath, sizeof(newPath), "%s/%s", path, dp->d_name);
			comparator(buf, inode, nlink, uid, gid, size);
		}

		free(dp);
	}

	// recursive call
	for (int p = 0; p < dirNum; p++)
		recursive_maxLen(dirList[p], inode, nlink, uid, gid, size);

	// clean up
	for (int q = 0; q < dirNum; q++)
		free(dirList[q]);
	free(entryList);
}

// used in print_info_base_on_file 
void file_maxLen(char* path, int* inode, int* nlink, int* uid, int* gid, int* size)
{
	// Declare stat struct 
	struct stat buf;

	// Get the file or directory name 
	int error = lstat(path, &buf);

	// check stat failure
	if (error < 0){
		printf("print_info_base_on_path static error\n");
		exit(1);
	}

	comparator(buf, inode, nlink, uid, gid, size);
}