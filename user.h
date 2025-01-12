//
// Created by toru on 2025/01/11.
//

#ifndef OS1000__USER_H_
#define OS1000__USER_H_

#include "common.h"

__attribute__((noreturn)) void exit(void);
void putchar(char ch);
int getchar(void);
int readfile(const char *filename, char *buf, int len);
int writefile(const char *filename, const char *buf, int len);

#endif //OS1000__USER_H_
