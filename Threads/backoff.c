#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include "debug.h"


#define ITERATIONS 20 

pthread_mutex_t mutex[3] = {
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_INITIALIZER,
};

/* backoff is used to avoid Deadlock */
int backoff = 1;
int yield_flag = 0;


void *lock_forward(void *arg) 
{
	int i, iterations, backoffs, gotlocks;
	int status;

	status = pthread_detach(pthread_self());
	if (status != 0) {
		err_abort(status, "Detach Failed ");
	}

	for (iterations = 0; iterations < ITERATIONS; iterations++) {
	DPRINTF(("<-------------------Forward Iteration %d ------------------------->\n", i ))
		backoffs = 0;
		for (i = 0; i < 3; i++) {
			gotlocks = 1;
			if (i == 0 ) {
				status = pthread_mutex_lock(&mutex[i]);
				if (status != 0) {
					gotlocks = 0;
					err_abort(status, "Mutex Lock  Failed ");
				}
				DPRINTF(("Got Forward Lock  %d \n", i));
			} else {
				if (!backoff) {
					status = pthread_mutex_lock(&mutex[i]);
				} else {
					status = pthread_mutex_trylock(&mutex[i]);
				}
				if (status == EBUSY) {
					backoffs++
					DPRINTF(("Forward Lock %d Backing off at %d \n", i, backoffs));
					for (i--; i >= 0; i--) {
						status = pthread_mutex_unlock(&mutex[i]);
						if (status != 0) {
							err_abort(status, "Mutex UnLock Failure  ");
						}
					}
					gotlocks = 0;
					break;
				} else {
					if (status != 0) {
						gotlocks = 0;
						err_abort(status, "TryLock Mutex Failed ");
					} else if (status == 0) {
						DPRINTF(("Forward locker got %d \n", i));
					} else {
						err_abort(status, "Something I dont know ");
					}
				}
			}
		}

		/* I did not get lock and I need to do backoff */ 
		if (!gotlocks) {
			if (yield_flag) {
				sched_yield();
			} else {
				sleep(1);
			}
		}

		if (gotlocks) {
			printf(" I got all the forward lock and now I am unlocking , iterations :%d \n", iterations);
			/* order is taken care */
			for (int i = 2; i >= 0; i--)  {
				status = pthread_mutex_unlock(&mutex[i]);
				if (status != 0) {
					err_abort(status, "Mutex UnLock Issue ");
				} 
			}
			sched_yield();
		}
		DPRINTF(("<-------------------Forward Iteration Completed %d ------------------------->\n", i ))
	}
	return NULL;
}


void* lock_backward (void *arg) 
{
	int i, iterations, backoffs;
	int status, gotlocks;

	status = pthread_detach(pthread_self());
	if (status != 0) {
		err_abort(status, "Detach Failed ");
	}

	for (iterations = 0; iterations < ITERATIONS; iterations++) {
		DPRINTF(("<-------------------Backward Iteration %d ------------------------->\n", i ))
		backoffs = 0;
		for (i = 2; i >=0; i--) {
			gotlocks = 1;
			if (i == 2) {
				status = pthread_mutex_lock(&mutex[i]);
				if (status != 0) {
					err_abort(status, "Mutex Lock Failed ");
				}
			} else {
				if (backoff) {
					status = pthread_mutex_trylock(&mutex[i]);
				} else {
					status = pthread_mutex_lock(&mutex[i]);
				} 
				if (status == EBUSY) {
					backoffs++;
					DPRINTF(("TryLock Mutex %d  failed for %d times ", i, backoffs));
					for (i++; i < 3; i++) {
						status = pthread_mutex_unlock(&mutex[i]);
						if (status != 0) {
							err_abort(status, "Unlock Failed ");
							gotlocks = 0;
							break;
						}
						gotlocks = 0;
						break;
					} 
				} else {
					if (status != 0) {
						err_abort(status, "Mutex Lock failed ");
						gotlocks = 0;
						break;
					} else if (status == 0) {
						DPRINTF(("Got Backward Lock %d", i));
					} else {
						err_abort(status, "Something Nasty Happened ");
						gotlocks = 0;
						break;
					} 
				}
			}
		}
		if (!gotlocks) {
			if (backoff) {
				if (yield_flag > 0) {
					sched_yield();
				} else {
					sleep(2);
				}
			}
		}

		if (gotlocks) {
			printf(" I got all the backward lock and now I am unlocking , iterations :%d \n", iterations);
			/* order is taken care */
			for (int i = 0; i < 3; i++)  {
				status = pthread_mutex_unlock(&mutex[i]);
				if (status != 0) {
					err_abort(status, "Mutex UnLock Issue ");
				} 
			}
			sched_yield();
		}
		DPRINTF(("<-------------------Backward Iteration Completed %d ------------------------->\n", i ))
	}

	return NULL;

}


int main(int argc, char *argv[]) 
{
	pthread_t f_thread, b_thread;
	int status;

	if (argc > 1)  {
		backoff = atoi(argv[1]);
	} 

	if (argc > 2) {
		yield_flag = atoi(argv[2]);
	}

	/* Create Threads to simulate the deadlock */
	status = pthread_create(&f_thread, NULL, lock_forward, NULL);
	if (status != 0) {
		err_abort(status, "Pthread_create Failed ");
	}

	status = pthread_create(&b_thread, NULL, lock_backward, NULL);
	if (status != 0) {
		err_abort(status, "Pthread create Failed ");
	}

	pthread_exit(NULL);
	return 0;
}





