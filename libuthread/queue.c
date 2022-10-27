#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct listItem {
	struct listItem *before;
	struct listItem *after;
	void *data;
};

struct queue {
	/* TODO Phase 1 */
	int length;
	struct listItem *first;
	struct listItem *last;
};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	return (queue_t)malloc(sizeof(struct queue));

}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if(queue)
	{
		free(queue);
		return 0;
	}
	return -1;
	
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if((queue == NULL) || (data == NULL))
	{
		return -1;
	}
	// If queue is empty, then new data item is both first and last item in queue
	if (queue->last == NULL)
	{
		queue->first->data = data;
		queue->last->data = data;
	}
	else // if queue is not empty, then add the data item at the back aka end of the queue
	{
		queue->last->data = *data;
		queue->last = queue->last->after;
	}
	queue->size++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if(!queue)
	{
		return -1;
	}
	*data = queue->first->data;
	queue->first = queue->first->after;
	return 0; 
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if(!queue)
	{
		return -1;
	}
	struct listItem *current = queue->first;
	while(current->data != *data)
	{
		if(!current->after)
		{
			return -1;
		}
		*current = current->after;
	}
	current->before->after = current->after;
	current->after->before = current->before;
	return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
	if(!queue || !func)
	{
		return -1;
	}
	return 0;
	
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	if(!queue)
	{
		return -1;
	}
	return queue->length;
}

