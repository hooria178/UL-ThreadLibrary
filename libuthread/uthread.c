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
	return 0;
    /* I DON'T KNOW WHAT GOES IN HERE */
}

/* uthread_yield() should dequeue the first thread in the ready queue as the newRunningThread,
    then enqueue the currently running thread (runningThread) in the readyQueue,
    and finally context switch between the latter(newRunningThread) and the former (oldRunningThread).
    */
void uthread_yield(void)
{
	struct uthread_tcb* newRunningThread;

	// if(!readyQueue)
	// {
	// 	return -1;
	// }

	queue_dequeue(readyQueue, (void**)(&newRunningThread));

	queue_enqueue(readyQueue, (void*)(runningThread));
	struct uthread_tcb* oldRunningThread;
	oldRunningThread = runningThread;
	runningThread = newRunningThread;

	uthread_ctx_switch(oldRunningThread->uctx, runningThread->uctx);
	
}

/*  uthread_exit() enqueues currently running thread into zombieQueue,
    then dequeues a new running thread from readyQueue,
    and finally context switch between them.
*/
void uthread_exit(void)
{
    struct uthread_tcb* newRunningThread;

	// if(!readyQueue)
	// {
	// 	return -1;
	// }

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
    /* TODO Phase 2 */

	/* Initialize the new thread and its properties: */
	struct uthread_tcb* newThread = malloc(sizeof(struct uthread_tcb));
	newThread->threadFunc = func;
	newThread->threadArg = arg;
    newThread->threadTopOfStack = uthread_ctx_alloc_stack();
    uthread_ctx_init(newThread->uctx, newThread->threadTopOfStack, newThread->threadFunc, newThread->threadArg);

    /*  the creation of a new thread should
        enqueue its TCB at the end of the ready queue.*/
    queue_enqueue(readyQueue, (void*)(newThread));

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
    /* TODO Phase 2 */

    //Initialize queues
    readyQueue = queue_create();
    blockedQueue = queue_create();
    zombieQueue = queue_create();


    if (preempt == true)
    {
        preempt_start(preempt);
    }

    /* First Action: Register so-far execution flow as idleThread */
    
    // register so-far single execution flow of the application
    // thread that the library can later schedule for execution like any other threads
    struct uthread_tcb* idleThread = malloc(sizeof(struct uthread_tcb));
    idleThread->threadFunc = func;
    idleThread->threadArg = arg;
    idleThread->threadTopOfStack = uthread_ctx_alloc_stack();
    // uthread_ctx_init(idleThread->uctx, idleThread->threadTopOfStack, idleThread->threadFunc, idleThread->threadArg);

    /* Second Action: Create the initialThread as specified by the arguments of the function */
    // NEED TO PROPERLY CREATE AN INITIAL THREAD
    struct uthread_tcb* initialThread = uthread_create(func, arg);
    /* DONT KNOW WHAT TO DO WITH THE INITIAL THREAD */

    /* Third Action: Execute infinite loop which:
        1. Stops the idle loop and returns if there are no more threads in the readyQueue
        2. Yields to the next available thread in readyQueue
        3. (Optional?) Deal with threads that reach completion and destroys their TCB
    */
    /* NEED TO FINISH THIS LOOP */
    while (1)
    {
        if (!readyQueue) // if there are no more threads in the ready queue
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
    /* TODO Phase 3 */
    /* context switch function */
    /* change the state and yielding to the next available thread.*/
    struct uthread_tcb* newRunningThread;

	queue_dequeue(readyQueue, (void**)(&newRunningThread));

	queue_enqueue(blockedQueue, (void*)(runningThread));

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
    /* TODO Phase 3 */
    /*
    * uthread_unblock - Unblock thread
    * @uthread: TCB of thread to unblock
    */
	queue_dequeue(blockedQueue, (void**)(&uthread));
	queue_enqueue(readyQueue, (void*)(uthread));
}

/* GETTING TWO ERRORS WHEN COMPILING IN THE SECTION OF UTHREAD_RUN() FUNCTION */
/* NEED TO FINISH THE UTHREAD_CURRENT() FUNCTION */
