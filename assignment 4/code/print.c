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

// Print current directory files and directories
void print_no_option(char* dirname)
{
	DIR *dirPtr;
	struct dirent *dp;
	//char* dirname = ".";
 
	dirPtr = opendir(dirname);

	// Open file fail
	if(dirPtr == NULL){
		fprintf(stderr,"Error! can not open %s\n",dirname);
		exit(1);
	}

	while((dp = readdir(dirPtr)) != NULL)
		if (filter(dp->d_name))
			printf("%s\n",dp->d_name);
	closedir(dirPtr);
}

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
void getAndPrintUserName(uid_t uid)
{
    struct passwd *pw = getpwuid(uid);

    if (pw) {
        printf("%9s ", pw->pw_name);
    } else {
        printf("No name found for %u\n", uid);
    }
}

// Get group name by GID
void getAndPrintGroup(gid_t grpNum)
{
    struct group *grp = getgrgid(grpNum);

    if (grp) {
        printf("%9s ", grp->gr_name);
    } else {
        printf("No group name for %u found\n", grpNum);
    }
}

// Get all entries of that directory
direntEntry get_entry_list(char* path, int* count)
{
	direntEntry entryList = (direntEntry) malloc(sizeof(direntEntry));

	*count = scandir(path, &entryList, 0, alphasort);
}

// print the information based on ilR
void print_info_base_on_path(char* path, struct dirent* dp, _Bool i, _Bool l)
{
	// Declare stat struct 
	struct stat buf;
	// Use to print time
	struct tm *time;
	char print_time[40];

	// Get the fiel or directory name 
	int error = stat(path, &buf);
	//printf("dp name is: %s\n",dp->d_name);
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
			printf("%7ld ", buf.st_ino);

		// if -l is true, print long information
		if (l == true){
			// print file permission
			print_permission(buf.st_mode);
			// print number of hard link
			printf("%1ld ", buf.st_nlink);
			// print user name 
			getAndPrintUserName(buf.st_uid);
			// print group name 
			getAndPrintGroup(buf.st_gid);
			// print file size
			printf("%5ld ", buf.st_size);
			// print last time file modified
			time = localtime(&buf.st_mtime); 
			strftime(print_time, sizeof(print_time), "%b %2d %4Y %H:%M",time); 		
			printf("%s ", print_time);	
		}
		// print file or directoey name 
		printf("%s\n",dp->d_name);
	}
}

// DFS for recursively print
void dfs_print(_Bool i, _Bool l, char* path)
{	
	struct dirent* dp;
	struct stat buf;
	direntEntry entryList;

	// Scan the dir and sort it lexicographically using alphasort
	int count = scandir(path, &entryList, 0, alphasort);

	// Scan dir failure 
	if (count <= 0){
		printf("dfs_print scandir failure\n");
		exit(1);
	}

	// Loop through all entries in the corresponding dir path
	printf("\n%s:\n", path);
	for(int k = 0; k < count; k++)
	{	
		dp = entryList[k];

		char temp[1024];
    	if(path[strlen(path)-1] == '/')
        	snprintf(temp, sizeof(temp), "%s%s", path, dp->d_name);
    	else
        	snprintf(temp, sizeof(temp), "%s/%s", path, dp->d_name);

		int error = stat(temp, &buf);
		//printf("temp is: %s\n", temp);

		// check stat failure
		if (error < 0){
			printf("dfs_print static error");
			exit(1);
		}

		// if dp is a directory and is not hiden file, "." or ".." 
		if (S_ISDIR(buf.st_mode) && filter(dp->d_name)){
			char newPath[1024];
			snprintf(newPath, sizeof(newPath), "%s/%s", path, dp->d_name);
			print_info_base_on_path(newPath, dp, i, l);
			dfs_print(i, l, newPath);
		}

		// if dp is file and is not hiden file, "." or ".." 
		else if (!S_ISDIR(buf.st_mode) && filter(dp->d_name)){
			char newPath[1024];
			snprintf(newPath, sizeof(newPath), "%s/%s", path, dp->d_name);
			print_info_base_on_path(newPath, dp, i, l);
		}
	}
	free(entryList);
}

/*int PrintDir(char *path, _Bool i, _Bool l)
{
    DIR* pDir;
    struct dirent* dp;
    struct stat  buf;
 
    if((pDir = opendir(path)) == NULL)
    {
        printf("opendir failure on path: %s\n", path);
        exit(1);
    }
 	
 	// change directory to path
    chdir(path);
 	
    printf("\n%s:\n", path);

    while((dp = readdir(pDir)) != NULL)
    {
        int error = stat(dp->d_name, &buf);

		// check stat failure
		if (error < 0){
			printf("static error\n");
			exit(1);
		}
 
        if(S_ISDIR(buf.st_mode) && filter(dp->d_name))
        {
            //printf("%s\n", dp->d_name);
 			print_info_base_on_path(dp, i, l);
            PrintDir(dp->d_name, i, l);
        }
        else if(!S_ISDIR(buf.st_mode) && filter(dp->d_name))
        {
            //printf("%s\n", dp->d_name);
            print_info_base_on_path(dp, i, l);
        }
    }
 	
 	// change directory to previous directory
    chdir("..");
 
	closedir(pDir);
 
    return 0;
}

void ListDir(char* path, _Bool i, _Bool l)
{
    DIR* pDir;
    int max_depth,j;
    char c;
    struct dirent* dp;
    struct stat buf;
    char dirList[100][256];

    if((pDir = opendir(path)) == NULL)
    {
       	printf("opendir failure on path: %s\n", path);
       	exit(1);
    }

    // show root directory path
    printf("%s:\n",path);

    max_depth = 0;
    while((dp = readdir(pDir)) != NULL)
    {
        char temp[1024];

        if(path[strlen(path)-1] == '/')
            snprintf(temp, sizeof(temp), "%s%s", path, dp->d_name);
        else
            snprintf(temp, sizeof(temp), "%s/%s", path, dp->d_name);

        print_info_base_on_path(dp, i, l);

        int error = stat(temp,&buf);

		// check stat failure
		if (error <= 0){
			printf("static error\n");
			exit(1);
		}

        // path is directory add to dirList 
        if(S_ISDIR(buf.st_mode) && filter(dp->d_name))
        {
            strcpy(dirList[max_depth++],temp);  
            if( max_depth > 100 )
            {
                max_depth--;
                break;
            }
        }
    }
    closedir(pDir);
    printf("\n");
    //recursive print 
    for(j = 0; j < max_depth; j++)
    {
        ListDir(dirList[j], i, l);
    }
    return;
}*/

// Check if option conatin i or l or R and print base on the corresponding option
void print_with_option(_Bool i, _Bool l, _Bool R, char* path)
{
	struct dirent* dp;

	// process the path
    char temp[1024];
	char prefix[2] = "./";
	snprintf(temp, sizeof(temp), "%s%s", prefix, path);

	// Count number files and directories 
	int count;
	// entry lists contained in the specific path
	direntEntry entryList;

	// List subdirectories recursively
	if (R == true)
		dfs_print(i, l, path);

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
			print_info_base_on_path(newPath, dp, i, l);
		}
		free(entryList);
	}
}