#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

/*
bool parse_int(char *str, int *val){

	char *endptr;

	long int v = strtol(str, &endptr, 10);

	if(endptr == str || *endptr != '\0')
		return false;


	if(v > INT_MAX || v < INT_MIN )
		return false;

	*val = (int) v;

	return true;
}

bool parse_double(char *str, double *val){
	char *endptr;

	double v = strtod(str, &endptr);

	if(endptr == str || *endptr != '\0')
		return false;

	*val = (double) v;

	return true;
}

*/


static char *laststr = NULL;
char *spacetok(char *str){
	char c, *tok;


	if(str == NULL)
		str = laststr;


	// Skip initial whitespace
	c = *str;
	while(c != '\0' && isspace(c)){
		str++;
		c = *str;
	}


	if(c == '\0')
		return NULL;


	tok = str;

	while(c != '\0' && !isspace(c)){
		str++;
		c = *str;
	}

	if(c == '\0'){ // It is the actual end of the string
		laststr = str;
	}
	else{
		*str = '\0';
		laststr = str + 1;
	}


	return tok;
}


void keyvalue_split(char *str, char **key, char **value){

	// Find the delimeter
	char *v = str;
	for(; *v != '\0'; v++){
		// Split the string on the delimeter
		if(*v == ':'){
			*v = '\0';
			v++;
			break;
		}
	}

	*key = strip(str);
	*value = strip(v);
}


char *strip(char *str){
	// Strip leading whitespace
	while(isspace(*str))
		str++;

	// Check for empty string
	if(*str == '\0')
		return str;

	// Strip trailing whitespace
	char *last = str + strlen(str) - 1;
	for(; isspace(*last); last--){
		*last = '\0';
	}

	return str;
}




