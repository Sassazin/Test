/***************************** FILE HEADER *********************************/
/*!
*  \file      ex3_main.c
*
*  \brief     Shared resource exercise; parent process source.
*  
*  \note	  The synchronization problem is a variant of the reader-writer
*  			  problem. See https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem
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
#define NB_PROCS		3
#define CHILD_FILEPATH	"./ex3_child.exe"

#define SEM_PERM		0777


/*****************************************************************************
 * Globals
 *****************************************************************************/


/*****************************************************************************
 * Locals
 *****************************************************************************/



int main (int argc, char* argv[])
{
	int	pids[NB_PROCS];
	int shm_fd;

	int i;
	
	char logfilename[32];
	FILE* logfile = NULL;
	
	u_int8_t data[MAX_DATA_SIZE];
	
	sem_t* data_sem;	// mutex for data R/W
	sem_t* wrpriority_sem;	// prevents starvation for writer 
							//(not possible in this implementation - if requirements change may become necessary)
	sem_t* rdmutex_sem;	// mutex for readercount
	
	
	
	srand(time(NULL));
	
	sprintf(logfilename,"./logs/parent%d_%d.log", getpid(),rand()%100);
	if ( (logfile = fopen(logfilename, "w")) == NULL )
		FATAL_ERROR("Error opening/creating logfile (parent)");
	
	
	DBGPRINT("Initializing...\n");
	
	if ( sem_unlink(SEM_FILE_DATA) == -1 )  
		if ( errno != ENOENT )
			FATAL_ERROR("Error cleaning semaphore [data_sem]");
	
	if ( sem_unlink(SEM_FILE_WRPRIORITY) == -1 ) 
		if ( errno != ENOENT )
			FATAL_ERROR("Error cleaning semaphore [wrpriority_sem]");
	
	if ( sem_unlink(SEM_FILE_RDMUTEX) == -1 ) 
		if ( errno != ENOENT )
			FATAL_ERROR("Error cleaning semaphore [rdmutex_sem]");
	
	data_sem = sem_open(SEM_FILE_DATA, O_EXCL | O_CREAT, SEM_PERM, 1);
	if ( data_sem == SEM_FAILED )
		FATAL_ERROR("Error opening semaphore [data_sem]");
	
	wrpriority_sem = sem_open(SEM_FILE_WRPRIORITY, O_EXCL | O_CREAT, SEM_PERM, 1);
	if ( wrpriority_sem == SEM_FAILED )
		FATAL_ERROR("Error opening semaphore [wrpriority_sem]");
	
	
	
    shm_fd = open( SHDMEM_FILEPATH, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO );
    if ( shm_fd == -1)
    	FATAL_ERROR("Error opening shdmem");

	if ( mmap( NULL, MAX_DATA_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0 ) == MAP_FAILED )
		FATAL_ERROR("Error mmap");
	
	
	
	DBGPRINT("Spawning children...\n");
	
	for ( i = 0; i < NB_PROCS; i++ )
	{
		pids[i] = fork();
				 
		if ( pids[i] == 0 )
			if ( execvp("./ex3_child.exe", argv) == -1 )
				FATAL_ERROR("Error execvp");
	}
	
	
	DBGPRINT("Forks successful. Starting core functionality...\n");
	fflush(logfile);
	
	while (1)
	{
		DBGPRINT("Going to sleep.\n");
		sleep(1);
		
		DBGPRINT("Generating random data...\n");
		for ( i = 0; i < MAX_DATA_SIZE; i++ )
			data[i] = (u_int8_t) rand();
		
		
		DBGPRINT("Waiting for access...\n");
		sem_wait(wrpriority_sem);
		
		sem_wait(data_sem);
		DBGPRINT("Writing data...\n");
		write(shm_fd, data, MAX_DATA_SIZE);
		sem_post(data_sem);
		
		sem_post(wrpriority_sem);
		
		DBGPRINT("Signaling children...\n");
		for ( i = 0; i < NB_PROCS; i++ )
			kill ( pids[i], SIGUSR1 );
		
		fflush(logfile);
	}

	return 0;
}
