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
