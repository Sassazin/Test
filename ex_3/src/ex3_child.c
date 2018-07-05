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
#define SEM_PERM		0777


/*****************************************************************************
 * Globals
 *****************************************************************************/
 
/*****************************************************************************
 * Locals
 *****************************************************************************/
 

int main (int argc, char** argv)
{
	int sig;
	int fd;
	int readercount;
	
	u_int8_t data[MAX_DATA_SIZE];
	
	sem_t* data_sem;
	sem_t* rdmutex_sem;
	sem_t* wrpriority_sem;
	
	sigset_t set;
	
	char logfile[32];

	
	readercount = 0;
	
	/*
	sprintf(logfile,"./logs/%d.log",getpid());
	if ( freopen(logfile, "wt", stdout) == NULL )
		FATAL_ERROR("Error opening/creating logfile (child)");
	*/
	
	fd = open(SHDMEM_FILEPATH, O_RDONLY);
	if ( fd == -1 )
		FATAL_ERROR("Error opening shmem (child)");
	
	
	mmap( NULL, MAX_DATA_SIZE, PROT_READ, MAP_SHARED, fd, 0 );
	
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);
	
	data_sem		= 	sem_open(SEM_FILE_DATA		, O_CREAT, SEM_PERM, 1);
	rdmutex_sem		= 	sem_open(SEM_FILE_RDMUTEX	, O_CREAT, SEM_PERM, 1);
	wrpriority_sem 	= 	sem_open(SEM_FILE_WRPRIORITY, O_CREAT, SEM_PERM, 1);

	if ( data_sem 		== SEM_FAILED )	FATAL_ERROR("Error opening semaphore [data_sem] (child)");
	if ( rdmutex_sem	== SEM_FAILED )	FATAL_ERROR("Error opening semaphore [rdmutex_sem] (child)");
	if ( wrpriority_sem == SEM_FAILED )	FATAL_ERROR("Error opening semaphore [wrpriority_sem] (child)");

	
	while (1)
	{
		DBGPRINT("Waiting for signal...\n");
		
		//sigprocmask(SIG_UNBLOCK, &set, NULL);
		if ( sigwait(&set,&sig) > 0 )	FATAL_ERROR("sigwait error");
		//sigprocmask(SIG_BLOCK, &set, NULL);
		
		DBGPRINT ("Signal received. Waiting for access...\n");
		
		sem_wait(wrpriority_sem);
		
		DBGPRINT("Accessing entry zone...\n");
		
		sem_wait(rdmutex_sem);
		DBGPRINT("In entry zone...\n");
		readercount++;
		sem_post(rdmutex_sem);
		
		if ( readercount == 1 )
			sem_wait(data_sem);
		
		DBGPRINT("Exiting entry zone...\n");
		sem_post(wrpriority_sem);
		
		
		DBGPRINT("Reading data...\n");
		
		read(fd, data, MAX_DATA_SIZE);
		
		DBGPRINT("Accessing exit zone...\n");
		sem_wait(rdmutex_sem);
		DBGPRINT("In exit zone...\n");
		readercount--;
		sem_post(rdmutex_sem);
		if ( readercount == 0 )
			sem_post(data_sem);
		
		
		fwrite(data, sizeof(u_int8_t), MAX_DATA_SIZE, stdout);
		DBGPRINT("Done.\n");
	}
	
	return 0;
}
