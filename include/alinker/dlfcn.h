#ifndef _ALINKER_DLFCN_H_
#define _ALINKER_DLFCN_H_

#include <dlfcn.h>

void *android_dlopen(const char *filename, int flag);

const char *android_dlerror(void);

void *android_dlsym(void *handle, const char *symbol);

int android_dladdr(void *addr, Dl_info *info);

int android_dlclose(void *handle);

#endif