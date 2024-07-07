#ifndef STDIO_H

#define STDIO_H

#define NULL ((void *)0)

extern char *stdout;
extern char *stdin;
extern char *stderr;

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
int printf(const char *format, ...);
int sprintf(char *str, const char *format, ...);
char *ftoa(double f);
char *itoa(int x);
#endif