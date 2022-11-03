#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"


/* TCB contains the context of each thread (stack, registers etc.). contexts_fixed_schedule.c shows you how to use ucontext*/
/* Create variables to track the initialization of the queues and the id being assigned to the threads */

struct uthread_tcb
{
    /* TODO Phase 2 */
	/*
		threadID == unique identifier assigned by OS when created
		threadState == state of the thread as it progresses through the system
		stack == contains all the CPU information?
		func ==  function to be executed by the thread
		arg == argument to be passed to the thread

		uctx == thread's pointer to the tcb?
		prev == pointer to the process that triggered the creation of this thread?
		next == pointer to the thread(s) created by this thread?

	*/
	int *threadID; 
	int *threadState;
	void* stack;
    uthread_func_t threadFunc;
    void *threadArg;

	uthread_ctx_t *uctx;
};

/* Have the readyQueue, blockedQueue, and zombieQueue be global so all threads have access to it
   and the runningThread be for the thread in the running state.
*/

int threadCount = 0;
struct uthread_tcb* runningThread;
queue_t readyQueue;
queue_t blockedQueue;
queue_t zombieQueue;
int queuesSet = 0;


struct uthread_tcb *uthread_current(void)
{
    /* TODO Phase 2/3 */
}

void uthread_yield(void)
{
    /* TODO Phase 2 */
    /* yield() should dequeue the first thread in the ready queue,
    enqueue the currently running thread,
    and context switch between the latter and the former.
    */
}

void uthread_exit(void)
{
    /* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
    /* TODO Phase 2 */
	/* Intialize queues if not already done! */
    if (queuesSet == 0)
    {
        readyQueue = queue_create();
		blockedQueue = queue_create();
		zombieQueue = queue_create();

		queuesSet = 1;

		//preempt_enable();
		// idleThread? aka the main thread?
		runningThread = (uthread_tcb*)malloc(sizeof(uthread_tcb));
		//Error management: Memory Allocation
		if(runningThread == NULL)
		{
			return -1;
		}
		//preempt_disable();
    }

	/* Initialize the new thread and its properties: */
	struct uthread_tcb newThread = (uthread_tcb*)malloc(sizeof(uthread_tcb));
	newThread->threadFunc = func;
	newThread->threadArg = arg;
	threadCount++;
	newThread->threadID = threadCount;
	
	
    /*  the creation of a new thread should
        enqueue its TCB at the end of the ready queue.*/
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
    /* TODO Phase 2 */
    /* First Action: Register so-far execution flow as idleThread */
    preempt_enable();
    // register so-far single execution flow of the application
    // thread that the library can later schedule for execution like any other threads
    preempt_disable()

        /* Second Action: Create the initialThread as specified by the arguments of the function */
        // create a new thread, the initial thread, as specified by the arguments of the function
        struct uthread_tcb *initialThread = uthread_create(uthread_func_t func, void *arg);

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
}

void uthread_unblock(struct uthread_tcb *uthread)
{
    /* TODO Phase 3 */
}
