# UTHREAD: A user-level thread library

## Summary
This set of files called 'libuthread' implements a library that controls the
creation and scheduling of concurrent threads. The library is designed to
schedule in a round-robin fashion and use semaphores to prevent race conditions
and allow for more protection for resources shared among different threads.

## Implementation
The implementation of this program follows four distinct steps:
1. The Queue
Our queue made use of a linked list structure and used a struct called
'listItem' as its nodes. We implemented an overall struct called 'queue'
containing pointers to both the first and last listItem nodes and an integer
called 'length' that stored the total number of nodes stored in the queue. Each
listItem node also held three variables: a pointer to the node before it, a
pointer to the node after it, and a void pointer to some data defining the
node. When creating a new queue, the proper amount of memory is allocated,
the first and last pointers are set to NULL, and the length is set to zero.
Upon destruction of the queue, the aforementioned allocated memory is freed.
When enqueueing a new listItem, memory is allocated for it and the data is
assigned. Regardless of the situation the new node's after pointer will always
be initially set to NULL, as the queue's structure is FIFO. The before pointer
is set to the last node in the queue. If this item was the first item to be put
in the queue, this would still be appropriate as the before variable would then
be set to NULL, which would confirm that there is no other data before it. On
the queue's side, the last pointer is always set to the new node. The first
pointer is set only if a different first node doesnt yet exist. In this case,
that would mean the new node is the only one in the queue and that it is both
the first and the last node. When dequeuing, data is extracted from the first
pointer in the queue and the new first is set to the old first's after pointer.
When deleting a specified piece of data in the queue, we create a 'current'
pointer that iterates through the queue, following each node's after until it
finds the correct data. If it never finds this data, it returns -1, but if it
does, it manipulates the nodes infront of and behind it to point toward one
one another, essentially erasing the node from the queue. The iterate function
uses the same method of 'current' and runs the given function on each node's
data. Finally, the length function simply returns the lenght of the given
queue. 
2. Uthread
Our uthread file starts with the declaration of a 'uthread_tcb' struct. This
struct holds all the necessary data for each individual thread and includes the
thread's state, stack, function, arguments, and context. Our global variables
consist of a pointer to the current running thread, and three queues: a ready,
blocked, and zombie queue. The uthread_current function simply returns the
address of the runningThread variable. When a new thread is created, memory is
allocated for it, its variables are initialized, and it is entered into the
ready queue. The uthread_run function acts as the main organizer of the
threads. This function initializes each queue, checks for preemtion, and saves
the current context as a thread, the idle thread. Then, the passed function and
arguments are used to create the first thread to go into the ready queue. The
program then loops infinitely until the ready queue is empty. Otherwise, the
idle thread yields to all other threads until there are no more remaining
threads. When a thread yields, it gives up its place as the running thread and
re-enters the back of the ready queue. The new running thread is then dequeued
from the ready queue and the contexts are switched. In the case of the idle
thread, whenever it is dequeued and given the spotlight as the running thread,
it yields and re-enters the queue until it is the only thread remaining. When a
thread finishes, it exits and enters the zombie thread queue, and the next
running thread is dequeued from the the ready queue. The functions block and
unblock are mostly for use with semaphores. When a thread is blocked, it is
sent to the blocked queue to wait for access to a blocked resource and a new
thread from the ready thread queue takes its place as the running thread. When
the resource becomes available, the queue's delete function is used to find
the appropriate blocked thread, remove it from the blocked thread queue, and
put it back into the ready thread queue to be run.
3. Semaphores
Our semaphore uses a semCount to keep track of the number of resources that are
available and the semQueue queue holds a list of the threads that are in line
to use the resource. When a thread calls sem_down and the semCount is above 0,
the count is simply lowered by one. However, when there are no more resources
to spare and the count is 0, the queue is added to the semaphore's waitlist
queue and uthread_block is called. When a thread calls sem_up and releases a
resource, and if the count is zero, the first thread in the semQueue is
released and is sent to the uthread_unblock function, where it is placed back
into the ready queue to resume execution.  
4. Preempt
Our preempt.c file first creates an instance of sigaction to establish a 
signal handler. The functions preempt_disable() and preempt_enable() block
and unblock the signal the signal handler receives, respectively. Then we
created a timer function which fires an alarm a hundred times per second.
The function preempt_start() will execute all of the commands inside only
if the preempt flag is marked as "true". The commands start from installing
a signal handler which receives alarm signals of type SIGVTALRM, then it will
force the currently running thread to yield to the next available thread in
readyQueue, then the sigaction() function adds virtual alarm to the signal
handler, and finally we trigger the alarm which goes off at frequency of 
100 Hz.

## Testing
For our queue, we designed a program to allow us to manually enter integers
into the queue and see its results. We designed a queue_print function to print
the contents of the queue at each step, so we could see the effect of different
cases. With this, we manually tested the queue, including edge cases and any
other cases we found useful. For uthread, we used the provided test cases and
examples from lecture slides and homework quizes to check the accuracy of our
threads. We used a similar testing mechanism for sem and preempt, though we
weren't able to test the latter as extensively as the queue and uthread.


## Sources

The only code copied directly from any source was code provided to us in
lectures and lecture slides. We did, however, use internet sources to advise 
our code in different places. Most of the content we found to be useful came
from Geeksforgeeks.org, tutorialspoint.com, and the GNU C Library (gnu.org)


## Important Info About the Code

Our current code is not fully operational. However, the queue and uthread
components seem to be working pretty well. The sem portion works in a few cases
but isn't fully accurate in all situations. We didn't get time to implement
preempt_stop or test the full capabilities of preempt. 
## 
