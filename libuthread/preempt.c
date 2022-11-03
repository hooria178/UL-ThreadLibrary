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

struct sigaction preemptSignal;

void preempt_disable(void)
{
    /* TODO Phase 4 */
    /* Block signals of type SIGVTALRM */
    sigprocmask(SIG_BLOCK, &preemptSignal.sa_mask, NULL);
}

void preempt_enable(void)
{
    /* TODO Phase 4 */
    /* Unblock signals of type SIGVTALRM */
    sigprocmask(SIG_UNBLOCK, &preemptSignal.sa_mask, NULL);
}

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

void preempt_start(bool preempt)
{
    /* TODO Phase 4 */

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
    /* TODO Phase 4 */
    /* 
        Restore previous timer configuration, and previous action associated to
        virtual alarm signals.
    */
}

/* NEED TO FINISH PREEMPT_STOP() FUNCTION */
