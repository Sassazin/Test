/***************************** FILE HEADER *********************************/
/*!
*  \file      ex1_main.c
*
*  \brief    "multiple threads producer-consumer" exercise
*   
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
#define GEN_TIME_MS	100
#define GEN_TIME	GEN_TIME_MS/1000

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
void* read_numbers(void*);
void* generate_numbers(void*);


int main ()
{
	pthread_t thd;
	shared_data_t data;
	
	int i;
	
	
	data->N = 0;
	for ( i = 0; i < MAX_NUMBERS; i++ )
		data->numbers[i] = 0;
	
	full_sem		= sem_open("/OIT_ex1_full_sem",		 O_CREAT, O_RDWR, 0);
	empty_sem 		= sem_open("/OIT_ex1_empty_sem", 	 O_CREAT, O_RDWR, 0);
	data_sem 		= sem_open("/OIT_ex1_data_sem", O_CREAT, O_RDWR, 0);
	
	if ( empty_sem 	== SEM_FAILED ) 	FATAL_ERROR("Error opening empty_sem");
	if ( full_sem  	== SEM_FAILED ) 	FATAL_ERROR("Error opening full_sem");
	if ( data_sem  == SEM_FAILED ) FATAL_ERROR("Error opening datamutex_sem");
	
	
	if ( !pthread_create( &thd, NULL, &generate_numbers, &data) )
		FATAL_ERROR("Failed to create thread (generator)");
	
	for ( i = 0; i < NTHREADS; i++ )
		if ( !pthread_create( &thd, NULL, &read_numbers, &data) )
			FATAL_ERROR("Failed to create thread (read)");
	
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
	int i;
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
