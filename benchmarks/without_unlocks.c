#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/stat.h>

#define NUM_THREADS 20
#define MAX_PRINTING 5

int threads_printing = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t print_cond = PTHREAD_COND_INITIALIZER;

void* print_thread_num_and_exit(void* thread_num) {
	int* num_ptr = (int*) thread_num;
	pthread_mutex_lock(&lock);
	while (threads_printing >= 5) {
		pthread_cond_wait(&print_cond, &lock);
	}
	threads_printing++;
	
	printf("Thread %d printing!\n", *num_ptr);
	sleep(1);
 	
	threads_printing--;
	pthread_cond_signal(&print_cond);
	pthread_mutex_unlock(&lock);
	
	return 0;
}


int main(int argc, char** argv) {
	pthread_t threads[NUM_THREADS];
	int tids[NUM_THREADS];
	struct timeval startt, stopt;
	
	printf("timing threads starting now!\n");
	// we only have 1 thread at the moment, so start the timer.
	gettimeofday(&startt, NULL);

	for (int i = 0; i < NUM_THREADS; i++) {
		tids[i] = i + 1;
		pthread_create(&threads[i], 0, print_thread_num_and_exit, &tids[i]);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], 0);
	}
	// back down to one thread, fetch time of day.
	gettimeofday(&stopt, NULL);
	printf("Program took %ld usecs\n", stopt.tv_usec - startt.tv_usec);
}

