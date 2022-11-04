#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"
#include  "uthread.h"

/* semaphore struct containing semCount and semQueue as properties */
struct semaphore
{
    size_t semCount;
    queue_t semQueue;

};

/* sem_create() allocates memory to the new semaphore,
    then initializes it with its properties
*/
sem_t sem_create(size_t count)
{
    sem_t sem = (sem_t)malloc(sizeof(sem_t));
    sem->semCount = count;
    sem->semQueue = queue_create();

    return sem;
}

/* sem_destroy() does error management, then destroyes the queue associated to that
    sem and also deallocates the semaphore @sem
*/
int sem_destroy(sem_t sem)
{

    if (sem == NULL || (sem->semQueue != NULL))
    {
        return -1;
    }

    queue_destroy(sem->semQueue);
    free(sem);

    return 0;
}


int sem_down(sem_t sem)
{
    if (sem == NULL)
    {
        return -1;
    }
    if(sem->semCount == 0){
        queue_enqueue(sem->semQueue, uthread_current());
        uthread_block();
    }
    else if(sem->semCount > 0){
        sem->semCount--;
    }
    return 0;
}

int sem_up(sem_t sem)
{
    if (sem == NULL)
    {
        return -1;
    }
    if(sem->semCount == 0){
        struct uthread_tcb* currentThread;
        queue_dequeue(sem->semQueue, (void**)&currentThread);
        uthread_unblock(currentThread);
    }
    sem->semCount++;

    return 0;
}
