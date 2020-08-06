#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "print.h"
#include "stringProcess.h"

#define MAX_LEN 100


int main(int argc, char** args)
{
	/*struct stat buf;
	char path[1024] = "./test";
	int error = lstat(path, &buf);
	printf("error is: %d\n", error);
	if (error < 0){
		printf("stat failure\n");
		exit(1);
	}

	printf("%7ld ", buf.st_ino);*/

	// Only one input, ls current directory
	if (argc == 1){
		char path[MAX_LEN] = ".";
		print_no_option(path);
	}

	int index = 1;
	_Bool i = false;
	_Bool l = false;
	_Bool R = false;

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

	// determine file or directory
	while (index < argc)
	{		
		// print the information based on input
		print_with_option(i, l, R, args[index]);

		index++;
	}

	return 0;
}