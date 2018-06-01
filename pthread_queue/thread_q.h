#ifndef THREAD_Q
#define THREAD_Q

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <stdint.h>

typedef struct {
	uint32_t	*pBuff;
	uint32_t	MaxSize;
	uint32_t	Size;
	uint32_t	Head;
	uint32_t	Tail;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} thread_queue_t;

int thread_queue_init(thread_queue_t *queue, uint32_t *buffer, uint32_t size);
int thread_queue_deinit(thread_queue_t *queue);
int thread_queue_send(thread_queue_t *queue, void* element);
void *thread_queue_recv(thread_queue_t *queue, int *error);

#ifdef __cplusplus
}
#endif

#endif