/***************************** FILE HEADER *********************************/
/*!
*  \file      ex3_child.c
*
*  \brief     Shared resource exercise; child process source.
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
#include <unistd.h>
#include <semaphore.h>

#include "../h/coreutils.h"


/*****************************************************************************
 * Defines
 *****************************************************************************/
#define MAX_DATA_SIZE	100
#define RW_PERM_ALL		0666

#define SHDMEM_FILEPATH	"/tmp/ex3/mmapped.bin"


/*****************************************************************************
 * Globals
 *****************************************************************************/
 
/*****************************************************************************
 * Locals
 *****************************************************************************/
 

int main ()
{
	int sig;
	int fd;
	int readercount;
	
	u_int8_t data[MAX_DATA_SIZE];
	
	sem_t* data_sem;
	sem_t* rdmutex_sem;
	sem_t* wrpriority_sem;
	
	sigset_t set;
	
	
	readercount = 0;
	
	fd = open(SHDMEM_FILEPATH, O_RDONLY);
	if ( fd == -1 )
		FATAL_ERROR("Error opening shmem (child)");

	
	mmap( NULL, MAX_DATA_SIZE, PROT_READ, MAP_SHARED, fd, 0 );
	
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);
	
	data_sem		= 	sem_open("/OIT_ex3_data_sem"		,0666);
	rdmutex_sem		= 	sem_open("/OIT_ex3_rdmutex_sem"		,0666);
	wrpriority_sem 	= 	sem_open("/OIT_ex3_wrpriority_sem"	,0666);

	if ( data_sem 		== SEM_FAILED )	FATAL_ERROR("Error opening semaphore [data_sem] (child)");
	if ( rdmutex_sem	== SEM_FAILED )	FATAL_ERROR("Error opening semaphore [rdmutex_sem] (child)");
	if ( wrpriority_sem == SEM_FAILED )	FATAL_ERROR("Error opening semaphore [wrpriority_sem] (child)");
	
	
	while (1)
	{
		sigprocmask(SIG_UNBLOCK, &set, NULL);
		if ( sigwait(&set,&sig) > 0 )	FATAL_ERROR("sigwait error");
		sigprocmask(SIG_BLOCK, &set, NULL);
		
		
		sem_wait(rdmutex_sem);
		
		sem_wait(wrpriority_sem);
	
		readercount++;
		if ( readercount == 1 )
			sem_wait(data_sem);
		
		sem_post(wrpriority_sem);
		
		sem_post(rdmutex_sem);
		read(fd, data, MAX_DATA_SIZE);
		sem_wait(rdmutex_sem);
		
		readercount--;
		if ( readercount == 0 )
			sem_post(data_sem);
		
		sem_post(rdmutex_sem);
		
		fwrite(data, sizeof(u_int8_t), MAX_DATA_SIZE, stdout);
	}
	
	return 0;
}
