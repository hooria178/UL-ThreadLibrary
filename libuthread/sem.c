#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore
{
    /* TODO Phase 3 */
    size_t semCount;
    queue_t semQueue;
    //sem->lock ?
};

sem_t sem_create(size_t count)
{
    /* TODO Phase 3 */
    sem_t sem = (sem_t)malloc(sizeof(sem_t)); // multiply by count?
    sem->semCount = count;
    sem->semQueue = queue_create();

    return sem;
}

int sem_destroy(sem_t sem)
{
    /* TODO Phase 3 */
    if (sem == NULL || (sem->semQueue != NULL))
    {
        return -1;
    }

    queue_destroy(sem->semQueue);
    free(sem);

    // if (!free(sem))
    // {
    //     return -1;
    // }

    return 0;
    /* Error management: return -1 if @sem is NULL
    or if other threads are still being blocked on @sem;
    return 0 if sem was successfully destroyed*/
}

int sem_down(sem_t sem)
{
    /* TODO Phase 3 */
    if (sem == NULL)
    {
        return -1;
    }
    spinlock_lock(sem->lock);
    while ((sem->count) == 0)
    {
        /*Block self            */

        /* ...                  */
        /* Call uthread_block() */
        uthread_t currentThread = uthread_current();
        queue_enqueue(sem->semQueue, (void*)currentThread);
        uthread_block();
    }
    sem->count -= 1;
    spinlock_unlock(sem->lock);
    return 0;
    /* Error management: return -1 if @sem is NULL; return 0 if sem was successfully taken*/
}

int sem_up(sem_t sem)
{
    /* TODO Phase 3 */
    if (sem == NULL)
    {
        return -1;
    }
    spinlock_lock(sem->lock);
    sem->count += 1;
    /* Wake up first in line */
    /* (if any)             */
    /* ...                  */
    /* Call uthread_unblock() */
    if(sem->semQueue != NULL)
    {
        uthread_t currentThread;
        queue_dequeue(sem->semQueue, (void**)&currentThread);
        uthread_unblock(currentThread);
    }
    spinlock_unlock(sem->lock);
    return 0;
    /* Error management: return -1 if @sem is NULL; return 0 if sem was successfully released*/
}
/* go to the lectures recordings regarding synchronization
 and specifically focus on the count part and the block part of slide 20 and 21.*/


/* NEED TO FIGURE OUT THE SPINLOCK DEAL AND SO BASICALLY SEM_UP and SEM_DOWN functions*/
