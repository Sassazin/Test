/***************************** FILE HEADER *********************************/
/*!
*  \file      sourceTemplate.c
*
*  \brief     Todo: Brief description of this file
*
*  \note      
*
*  \author    Bob Builder
*  \copyright 2012 Hirschmann Automation & Control GmbH
*
*  \version 1 Created on: 20.04.2012
*
*//**************************** FILE HEADER *********************************/

 
/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


/*****************************************************************************
 * Defines
 *****************************************************************************/
#define MAX_DATA_SIZE	100
#define NB_PROCS		3

#define SHDMEM_FILEPATH	"/tmp/ex3/mmapped.bin"
#define CHILD_FILEPATH	"./ex3_child.exe"
#define RW_PERM_ALL		0666


/*****************************************************************************
 * Globals
 *****************************************************************************/


/*****************************************************************************
 * Locals
 *****************************************************************************/


/*****************************************************************************
 *  \fn                 functionname
 *************************************************************************//**
 *  \brief              This function ...
 *  \note               -
 *  \param[in]          ...
 *  \param[out]         ...
 *  \return             ...
 *  \author             ...
 *  \date               YYYY-MM-DD
 ****************************************************************************/


int main ()
{
	int	pids[NB_PROCS];
	u_int8_t data[MAX_DATA_SIZE];
	int fd;
	
	sem_t* data_sem;
	
	int i;
	const char dummyargs[] = "whatever";
	
	data_sem = sem_open("/OIT_ex3_data_sem", O_CREAT, 0666, 1);
	sem_open("/OIT_ex3_rdmutex_sem",	O_CREAT, 0666, 1);
	sem_open("/OIT_ex3_wrpriority_sem", O_CREAT, 0666, 1);
	
	
    fd = open( SHDMEM_FILEPATH, O_WRONLY | O_CREAT, RW_PERM_ALL );
    if ( (fd) == -1) {
    	return -1;
    }

	mmap( NULL, MAX_DATA_SIZE, PROT_WRITE, MAP_SHARED, fd, 0 );
	
	
	for ( i = 0; i < NB_PROCS; i++ )
	{
		pids[i] = fork();
				 
		if ( pids[i] == 0 )
		{
			execl("./ex3_child.exe",dummyargs);
		}
	}
	
	
	while (1)
	{
		sleep(1);
		
		sem_wait(data_sem);
		
		for ( i = 0; i < MAX_DATA_SIZE; i++ )
		{
			srandom(time(NULL));
			data[i] = (u_int8_t) random();
		}
		
		for ( i = 0; i < NB_PROCS; i++ )
			kill ( pids[i], SIGUSR1 );
		
		sem_post(data_sem);
	}

}
