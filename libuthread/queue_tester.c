#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

int main(void)
{
  /*
  0: enqueue
  1: dequeue
  2: delete
  3: iterate
  */
  queue_t queue = queue_create();
  int command;
  while(1)
  {
    queue_print(queue);
    printf("Enter command: ");
    scanf("%d", &command);
    if(command == 0)
    {
        int* d = (int*)malloc(sizeof(int));
        printf("Enter data to enqueue: ");
        scanf("%d", d);
        queue_enqueue(queue, d);
        //free(d);
    }
    else if(command == 1)
    {
        void** d = (void**)malloc(sizeof(void*));
        queue_dequeue(queue, d);
        printf("Dequeued value %d\n", **(int**)d);
        //free(d);
    }
    else if(command == 2)
    {
        int* d = (int*)malloc(sizeof(int));
        printf("Enter data to delete: ");
        scanf("%d", d);
        void* p = queue->first->data;
        int flag = queue_delete(queue, p);
        if(flag == -1){printf("ERROR");}
        //free(d);
    }
    else
    {
        break;
    }
  }

	return 0;
}
