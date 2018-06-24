#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commons.h"


static int periph_init()
{
    return 0;
}

static void keeper_start()
{
    
    pid_t keeper_pid;

    keeper_pid = fork();
    switch(keeper_pid) {
        case -1:
            printf("Fork keeper error!\n");
            break;

        case 0: 
            execl(KEEPER_BIN_NAME, KEEPER_BIN_NAME, NULL);
            printf("Execl error!\n");
            break;

        default: 
            wait(NULL);
            break;
    }
}

int main(int argc, char const *argv[])
{
    int result = periph_init();

    if(result == 0) {
        keeper_start();
    }
    else {
        printf("Init error, stop!\n");
    }

    printf("initapp exit\n");
    return 0;
}
