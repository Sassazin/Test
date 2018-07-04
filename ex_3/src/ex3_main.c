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

#include "../h/coreutils.h"


/*****************************************************************************
 * Defines
 *****************************************************************************/
#define MAX_DATA_SIZE	100
#define NB_PROCS		3

#define SHDMEM_FILEPATH	"/tmp/ex3/mmapped.bin"
#define CHILD_FILEPATH	"./ex3_child.exe"

#define RW_PERM_ALL		0777


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

	
	DBGPRINT("Initializing...\n");
	
	data_sem = sem_open("/OIT_ex3_data_sem", O_CREAT, 0700, 1);
	if ( data_sem == SEM_FAILED )
		FATAL_ERROR("Error opening semaphore [data_sem]");
	
    fd = open( SHDMEM_FILEPATH, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO );
    if ( fd == -1)
    	FATAL_ERROR("Error opening shdmem");


	if ( mmap( NULL, MAX_DATA_SIZE, PROT_WRITE, MAP_SHARED, fd, 0 ) == MAP_FAILED )
		FATAL_ERROR("Error mmap");
	
	
	DBGPRINT("Spawning children...\n");
	
	for ( i = 0; i < NB_PROCS; i++ )
	{
		pids[i] = fork();
				 
		if ( pids[i] == 0 )
			if ( execvp("./ex3_child.exe",argv) == -1 )
				FATAL_ERROR("Error execvp");
	}
	
	DBGPRINT("Forks successful. Starting core functionality...\n");
	
	while (1)
	{
		DBGPRINT("Going to sleep.\n");
		
		sleep(1);
		
		DBGPRINT("Wake up.\nGenerating random data...\n");
		
		for ( i = 0; i < MAX_DATA_SIZE; i++ )
		{
			srandom(time(NULL));
			data[i] = (u_int8_t) random();
		}
		
		DBGPRINT("Waiting for access...\n");
		
		sem_wait(data_sem);
		DBGPRINT("Writing data...\n");
		write(fd, data, MAX_DATA_SIZE);	
		sem_post(data_sem);
		
		DBGPRINT("Signaling children...\n");
		
		for ( i = 0; i < NB_PROCS; i++ )
			kill ( pids[i], SIGUSR1 );
	}

	return 0;
}
