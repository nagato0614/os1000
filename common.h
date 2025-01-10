//
// Created by toru on 2025/01/10.
//

#ifndef OS1000__COMMON_H_
#define OS1000__COMMON_H_

#define va_list  __builtin_va_list
#define va_start __builtin_va_start
#define va_end   __builtin_va_end
#define va_arg   __builtin_va_arg

void printf(const char *fmt, ...);

#endif //OS1000__COMMON_H_
