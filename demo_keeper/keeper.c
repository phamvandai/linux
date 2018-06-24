#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commons.h"
#include "linuxutils.h"


static int child_id = 0;
static void handler(int signum)
{
    printf("keeper sighandler: PID:%d, CHLDID:%d, SIG:%d\n",
          getpid(), child_id, signum);
    kill(child_id, SIGKILL);
    exit(-1);
}

int main(int argc, char const *argv[])
{
    if(LinuxUtils_LockInstance("/tmp/keeper_lock", "keeper") != 0) {
        exit(-1);
    }
    
    struct sigaction sa;
    pid_t fork_id;
    int retval;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler   = &handler;
    sa.sa_flags     = SA_SIGINFO;

    if(sigaction(SIGHUP, &sa, NULL) == -1 ) {
        perror("Cannot handle SIGHUP\n");
        exit(-1);
    }

    if(sigaction(SIGINT, &sa, NULL) == -1 ) {
        exit(-1);
        perror("Cannot handle SIGINT\n");
    }

    if(sigaction(SIGTERM, &sa, NULL) == -1 ) {
        exit(-1);
        perror("Cannot handle SIGINT\n");
    }

    _fork:
    fork_id = fork();
    int fd;
    switch(fork_id) {
        case -1: 
            printf("fork mainapp failed\n");
            break;

        default: 
            child_id = fork_id;
            printf("mainapp: %d started\n", child_id);
            child_id = wait(&retval);
            printf("mainapp terminated(%d)\n", retval);
            sleep(5);
            goto _fork;
            break;

        case 0: 
            // fd = open("mainapp.out", O_WRONLY | O_CREAT, 0666);
            // dup2(fd, 1);
            execl(MAINAPP_BIN_NAME, MAINAPP_BIN_NAME, (char*) MAINAPP_ARG, NULL);
            printf("exec mainapp failed\n");
            break;            
    }

    printf("Keeper exit\n");
    return 0;
}
