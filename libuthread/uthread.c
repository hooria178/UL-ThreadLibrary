#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

/* uthread TCB struct that contains its state, stack, func, arg, and context */
struct uthread_tcb
{ 
	int *threadState;
	void* stack;
    uthread_func_t threadFunc;
    void *threadArg;

	uthread_ctx_t *uctx;
    void* threadTopOfStack;
};

/* Have the readyQueue, blockedQueue, and zombieQueue be global so all threads have access to it
   and the struct runningThread be for the thread in the running state.
*/
struct uthread_tcb* runningThread;
queue_t readyQueue;
queue_t blockedQueue;
queue_t zombieQueue;


struct uthread_tcb *uthread_current(void)
{
	return runningThread;
}

/* uthread_yield() should dequeue the first thread in the ready queue as the newRunningThread,
    then enqueue the currently running thread (runningThread) in the readyQueue,
    and finally context switch between the latter(newRunningThread) and the former (oldRunningThread).
    */
void uthread_yield(void)
{
    
    queue_enqueue(readyQueue, runningThread);
    struct uthread_tcb* oldRunningThread = runningThread;
    queue_dequeue(readyQueue, (void**)(&runningThread));
    uthread_ctx_switch(oldRunningThread->uctx, runningThread->uctx);
}

/*  uthread_exit() enqueues currently running thread into zombieQueue,
    then dequeues a new running thread from readyQueue,
    and finally context switch between them.
*/
void uthread_exit(void)
{
    struct uthread_tcb* newRunningThread;

	queue_dequeue(readyQueue, (void**)(&newRunningThread));

	queue_enqueue(zombieQueue, (void*)(runningThread));

	struct uthread_tcb* oldRunningThread;
	oldRunningThread = runningThread;
	runningThread = newRunningThread;

	uthread_ctx_switch(oldRunningThread->uctx, runningThread->uctx);

}

/* uthread_create() first allocates memory for the new thread,
    then sets all the newThread's properties, then finally enqueus 
    the newThread into the readyQueue to be scheduled to run.
*/
int uthread_create(uthread_func_t func, void *arg)
{
	/* Initialize the new thread and its properties: */
	struct uthread_tcb* newThread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	newThread->threadFunc = func;
	newThread->threadArg = arg;
    newThread->stack = uthread_ctx_alloc_stack();
    newThread->uctx = malloc(sizeof(uthread_ctx_t));
    uthread_ctx_init(newThread->uctx, newThread->stack, newThread->threadFunc, newThread->threadArg);

    queue_enqueue(readyQueue, newThread);

    return 0;
    
}

/* uthread_run() is called in the main function, so it will intialize all the queues,
    then start preemption if preempt == true, then register so-far execution flow as the idleThread(?),
    then create the initialThread and set its properties through uthread_create(),
    then finally run an infinite loop which keeps yielding to the next available thread
    until there are no more threads to run in the readyQueue.
*/
int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
    //Initialize queues
    readyQueue = queue_create();
    blockedQueue = queue_create();
    zombieQueue = queue_create();


    if (preempt == true)
    {
        preempt_start(preempt);
    }

    /* First Action: Register so-far execution flow as idleThread */
    runningThread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
    runningThread->uctx = malloc(sizeof(uthread_ctx_t));

    /* Second Action: Create the initialThread as specified by the arguments of the function */
    uthread_create(func, arg);

    /* Third Action: Execute infinite loop where the main thread keeps 
        yielding until there are no more threads left in the readyQueue
    */
    while(1)
    {
        if (queue_length(readyQueue) <= 0) // if there are no more threads in the ready queue
        {
            return 0;
        }
        /* Yield to the next available thread */
        uthread_yield();
    }
}

/* uthread_block() dequeues a thread in the readyQueue and have it be the 
    newRunningThread, then it enqueues the currently running thread in the blockedQueue,
    then context switch!
*/
void uthread_block(void)
{
    struct uthread_tcb* newRunningThread;

	queue_dequeue(readyQueue, (void**)(&newRunningThread));

	queue_enqueue(blockedQueue, runningThread);

	struct uthread_tcb* assignNewRunningThread;
	assignNewRunningThread = runningThread;
	runningThread = newRunningThread;

	uthread_ctx_switch(assignNewRunningThread->uctx, newRunningThread->uctx);
}

/* uthread_unblock() dequeues the thread from blockedQueue,
    then enqueus it back in readyQueue to be scheduled.
*/
void uthread_unblock(struct uthread_tcb *uthread)
{
    queue_delete(blockedQueue, uthread);
    queue_enqueue(readyQueue, uthread);
}
