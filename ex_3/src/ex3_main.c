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
#include <semaphore.h>


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
int ex3_init ( int* pids, int* fd )
{
    *fd = open( SHDMEM_FILEPATH, O_WRONLY | O_CREAT, RW_PERM_ALL );
    if ( (*fd) == -1) {
    	return -1;
    }

	mmap( NULL, MAX_DATA_SIZE, PROT_WRITE, MAP_SHARED, fd, 0 );
	
	
	for ( i = 0; i < NB_PROCS; i++ )
	{
		pids[i] = fork();
				 
		if ( pids[i] == 0 )
		{
			execl();
		}
	}
	
	return 0;
}


int main ()
{
	int	pids[NB_PROCS];
	uint8_t data[MAX_DATA_SIZE];
	int fd;
	
	sem_t data_sem;
	sem_t rdmutex_sem;
	sem_t wrpriority_sem;
	
	int i;
	
	sem_open("/OIT_ex3_data_sem", 	 	O_CREAT, 0666, 1);
	sem_open("/OIT_ex3_rdmutex_sem",	O_CREAT, 0666, 1);
	sem_open("/OIT_ex3_wrpriority_sem", O_CREAT, 0666, 1);
	
	ex3_init(pids,&fd);
	
	

}
