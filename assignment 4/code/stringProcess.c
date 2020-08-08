#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "stringProcess.h"
#include "print.h"

#define MAX_LEN 100


// Check if the give char array contain specific char
_Bool check_contain_char(char* s, char c)
{
	char ch;
	int i = 0;

	// Bad pointer
	if (s == NULL)
		return false;

	// Loop through the char*
	while(1){
		ch = *(s+i);
		if (ch == c)
			return true;
		if (ch == 0)
			break;
		i++;
	}

	// Do not find the char
	return false;
}

// Option checker, conatin invalid option return false, otherwise return true
_Bool option_checker(char* s)
{
	char ch;
	int i = 1;

	// Bad pointer
	if (s == NULL)
		return false;

	// s just contain "-"
	if (strlen(s) < 2)
		return false;

	while(1){
		ch = *(s+i);
		if (ch == 0)
			break;
		if (ch != 'i' && ch != 'l' && ch != 'R')
			return false;
		i++;
	}
	return true;
}

// Dir filter, filt out hiden file and . and ..
_Bool filter(char* s)
{
	if (*(s) == 46)
		return false;
	return true;
}

// Sort user input base on Ascii  
void sort_user_input(int argc, char** args)
{
	int index = 1;
	while (index < argc)
	{
		if (args[index][0] != '-')
			break;
		index++;
	}

	// sort file and directory separately
	int index1 = index;
	int fileIndex = 0;
	int dirIndex = 0;
	char** file = (char**) malloc(argc*sizeof(char*));
	char** dir = (char**) malloc(argc*sizeof(char*));
	while (index1 < argc)
	{
		if (check_file_directory_invalid(args[index1]) == 2){
			dir[dirIndex++] = args[index1];
		}
		if (check_file_directory_invalid(args[index1]) == 1){
			file[fileIndex++] = args[index1];
		}
		index1++;
	}

	// sort file
   	for (int i = 0; i < fileIndex-1; i++)         
       	for (int j = 0; j < fileIndex-i-1; j++)  
        	if (strcmp(file[j], file[j+1]) > 0)
        	{
        		char* temp = file[j];
        		file[j] = file[j+1];
        		file[j+1] = temp;
        	} 

	// sort directory
   	for (int i = 0; i < dirIndex-1; i++)         
       	for (int j = 0; j < dirIndex-i-1; j++)  
        	if (strcmp(dir[j], dir[j+1]) > 0)
        	{
        		char* temp = dir[j];
        		dir[j] = dir[j+1];
        		dir[j+1] = temp;
        	}    

    // Copy sorted file to args
    int fileIndex1 = 0;
    while (index < argc && fileIndex1 < fileIndex)
    {
    	args[index++] = file[fileIndex1++];
    }

    // Copy sorted directory to args
    int dirIndex1 = 0;
    while (index < argc && dirIndex1 < dirIndex)
    {
    	args[index++] = dir[dirIndex1++];
    }

    free(file);
    free(dir);        
}