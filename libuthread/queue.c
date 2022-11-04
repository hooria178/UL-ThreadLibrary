#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"
#include "private.h"


struct listItem {
	struct listItem *before;
	struct listItem *after;
	void *data;
};

struct queue {
	int length;
	struct listItem *first;
	struct listItem *last;
};

queue_t queue_create(void)
{
	queue_t q = (queue_t)malloc(sizeof(struct queue));
  q->first = NULL;
  q->last = NULL;
  q->length = 0;
  return q;
}

int queue_destroy(queue_t queue)
{
	if(!queue || queue->length != 0)
	{
		return -1;
	}
  	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
  struct listItem* newItem = (struct listItem*)malloc(sizeof(struct listItem));
  if(!queue || !data || !newItem)
	{
		return -1;
	}
  newItem->data = data;
  newItem->before = queue->last;
  newItem->after = NULL;
  if(queue->last)
  {
	queue->last->after = newItem;
  }
  queue->last = newItem;
  if(!queue->first){
    queue->first = newItem;
  }
  queue->length++;
  return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if(!queue || !data || queue->length == 0)
	{
		return -1;
	}
	*data = queue->first->data;
	queue->first = queue->first->after;
  	queue->length--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if(!queue)
	{
		return -1;
	}
	struct listItem *current = queue->first;
	while(current->data != data)
	{
		if(!current->after)
		{
			return -1;
		}
		current = current->after;
	}

	if(current->before)
	{
		current->before->after = current->after;
	}
	else
	{
		queue->first = current->after;
	}

	if(current->after)
	{
		current->after->before = current->before;
	}
	else
	{
		queue->last = current->before;
	}
  	queue->length--;
	return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if(!queue || !func)
	{
		return -1;
	}
	struct listItem* current = queue->first;
	while(current)
	{
		void* funcdata = current->data;
		func(queue, funcdata);
		current = current->after;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	if(!queue)
	{
		return -1;
	}
	return queue->length;
}

void queue_print(queue_t queue)
{
  printf("Queue[%d]:\n", queue->length);
  struct listItem *current = queue->first;
  int i = 0;
  while(current)
  {
    printf("[%d]: %d\n", i, *(int*)current->data);
    i++;
    current = current->after;
  }
}
