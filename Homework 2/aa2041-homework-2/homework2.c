
// Ashwin Anand RUID: 192007894 Net Id: aa2041

// Packages needed for program to run
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

// Global sum variable that can be referenced in all methods
int global_sum = 0;

// locks pthread for changes
pthread_mutex_t lck; 

// run thread 1 method
void* run_thread_1_method(void* arg)
{
	int temp1 = 0;
	// for loop to go through 1 to 50 for summation
	for(int a = 1; a <= 25; a++)
	{
		// locking thread
		pthread_mutex_lock(&lck);
		// adding up 1 to 50
		global_sum += a;
		// sum of 1 to 50
		temp1+=a;
		// unlocking the thread
		pthread_mutex_unlock(&lck);
	}
		printf("Thread 1 sum: %d\n", temp1);
}

void* run_thread_2_method(void* arg)
{
	int temp2 = 0;
	// for loop to go through 51 to 100 for summation
	for(int b = 26; b <= 50; b++)
	{
		// locking thread
		pthread_mutex_lock(&lck);
		// adding up 51 to 100
		global_sum += b;
		// sum of 26 to 50
		temp2+=b;
		// unlocking the thread
		pthread_mutex_unlock(&lck);
	}
	printf("Thread 2 sum: %d\n", temp2);
}

void* run_thread_3_method(void* arg)
{
	int temp3 = 0;
	// for loop to go through 51 to 100 for summation
	for(int c = 51; c <= 75; c++)
	{
		// locking thread
		pthread_mutex_lock(&lck);
		// adding up 51 to 75
		global_sum += c;
		// sum of 51 to 75
		temp3+=c;
		// unlocking the thread
		pthread_mutex_unlock(&lck);
	}
	printf("Thread 3 sum: %d\n", temp3);
}

void* run_thread_4_method(void* arg)
{
	int temp4 = 0;
	// for loop to go through 51 to 100 for summation
	for(int d = 76; d <= 100; d++)
	{
		// locking thread
		pthread_mutex_lock(&lck);
		// adding up 76 to 100
		global_sum += d;
		// sum of 76 to 100
		temp4+=d;
		// unlocking the thread
		pthread_mutex_unlock(&lck);
	}
	printf("Thread 4 sum: %d\n", temp4);
}

// main method
int main(int argc, char **argv){

	// pthread creation
	pthread_t thread_id_1;
	pthread_t thread_id_2;
	pthread_t thread_id_3;
	pthread_t thread_id_4;

	// if loop fo check thread creation 1 and if failed print error and exit
	int a_1 = pthread_create(&thread_id_1, NULL, run_thread_1_method, NULL);
	if(a_1!= 0){
		perror("Thread creation failed\n");
		exit(-1);
	}

	// if loop fo check thread creation 2 and if failed print error and exit
	int a_2 = pthread_create(&thread_id_2, NULL, run_thread_2_method, NULL);
	if(a_2!= 0){
		perror("Thread creation failed\n");
		exit(-1);
	}

	// if loop fo check thread creation 3 and if failed print error and exit
	int a_3 = pthread_create(&thread_id_3, NULL, run_thread_3_method, NULL);
	if(a_3!= 0){
		perror("Thread creation failed\n");
		exit(-1);
	}

	// if loop fo check thread creation 4 and if failed print error and exit
	int a_4 = pthread_create(&thread_id_4, NULL, run_thread_4_method, NULL);
	if(a_4!= 0){
		perror("Thread creation failed\n");
		exit(-1);
	}

	// pthread join for thread_id_1
	pthread_join(thread_id_1, NULL);

	// pthread join for thread_id_2
	pthread_join(thread_id_2, NULL);

	// pthread join for thread_id_2
	pthread_join(thread_id_3, NULL);

	// pthread join for thread_id_2
	pthread_join(thread_id_4, NULL);

	// print statement to print the total sum
	printf("Total sum: %d\n", global_sum);

	//exit statement
	exit(0);

}
