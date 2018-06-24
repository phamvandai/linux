#ifndef LINUX_UTILS
#define LINUX_UTILS

#include <stdint.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>


int LinuxUtils_LockInstance(const char *lock_file, const char *proc_name);
#endif