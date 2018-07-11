/***************************** FILE HEADER *********************************/
/*!
*  \file      ex1_main.c
*
*  \brief    "multiple threads producer-consumer" exercise
*
*	The program spawns 4 threads:
*	
*	> one thread which generates N random numbers every GEN_TIME seconds and puts them into a shared buffer
*	> three threads which read from the buffer, 'consuming' the numbers in the process, and output
*	if the number is prime or not
*	
*  \note The generation doesn't happen every GEN_TIME, rather GEN_TIME is just the amount of time the producer sleeps every round. 
*
*  \author    Octav Teodorescu
*  \copyright 2018 Luxoft Romania
*  
*  \version 1 Created on: 04/JUL/2018
*
*//**************************** FILE HEADER *********************************/

 
/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/syscall.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


/*****************************************************************************
 * Defines
 *****************************************************************************/
#define NTHREADS	3 					// number of reader threads to generate
#define GEN_TIME_MS	100
#define GEN_TIME	GEN_TIME_MS/1000	// sleeping time (seconds) for producer thread

#define MAX_NUMBERS	1000				// maximum amount of numbers generated each round
#define MIN_NUMBERS	100					// minimum amount of numbers generated each round

#define SEM_FULL	"/OIT_ex1_full_sem"  // Standard producer-consumer problem
#define SEM_EMPTY	"/OIT_ex1_empty_sem" // 	see https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem
#define SEM_DATA	"/OIT_ex1_data_sem"  // 	for details regarding the algorithm


#define FATAL_ERROR(err) { \
	fprintf(stderr,"%d |" err "\t%s\n", getpid(), strerror(errno)); \
return -1;}

#define DBGPRINT(str) {\
fprintf(stdout,"%ld|" str,syscall(SYS_gettid));}


/*****************************************************************************
 * Globals
 *****************************************************************************/
 extern int isprime (int);

 
/*****************************************************************************
 * Locals
 *****************************************************************************/

sem_t* empty_sem;
sem_t* full_sem;
sem_t* data_sem;

int readercount = 0;

typedef struct shared_data
{
	 int N;
	 int numbers[MAX_NUMBERS];
	 
}shared_data_t;


/*****************************************************************************
 * Functions
 *****************************************************************************/
void* read_numbers		(void*);
void* generate_numbers	(void*);



int main ()
{
	pthread_t thd;
	shared_data_t data;
	
	int i;
	
	
	data.N = 0;
	for ( i = 0; i < MAX_NUMBERS; i++ )
		data.numbers[i] = 0;
	
	// Initialize semaphores:
	// empty_sem	-> number of empty slots in the data buffer
	// full_sem		-> number of filled slots
	// data_sem		-> data access mutex
	if ( sem_unlink(SEM_FULL) == -1 )  
		if ( errno != ENOENT )
			FATAL_ERROR("Error cleaning semaphore [full_sem]");
	
	if ( sem_unlink(SEM_EMPTY) == -1 ) 
		if ( errno != ENOENT )
			FATAL_ERROR("Error cleaning semaphore [empty_sem]");
	
	if ( sem_unlink(SEM_DATA) == -1 ) 
		if ( errno != ENOENT )
			FATAL_ERROR("Error cleaning semaphore [data_sem]");
	
	full_sem		= sem_open(SEM_FULL,  O_CREAT | O_EXCL, O_RDWR,		 0		);
	data_sem 		= sem_open(SEM_DATA,  O_CREAT | O_EXCL, O_RDWR,		 1		);
	empty_sem 		= sem_open(SEM_EMPTY, O_CREAT | O_EXCL, O_RDWR, MAX_NUMBERS );
	
	if ( empty_sem == SEM_FAILED ) 	FATAL_ERROR("Error opening empty_sem");
	if ( full_sem  == SEM_FAILED ) 	FATAL_ERROR("Error opening full_sem");
	if ( data_sem  == SEM_FAILED ) 	FATAL_ERROR("Error opening datamutex_sem");
	
	
	// Spawn threads
	if ( pthread_create( &thd, NULL, &generate_numbers, &data) != 0 )
		FATAL_ERROR("Failed to create thread (generator)");

	for ( i = 0; i < NTHREADS; i++ )
		if ( pthread_create( &thd, NULL, &read_numbers, &data) != 0 )
			FATAL_ERROR("Failed to create thread (read)");

	
	// Main thread has to be kept alive artificially. 
	while(1);
		
	return 0;
}


void* read_numbers (void* arg)
 {
	 shared_data_t* data = (shared_data_t*)(arg);
	 int number;	 	 
	 
	 
	 while (1)
	 {
		 sem_wait(full_sem);
		 
		 sem_wait(data_sem);
		 number = data->numbers[data->N-1];
		 data->N--;
		 sem_post(data_sem);
		 
		 sem_post(empty_sem);
		 
		 if ( isprime(number) )
			 fprintf(stdout, "Thread %ld: number %d, prime: %s\n", syscall(SYS_gettid), number, "YES" );
		 else
			 fprintf(stdout, "Thread %ld: number %d, prime: %s\n", syscall(SYS_gettid), number, "NO" );
	 }
 }


void* generate_numbers (void* arg)
{
	shared_data_t* data = (shared_data_t*)(arg);
	int N = MAX_NUMBERS;
	
	srand(time(NULL));
	while (1)
	{
		sleep(GEN_TIME);
				
		while ( N != 0 )
		{
			sem_wait(empty_sem);
			
			sem_wait(data_sem);
			data->numbers[data->N] = rand();
			data->N++;
			sem_post(data_sem);
			
			sem_post(full_sem);
			
			N--;
		}
		
		N = rand() % (MAX_NUMBERS - MIN_NUMBERS) + MIN_NUMBERS;
	}
}
