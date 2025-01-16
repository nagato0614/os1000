#ifndef OS1000__USER_H_
#define OS1000__USER_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct sysret
{
  int a0;
  int a1;
  int a2;
};

void putchar(char ch);
int getchar(void);
int readfile(const char *filename, char *buf, int len);
int writefile(const char *filename, const char *buf, int len);
__attribute__((noreturn)) void exit(void);

#ifdef __cplusplus
}
#endif

#endif // OS1000__USER_H_