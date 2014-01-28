/* 
 * File:   osxGetTime.h
 * Author: russ
 *
 * Created on November 2, 2011, 10:04 AM
 */

#ifndef OSXGETTIME_H
#define	OSXGETTIME_H

#ifdef __APPLE__

#pragma weak clock_gettime

#include <sys/time.h>
#include <sys/resource.h>
#include <mach/mach.h>
#include <mach/clock.h>
#include <mach/mach_time.h>
#include <errno.h>
#include <unistd.h>
#include <sched.h>

typedef enum {
	CLOCK_REALTIME,
	CLOCK_MONOTONIC,
	CLOCK_PROCESS_CPUTIME_ID,
	CLOCK_THREAD_CPUTIME_ID
} clockid_t;

static mach_timebase_info_data_t __clock_gettime_inf;

struct timespec clock_gettime(clockid_t clk_id, struct timespec *tp) {
	kern_return_t ret;
	clock_serv_t clk;
	clock_id_t clk_serv_id;
	mach_timespec_t tm;

	uint64_t start, end, delta, nano;

	task_basic_info_data_t tinfo;
	task_thread_times_info_data_t ttinfo;
	mach_msg_type_number_t tflag;

	int retint = -1;
	switch (clk_id) {
		case CLOCK_REALTIME:
		case CLOCK_MONOTONIC:
			clk_serv_id = clk_id == CLOCK_REALTIME ? CALENDAR_CLOCK : SYSTEM_CLOCK;
			if (KERN_SUCCESS == (ret = host_get_clock_service(mach_host_self(), clk_serv_id, &clk))) {
				if (KERN_SUCCESS == (ret = clock_get_time(clk, &tm))) {
					tp->tv_sec = tm.tv_sec;
					tp->tv_nsec = tm.tv_nsec;
					retint = 0;
				}
			}
			if (KERN_SUCCESS != ret) {
				errno = EINVAL;
				retint = -1;
			}
			break;
		case CLOCK_PROCESS_CPUTIME_ID:
		case CLOCK_THREAD_CPUTIME_ID:
			start = mach_absolute_time();
			if (clk_id == CLOCK_PROCESS_CPUTIME_ID) {
				getpid();
			} else {
				sched_yield();
			}
			end = mach_absolute_time();
			delta = end - start;
			if (0 == __clock_gettime_inf.denom) {
				mach_timebase_info(&__clock_gettime_inf);
			}
			nano = delta * __clock_gettime_inf.numer / __clock_gettime_inf.denom;
			tp->tv_sec = nano * 1e-9;
			tp->tv_nsec = nano - (tp->tv_sec * 1e9);
			retint = 0;
			break;
		default:
			errno = EINVAL;
			retint = -1;
	}
	struct timespec retval;
	retval.tv_sec = (int)retint / 1000000000;
	retval.tv_nsec = (long)retint - retval.tv_sec;
	return retval;
}

/* Identifier for system-wide realtime clock.  */
#define CLOCK_REALTIME 0
/* Monotonic system-wide clock.  */
#define CLOCK_MONOTONIC 1
/* High-resolution timer from the CPU.  */
#define CLOCK_PROCESS_CPUTIME_ID 2
/* Thread-specific CPU-time clock.  */
#define CLOCK_THREAD_CPUTIME_ID 3

#endif // __APPLE__

#endif	/* OSXGETTIME_H */
