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


struct uthread_tcb
{
	int *threadID; 
	int *threadState;
	void* stack;
    uthread_func_t threadFunc;
    void *threadArg;

	uthread_ctx_t *uctx;
    void* threadTopOfStack;
};


/* Create variables to track the id being assigned to the threads */
int threadCount = 0;

/* Have the readyQueue, blockedQueue, and zombieQueue be global so all threads have access to it
   and the struct runningThread be for the thread in the running state.
*/
struct uthread_tcb* runningThread;
queue_t readyQueue;
queue_t blockedQueue;
queue_t zombieQueue;


struct uthread_tcb *uthread_current(void)
{
	return runningThread->threadID;
}

void uthread_yield(void)
{
    /* TODO Phase 2 */
    /* yield() should dequeue the first thread in the ready queue,
    enqueue the currently running thread,
    and context switch between the latter and the former.
    */

	struct uthread_tcb* newRunningThread;

	if(!readyQueue)
	{
		return -1;
	}

	queue_dequeue(readyQueue, (void**)(&newRunningThread));

	queue_enqueue(readyQueue, (void*)(runningThread));
	struct uthread_tcb* assignNewRunningThread;
	assignNewRunningThread = runningThread;
	runningThread = newRunningThread;

	uthread_ctx_switch(&assignNewRunningThread->uctx, &newRunningThread->uctx);
	
}

void uthread_exit(void)
{
    struct uthread_tcb* newRunningThread;

	// if(!readyQueue)
	// {
	// 	return -1;
	// }

	queue_dequeue(readyQueue, (void**)(&newRunningThread));

	queue_enqueue(zombieQueue, (void*)(runningThread));

	struct uthread_tcb* assignNewRunningThread;
	assignNewRunningThread = runningThread;
	runningThread = newRunningThread;

	uthread_ctx_switch(&assignNewRunningThread->uctx, &newRunningThread->uctx);

}

int uthread_create(uthread_func_t func, void *arg)
{
    /* TODO Phase 2 */

	/* Initialize the new thread and its properties: */
	struct uthread_tcb newThread = (uthread_tcb*)malloc(sizeof(uthread_tcb));
	newThread->threadFunc = func;
	newThread->threadArg = arg;
	threadCount++;
	newThread->threadID = threadCount;
    newThread->threadTopOfStack = uthread_ctx_alloc_stack();
    uthread_ctx_init(newThread->uctx, newThread->threadTopOfStack, newThread->threadFunc, newThread->threadArg);

    /*  the creation of a new thread should
        enqueue its TCB at the end of the ready queue.*/
    queue_enqueue(readyQueue, (void*)(newThread));
    
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
    /* TODO Phase 2 */

    //Initialize queues
    readyQueue = queue_create();
    blockedQueue = queue_create();
    zombieQueue = queue_create();


    if (preempt == true)
    {
        preempt_start();
    }

    /* First Action: Register so-far execution flow as idleThread */
    
    // register so-far single execution flow of the application
    // thread that the library can later schedule for execution like any other threads
    struct uthread_tcb* idleThread = (uthread_tcb*)malloc(sizeof(uthread_tcb));
    idleThread->threadFunc = func;
    idleThread->threadArg = arg;
    idleThread->threadID = threadCount;
    idleThread->threadTopOfStack = uthread_ctx_alloc_stack();
    // uthread_ctx_init(idleThread->uctx, idleThread->threadTopOfStack, idleThread->threadFunc, idleThread->threadArg);

    /* Second Action: Create the initialThread as specified by the arguments of the function */
    // create a new thread, the initial thread, as specified by the arguments of the function
    struct uthread_tcb* initialThread = uthread_create(uthread_func_t func, void *arg);

    /* Third Action: Execute infinite loop which:
        1. Stops the idle loop and returns if there are no more threads in the readyQueue
        2. Yields to the next available thread in readyQueue
        3. (Optional?) Deal with threads that reach completion and destroys their TCB
    */


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

	uthread_ctx_switch(&assignNewRunningThread->uctx, &newRunningThread->uctx);
}

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
