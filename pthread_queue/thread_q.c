
#include "thread_q.h"

/**
 * @brief	Init queue
 */ 
int thread_queue_init(thread_queue_t *queue, uint32_t *buffer, uint32_t size)
{
	if((queue == NULL) || (buffer == NULL)) {
		return -1;
	}
	
	queue->pBuff 	= buffer;
	queue->MaxSize	= size;
	queue->Size		= 0;
	queue->Head		= 0;
	queue->Tail		= 0;
	pthread_mutex_init(&queue->mutex, NULL);
	pthread_cond_init(&queue->cond, NULL);
	
	return 0;
}

/**
 * @brief	Deinit queue
 */ 
int thread_queue_deinit(thread_queue_t *queue)
{
	pthread_mutex_destroy(&queue->mutex);
	pthread_cond_destroy(&queue->cond);
	queue->pBuff 	= 0;
	queue->MaxSize	= 0;
	queue->Size		= 0;
	queue->Head		= 0;
	queue->Tail		= 0;
	return 0;
}

/**
 * @brief	Send an item to queue
 */ 
int thread_queue_send(thread_queue_t *queue, void *element)
{
	int ret = 0;
	uint32_t push_val = (uint32_t) element;
	
	pthread_mutex_lock(&queue->mutex);
	if(queue->Size < queue->MaxSize) {
		queue->pBuff[queue->Tail] = push_val;
		queue->Tail++;
		if(queue->Tail >= queue->MaxSize) {
			queue->Tail = 0;
		}
		queue->Size++;
		pthread_cond_signal(&queue->cond);
	}
	else {
		ret = -1;
	}
	pthread_mutex_unlock(&queue->mutex);
	
	return ret;
}

/**
 * @brief	Blocking receive queue item
 */ 
void *thread_queue_recv(thread_queue_t *queue, int *error)
{
	uint32_t ret = 0;
	int 	res = 0;
	
	pthread_mutex_lock(&queue->mutex);
	/**
	 *Recommendation of pthread_cond_wait use;
	 * Avoid of spurious wakeup signals 
	 */
	while(queue->Size == 0) {
		pthread_cond_wait(&queue->cond, &queue->mutex);
	}
		
	if(queue->Size != 0) {
		ret = queue->pBuff[queue->Head];
		queue->Head++;
		if(queue->Head >=  queue->MaxSize) {
			queue->Head = 0;
		}
		queue->Size--;
		if(queue->Size) {
			pthread_cond_signal(&queue->cond);
		}
	}
	else {
		res = -1;
		ret = 0;
	}
	pthread_mutex_unlock(&queue->mutex);
	
	*error = res;
	return (void*) ret;
}