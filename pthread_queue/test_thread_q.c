#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "thread_q.h"

#define QSIZE   128
uint32_t buffer[QSIZE];
thread_queue_t queue;

void *send_thread(void *arg)
{
    thread_queue_t *q = (thread_queue_t*) arg;
    int i = 0, res;

    while(1) {
        res = thread_queue_send(q, (void*) i);
        if(res) {
            printf("<--send %d error\n", i);
        }
        else {
            printf("<--send %d\n", i);
        }
        
        i += 5;
        if(i % 100 == 0) {
            sleep(1);
        }
        else {
            usleep(2000);
        }
    }
}


void *recv_thread(void *arg)
{
    thread_queue_t *q = (thread_queue_t*) arg;
    int res, val;

    sleep(1);

    while(1) {
        val = (int) thread_queue_recv(q, &res);
        if(res) {
            printf("-->recv error!\n");
        }
        else {
            printf("-->recv %d\n", val);
        }
    }
}


int main(int argc, char *argv[])
{
    pthread_t recv_id, send_id;
    int res;

    thread_queue_init(&queue, buffer, QSIZE);

    res = pthread_create(&send_id, NULL, send_thread, &queue);
    if(res) {
        printf("Create thread error\n");
        exit(-1);
    }

    res = pthread_create(&recv_id, NULL, recv_thread, &queue);
    if(res) {
        printf("Create thread error\n");
        exit(-1);
    }

    printf("Main send...\n");
    int i = 0;
    while(1) {
        res = thread_queue_send(&queue, (void*) i);
        if(res) {
            printf("_send %d error\n", i);
        }
        else {
            printf("_send %d\n", i);
        }
        
        i += 3;
        usleep(1000);
    }

    pthread_join(&send_id, &i);
    pthread_join(&recv_id, &i);
    thread_queue_deinit(&queue);
    return 0;
}