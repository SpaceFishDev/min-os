#ifndef STDIO_H

#define STDIO_H

#define NULL ((void *)0)

extern char *stdout;
extern char *stdin;
extern char *stderr;

extern int stdout_index;
extern int stderr_index;
extern int stdin_index;
#include "../utils/util.h"
// typedef struct _iobuf
// {
//     char *_ptr;
//     int _cnt;
//     char *_base;
//     int _flag;
//     int _file;
//     int _charbuf;
//     int _bufsiz;
//     char *_tmpfname;
// } FILE;
// temporarily use a char* until file system is implimented
#include <stdarg.h>

char *itoa(int x);

int fprintf(char *stream, const char *format, ...);
int printf(char *format, ...);
int sprintf(char *str, const char *format, ...);
char *ftoa(double f);
char *itoa(int x);
void putc(char c);
char getc();
char *gets();
char *getline();
bool strcmp(char *a, char *b);
#endif