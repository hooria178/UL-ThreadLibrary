#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore
{
    /* TODO Phase 3 */
    size_t count;
};

sem_t sem_create(size_t count)
{
    /* TODO Phase 3 */
    semt_t semaphore sem = malloc(sizeof(struct semaphore)); // multiply by count?
    return *semaphore;
}

int sem_destroy(sem_t sem)
{
    /* TODO Phase 3 */
    free(sem);
    if (!free(sem))
    {
        return -1;
    }
    return 0;
    /* Error management: return -1 if @sem is NULL
    or if other threads are still being blocked on @sem;
    return 0 if sem was successfully destroyed*/
}

int sem_down(sem_t sem)
{
    /* TODO Phase 3 */
    spinlock_lock(sem->lock);
    while ((sem->count) == 0)
    {
        /*Block self            */
        /* ...                  */
        /* Call uthread_block() */
    }
    sem->count -= 1;
    spinlock_unlock(sem->lock);
    /* Error management: return -1 if @sem is NULL; return 0 if sem was successfully taken*/
}

int sem_up(sem_t sem)
{
    /* TODO Phase 3 */
    spinlock_lock(sem->lock);
    sem->count += 1;
    /* Wake up first in line */
    /* (if any)             */
    /* ...                  */
    /* Call uthread_unblock() */
    spinlock_unlock(sem->lock);
    /* Error management: return -1 if @sem is NULL; return 0 if sem was successfully released*/
}
/* go to the lectures recordings regarding synchronization
 and specifically focus on the count part and the block part of slide 20 and 21.*/
