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
	// sort user input
	sort_user_input(argc, args);

	// Only one input, ls current directory
	if (argc == 1){
		char path[MAX_LEN] = ".";
		print_no_option(path);
		return 0;
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

	// with option but no file list
	if (index == argc)
	{
		// print the information based on input
		print_with_option(i, l, R, ".");
		return 0;
	}

	// more file list 
	if (index < argc-1)
	{
		// determine file or directory
		while (index < argc)
		{	
			// if args is directory
			if (check_file_directory_invalid(args[index]) == 2){
				//print root dir
				if (R == false)
					printf("%s:\n", args[index]);
				// print the information based on input
				print_with_option(i, l, R, args[index]);
				//if (R == false && index < argc-1)
				//	printf("h\n");
				if (index < argc-1 && check_file_directory_invalid(args[index+1]) == 2)
					printf("\n");
			}

			// if args is file 
			else if (check_file_directory_invalid(args[index]) == 1){
				print_info_base_on_file(args[index], i, l);
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
			// print the information based on input
			print_with_option(i, l, R, args[index]);
		}

		// if args is file 
		else if (check_file_directory_invalid(args[index]) == 1){
			print_info_base_on_file(args[index], i, l);
		}

		// if args is invalid 
		else{
			printf("invalid input\n");
			exit(1);
		}
	}

	return 0;
}