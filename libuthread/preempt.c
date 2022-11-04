#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

/* Creates an instance of struct sigaction called preemptSignal*/
struct sigaction preemptSignal;

/* Block signals of type SIGVTALRM */
void preempt_disable(void)
{
    sigprocmask(SIG_BLOCK, &preemptSignal.sa_mask, NULL);
}

/* Unblock signals of type SIGVTALRM */
void preempt_enable(void)
{
    sigprocmask(SIG_UNBLOCK, &preemptSignal.sa_mask, NULL);
}

/* The alarm function that sets the virtual-time timer to
    expire in 10000 seconds. */
unsigned int signalAlarm(void)
{
    struct itimerval old, new;
    new.it_interval.tv_usec = 0;
    new.it_interval.tv_sec = 10000;
    new.it_value.tv_usec = 0;
    new.it_value.tv_sec = 10000;
    if (setitimer(ITIMER_VIRTUAL, &new, &old) < 0)
        return 0;
    else
        return old.it_value.tv_sec;
}

/* preempt_start() executes all the function inside only if preempt == true,
    then installs a signal handler that receives alarm signals, 
    then force currently running thread to yield, then to the instance of sigaction, 
    add virtual alarm , then the timer will fire a virtual alarm at frequency of 
    100 HZ.
*/
void preempt_start(bool preempt)
{

    if (preempt == true)
    {
        /* Installs a signal handler that receives alarm signals (of type SIGVTALRM) */
        sigemptyset(&preemptSignal.sa_mask);
        sigaddset(&preemptSignal.sa_mask, SIGVTALRM);

        /* Force the currently running thread to yield */
        preempt_enable();
        uthread_yield();

        /* To the instance of sigaction, add virtual alarm (SIGVTALRM) */
        sigaction(SIGVTALRM, &preemptSignal, 0);

        /* A timer that fires a virtual alarm at frequency of 100 Hz*/
        signalAlarm();
    }
}

void preempt_stop(void)
{
    /* 
        Restore previous timer configuration, and previous action associated to
        virtual alarm signals.
    */
}
