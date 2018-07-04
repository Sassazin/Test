/***************************** FILE HEADER *********************************/
/*!
*  \file      ex3_main.c
*
*  \brief     Shared resource exercise; parent process source.
*
*  \author    Octav Teodorescu
*  \copyright 2018 Luxoft Romania
*
*  \version 1 Created on: 03/07/2018
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

#define FATAL_ERROR(err) perror(err);\
	return -1

/*****************************************************************************
 * Globals
 *****************************************************************************/


/*****************************************************************************
 * Locals
 *****************************************************************************/



int main (int argc, char* argv[])
{
	int	pids[NB_PROCS];
	int fd;

	int i;
	
	u_int8_t data[MAX_DATA_SIZE];
	sem_t* data_sem;
	
	
	
	data_sem = sem_open("/OIT_ex3_data_sem", O_CREAT, 0700, 1);
	if ( data_sem == SEM_FAILED )
		FATAL_ERROR("Error opening semaphore [data_sem]");
	
    fd = open( SHDMEM_FILEPATH, O_WRONLY | O_CREAT, RW_PERM_ALL );
    if ( fd == -1)
    	FATAL_ERROR("Error opening shdmem");


	if ( mmap( NULL, MAX_DATA_SIZE, PROT_WRITE, MAP_SHARED, fd, 0 ) == MAP_FAILED )
		FATAL_ERROR("Error mmap");
	
	
	for ( i = 0; i < NB_PROCS; i++ )
	{
		pids[i] = fork();
				 
		if ( pids[i] == 0 )
			if ( execvp("./ex3_child.exe",argv) == -1 )
				FATAL_ERROR("Error execvp");
	}
	
	
	while (1)
	{
		sleep(1);
		
		for ( i = 0; i < MAX_DATA_SIZE; i++ )
		{
			srandom(time(NULL));
			data[i] = (u_int8_t) random();
		}
		
		sem_wait(data_sem);
		write(fd, data, MAX_DATA_SIZE);	
		sem_post(data_sem);
		
		for ( i = 0; i < NB_PROCS; i++ )
			kill ( pids[i], SIGUSR1 );
	}

	return 0;
}
