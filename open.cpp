/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "private/libc_logging.h"
//#include <execinfo.h>

// void printStack(void)
// {
//   #define STACK_SIZE 32;
//   void *trace[STACK_SIZE];
//   size_t size = backtrace(trace, STACK_SIZE);
//   char **symbols = (char **)backtrace_symbols(trace,size);
//   size_t i = 0;
//   for(; i<size; i++)
//   {
//     printf("%d--->%s\n", i, symbols[i]);
//   }
//   return;
// }


extern "C" int __openat(int, const char*, int, int);

static inline int force_O_LARGEFILE(int flags) {
#if __LP64__
  return flags; // No need, and aarch64's strace gets confused.
#else
  return flags | O_LARGEFILE;
#endif
}

int creat(const char* pathname, mode_t mode) {
  return open(pathname, O_CREAT | O_TRUNC | O_WRONLY, mode);
}
__strong_alias(creat64, creat);


int is_begin_with(const char * str1,const char *str2)
{
  if(str1 == NULL || str2 == NULL)
    return 0;
  int len1 = strlen(str1);
  int len2 = strlen(str2);
  if((len1 < len2) || (len1 == 0 || len2 == 0))
    return 0;
  const char *p = str2;
  int i = 0;
  while(*p != '\0')
  {
    if(*p != str1[i])
      return 0;
    p++;
    i++;
  }
  return 1;
}

int open(const char* pathname, int flags, ...) {
  mode_t mode = 0;

  if ((flags & O_CREAT) != 0) {
    va_list args;
    va_start(args, flags);
    mode = static_cast<mode_t>(va_arg(args, int));
    va_end(args);
  }

  if(strcmp(pathname, "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq") == 0 ||
     strcmp(pathname, "/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_cur_freq") == 0 ||
     strcmp(pathname, "/sys/devices/system/cpu/cpu2/cpufreq/cpuinfo_cur_freq") == 0 ||
     strcmp(pathname, "/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_cur_freq") == 0 ||
     strcmp(pathname, "/sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_cur_freq") == 0 ||
     strcmp(pathname, "/sys/devices/system/cpu/cpu5/cpufreq/cpuinfo_cur_freq") == 0 ||
     strcmp(pathname, "/sys/devices/system/cpu/cpu6/cpufreq/cpuinfo_cur_freq") == 0 ||
     strcmp(pathname, "/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_cur_freq") == 0)
  {
    return __openat(AT_FDCWD, "/sdcard/zhangdaye12344", force_O_LARGEFILE(flags), mode);
  }

 if(is_begin_with(pathname, "/sys/devices/system/cpu"))
 {
     char temp[256]; 
     strcpy(temp, pathname + 23);
     char result[256] = "/sdcard/cpu";
     strcat(result, temp);
     return __openat(AT_FDCWD, result, force_O_LARGEFILE(flags), mode);
 }

  if(strcmp(pathname, "/proc/cpuinfo") == 0)
  {
    return __openat(AT_FDCWD, "/sdcard/cpuinfo", force_O_LARGEFILE(flags), mode);
  }

  if(strcmp(pathname, "/proc/uptime") == 0)
  {
    return __openat(AT_FDCWD, "/sdcard/uptime", force_O_LARGEFILE(flags), mode);
  }


  return __openat(AT_FDCWD, pathname, force_O_LARGEFILE(flags), mode);
}
__strong_alias(open64, open);

int __open_2(const char* pathname, int flags) {
  if (__predict_false((flags & O_CREAT) != 0)) {
    __fortify_chk_fail("open(O_CREAT): called without specifying a mode", 0);
  }

  return __openat(AT_FDCWD, pathname, force_O_LARGEFILE(flags), 0);
}

int openat(int fd, const char *pathname, int flags, ...) {
  mode_t mode = 0;

  if ((flags & O_CREAT) != 0) {
    va_list args;
    va_start(args, flags);
    mode = static_cast<mode_t>(va_arg(args, int));
    va_end(args);
  }

  return __openat(fd, pathname, force_O_LARGEFILE(flags), mode);
}
__strong_alias(openat64, openat);

int __openat_2(int fd, const char* pathname, int flags) {
  if ((flags & O_CREAT) != 0) {
    __fortify_chk_fail("openat(O_CREAT): called without specifying a mode", 0);
  }

  return __openat(fd, pathname, force_O_LARGEFILE(flags), 0);
}
