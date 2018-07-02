/***************************** FILE HEADER *********************************/
/*!
*  \file      ex1_main.c
*
*  \brief     Multithreaded RNG 
*
*  \note      DEV
*
*  \author    Octav Teodorescu
*  \copyright 2018 Luxoft Romania
*
*  \version 1 Created on: 29.06.2018
*  
*  Note that name "Ex 1 - multiple threads producer-consumer" is misleading.
*  The exercise is formulated in fact like a simplified version of the reader-writer
*  problem. Since we only have one writer and one or more readers the only thing
*  that needs to be taken into consideration is stopping readers from reading
*  while the writer does his job.
*  
*
*//**************************** FILE HEADER *********************************/

 
/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h>  
#include <pthread.h>
#include <errno.h>


/*****************************************************************************
 * Defines
 *****************************************************************************/
#define GEN_TIME 		 100
#define MAX_NB_RND		 1000
#define MIN_NB_RND		 100

#define MAX_NB_THREADS	 4
#define NB_READERS		 MAX_NB_THREADS - 1

#define LOGFILE			"/home/oiteodorescu/WindRiver/Workspace/ex1.log"

#define EFAILEDALLOC	 -1


/*****************************************************************************
 * Globals
 *****************************************************************************/
 

/*****************************************************************************
 * Locals
 *****************************************************************************/
pthread_mutex_t wrlock;
pthread_mutex_t rdlock;

int* numbers;


/*****************************************************************************
 *  \fn                 genNumbers
 *************************************************************************//**
 *  \brief              Mainloop for number generator thread.
 *  \note               -
 *  \param[in]          ...
 *  \param[out]         ...
 *  \return             ...
 *  \author             ...
 *  \date               YYYY-MM-DD
 ****************************************************************************/
void* generateNumbers (void* arg)
{
	while (1)
	{
		
	}
}


void* printNumbers (void* arg)
{
	while (1)
	{
		
	}
}


int ex1_main_init()
{
	int i;
	
	
	numbers = (int*) malloc (MAX_NB_RND * sizeof(int));
	
	if ( numbers == NULL )
		return EFAILEDALLOC;
	
	for ( i = 0; i < MAX_NB_RND; i++ )
		numbers[i] = 0;
	
	
	pthread_mutex_init(&wrlock,NULL);
	pthread_mutex_init(&rdlock,NULL);
	
	return 0;
}


int main ()
{
	pthread_t writer;
	pthread_t readers[NB_READERS];
	
	int randnb = 0;
	int i;
	
	/////////////
	
	ex1_main_init();
	
	pthread_create (&writer,NULL,&generateNumbers,NULL);
	
	for ( i = 0; i < NB_THREADS; i++ )
		pthread_create(&readers[i],NULL,&printNumbers,NULL);
	
	/////////////
	
	
}
