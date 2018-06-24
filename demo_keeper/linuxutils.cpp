#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <stdio.h>
#include <stdlib.h>
#include "linuxutils.h"
#include <sys/socket.h>
#include <sys/un.h>


int LinuxUtils_LockInstance(const char *lock_file, const char *proc_name)
{
    if(lock_file == NULL) {
        return -1;
    }

    int pid_file = open(lock_file, O_CREAT | O_RDWR, 0666);
    int rc = flock(pid_file, LOCK_EX | LOCK_NB);

    if(rc) {
        if(EWOULDBLOCK == errno) {
            if(proc_name != NULL) {
                printf("%s - ", proc_name);
            }
            printf("another instance running\n");
            return -1;
        }
        else {
            if(proc_name != NULL) {
                printf("%s - ", proc_name);
            }
            printf("lock errno %d\n", errno);
            return -2;
        }
    }

    return 0;
}


