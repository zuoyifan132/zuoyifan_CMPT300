#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

#include "print.h"
#include "stringProcess.h"
#include "maxLen.h"

#define MAX_LEN 100


int main(int argc, char** args)
{	
	// varible declare
	int inode = 0;
	int nlink = 0;
	int uid = 0;
	int gid = 0;
	int size = 0;
	int index = 1;
	_Bool i = false;
	_Bool l = false;
	_Bool R = false;

	// sort user input
	sort_user_input(argc, args);

	// Only one input, ls current directory
	if (argc == 1){
		char path[MAX_LEN] = ".";
		print_no_option(path);
		return 0;
	}

	// determine ilR
	while (index < argc)
	{
		if (args[index][0] == '-')
		{
			// Check if contain invalid options 
			if (option_checker(args[1]) == false){
				printf("Invalid option\n");
				exit(1);
			}

			if (check_contain_char(args[index], 'i'))
				i = true;
			if (check_contain_char(args[index], 'l'))
				l = true;
			if (check_contain_char(args[index], 'R'))
				R = true;
		}

		else
			break;
		index++;
	}

	// with option but no file list
	if (index == argc)
	{
		if (R == true){
			recursive_maxLen(".", &inode, &nlink, &uid, &gid, &size);
			print_with_option(i, l, R, ".", inode, nlink, uid, gid, size);
		}
		else{
			not_recursive_maxLen(".", &inode, &nlink, &uid, &gid, &size);
			print_with_option(i, l, R, ".", inode, nlink, uid, gid, size);
		}
	}

	// more file list 
	else if (index < argc-1)
	{	
		// determine max len of file and directory
		int tempindex = index;
		while (tempindex < argc)
		{
			// directory
			if (check_file_directory_invalid(args[tempindex]) == 2){
				if (R == true)
					recursive_maxLen(args[tempindex], &inode, &nlink, &uid, &gid, &size);
				else
					not_recursive_maxLen(args[tempindex], &inode, &nlink, &uid, &gid, &size);
			}
			// file
			else if (check_file_directory_invalid(args[tempindex]) == 1){
				file_maxLen(args[tempindex], &inode, &nlink, &uid, &gid, &size);
			}
			// if args is invalid 
			else{
				printf("invalid input\n");
				exit(1);
			}
			tempindex++;
		}

		// print file or directory
		while (index < argc)
		{	
			// if args is directory
			if (check_file_directory_invalid(args[index]) == 2){
				//print root dir
				if (R == false)
					printf("%s:\n", args[index]);
				// print the information based on input
				print_with_option(i, l, R, args[index], inode, nlink, uid, gid, size);
				if (index < argc-1 && check_file_directory_invalid(args[index+1]) == 2)
					printf("\n");
			}

			// if args is file 
			else if (check_file_directory_invalid(args[index]) == 1){
				print_info_base_on_file(args[index], i, l, inode, nlink, uid, gid, size);
				if (index < argc-1 && check_file_directory_invalid(args[index+1]) == 2)
					printf("\n");
			}

			// if args is invalid 
			else{
				printf("invalid input\n");
				exit(1);
			}
			index++;
		}
	}

	// only one file list
	else
	{
		// if args is directory
		if (check_file_directory_invalid(args[index]) == 2){
			if (R == true)
				recursive_maxLen(args[index], &inode, &nlink, &uid, &gid, &size);
			else
				not_recursive_maxLen(args[index], &inode, &nlink, &uid, &gid, &size);

			print_with_option(i, l, R, args[index], inode, nlink, uid, gid, size);
		}

		// if args is file 
		else if (check_file_directory_invalid(args[index]) == 1){
			file_maxLen(args[index], &inode, &nlink, &uid, &gid, &size);
			print_info_base_on_file(args[index], i, l, inode, nlink, uid, gid, size);
		}

		// if args is invalid 
		else{
			printf("invalid input\n");
			exit(1);
		}
	}

	return 0;
}