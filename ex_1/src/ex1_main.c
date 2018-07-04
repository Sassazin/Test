/***************************** FILE HEADER *********************************/
/*!
*  \file      ex1_main.c
*
*  \brief    "multiple threads producer-consumer" exercise
*  
*  \note	Based on the given specifications the underlying pattern has been 
*  			identified as being in fact a particular case of reader-writers.  
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

#include "../h/ex1_main.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/
#define NTHREADS	3 
#define GEN_TIME	100
#define GEN_TIME_S	GEN_TIME/1000

#define MAX_NUMBERS	1000
#define MIN_NUMBERS	100

#define FATAL_ERROR(err) perror(err);\
	return -1


/*****************************************************************************
 * Globals
 *****************************************************************************/
 extern int isprime (int);

 
/*****************************************************************************
 * Locals
 *****************************************************************************/

pthread_mutex_t rdmutex;
sem_t* syncrw_sem;
sem_t* datamutex_sem;

int readercount = 0;

typedef struct shared_data
{
	 int N;
	 int numbers[MAX_NUMBERS];
	 
}shared_data_t;


/*****************************************************************************
 * Functions
 *****************************************************************************/
void* read_numbers(void*);
void* generate_numbers(void*);


int main ()
{
	pthread_t thd;
	shared_data_t data;
	
	int i;
	
	
	syncrw_sem		= sem_open("/OIT_ex1_syncrw_sem",	 O_CREAT, O_RDWR, 0666);
	datamutex_sem 	= sem_open("/OIT_ex1_datamutex_sem", O_CREAT, O_RDWR, 0666);
	if ( pthread_mutex_init(&rdmutex, NULL) != 0 ) FATAL_ERROR("Error initializing rdmutex");
	
	if ( syncrw_sem 	== SEM_FAILED ) FATAL_ERROR("Error opening syncrw_sem");
	if ( datamutex_sem  == SEM_FAILED ) FATAL_ERROR("Error opening datamutex_sem");

	
	if ( !pthread_create( &thd, NULL, &generate_numbers, &data) )
		FATAL_ERROR("Failed to create thread (generator)");
	
	for ( i = 0; i < NTHREADS; i++ )
		if ( !pthread_create( &thd, NULL, &read_numbers, &data) )
			FATAL_ERROR("Failed to create thread (read)");
	
	
	return 0;
}


void* read_numbers (void* arg)
 {
	 shared_data_t* data = (shared_data_t*)(arg);
	 int number;	 	 
	 
	 
	 while (1)
	 {
		 sem_wait(syncrw_sem);
		 
		 pthread_mutex_lock(&rdmutex);
		 
		 readercount++;
		 if ( readercount == 1 )
			 sem_wait(datamutex_sem);
		 
		 pthread_mutex_unlock(&rdmutex);
		 
		 sem_post(syncrw_sem);
		 
		 number = data->numbers[data->N-1];
		 data->N--;
		 
		 pthread_mutex_lock(&rdmutex);
		 
		 readercount--;
		 if ( readercount == 0 )
			 sem_post(datamutex_sem);
		 
		 pthread_mutex_unlock(&rdmutex);
		 
		 if ( isprime(number) )
			 fprintf(stdout, "Thread %ld: number %d, prime: %s\n", syscall(SYS_gettid), number, "YES" );
		 else
			 fprintf(stdout, "Thread %ld: number %d, prime: %s\n", syscall(SYS_gettid), number, "NO" );
	 }
 }


void* generate_numbers (void* arg)
{
	shared_data_t* data = (shared_data_t*)(arg);
	int i;
	
	srand(time(NULL));
	while (1)
	{
		sleep(GEN_TIME_S);
		
		sem_wait(syncrw_sem);
		sem_wait(datamutex_sem);
		
		data->N = rand() % (MAX_NUMBERS - MIN_NUMBERS) + MIN_NUMBERS;
		
		for ( i = 0; i < data->N; i++ )
			data->numbers[i] = rand();
		
		sem_post(datamutex_sem);
		sem_post(syncrw_sem);
	}
}
