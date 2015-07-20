#ifndef UTILS_H_
#define UTILS_H_

#include <limits.h>

// Error checked integer parsing
//bool parse_int(char *str, int *val);

// Error checked double parsing
//bool parse_double(char *str, double *val);

// Gets a files extension from its name/path
const char *get_filename_ext(const char *filename);

// Tokenizes a string by spaces; like strtok initally pass in the start string and then pass NULL
char *spacetok(char *str);

#endif
