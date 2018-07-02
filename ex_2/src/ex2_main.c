/***************************** FILE HEADER *********************************/
/*!
*  \file      ex2_main.c
*
*  \brief     Parenting Process Exercise
*
*  \author    Octav Teodorescu
*  \copyright 2018 Luxoft Romania
*
*  \version 1 Created on: 02.07.2018
*
*//**************************** FILE HEADER *********************************/

 
/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


/*****************************************************************************
 * Defines
 *****************************************************************************/
#define PIPEIN	0
#define PIPEOUT	1

#define LOGFILE "/home/oiteodorescu/WindRiver/Workspace/ex2.log"

/*****************************************************************************
 * Globals
 *****************************************************************************/
 
/*****************************************************************************
 * Locals
 *****************************************************************************/
typedef struct child_pipe_message_t
{
	int pid;
	int number;
} child_pipe_message_t;

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


// https://en.wikipedia.org/wiki/Primality_test
int isprime (int number)
{
	int i;
	
	if ( number <= 1 )	return 0;
	if ( number <= 3 )	return 1;
	if ( number % 2 == 0 || number % 3 == 0 ) return 0;
	
	i = 5;

	while ( i*i <= number )
	{
		if (( number % i == 0 ) || ( number % (i+2) == 0 ))	return 0;
		i += 6;
	}
	
	return 1;
}


int main (int argc, char** argv)
{

	int pipefds[10][2];
	int nproc;
	int pid;
	int number;
	
	int i;
	
	child_pipe_message_t msg;
	
	FILE* logf;
	
	printf("Initializing...\n");
	
	sscanf(argv[1], "%d", &nproc);
	
	printf("%d processes found\n",nproc);
	
	for ( i = 0; i < nproc; i++ )
		pipe( pipefds[i] );
	
	
	for ( i = 0; i < nproc; i++ )
	{
		printf("Fork %d\n",i);
		
		pid = fork();
		
		if ( pid == 0 )
		{
			close( pipefds[i][PIPEIN] );
			break;
		}
		
		close( pipefds[i][PIPEOUT] );
	}	
	
	
	if ( pid == 0 )
	{
		
		
		while (1)
		{
			struct timespec ts;
			
			if (timespec_get(&ts, TIME_UTC) == 0) {
				printf("Time err");
				return -1;
			}
			
			srandom(ts.tv_nsec ^ ts.tv_sec);
			number = random();
					
			
			if ( isprime(number) )
			{
				printf("Prime number generated! %d\n",number);
				
				msg.pid = getpid();
				msg.number = number;
				
				write( pipefds[i][PIPEOUT], &msg, sizeof(msg) );
				close( pipefds[i][PIPEOUT] );
				
				return 0;
			}
		}
	}
	else
	{
		logf = fopen(LOGFILE, "wt");
		
		while (1)
		{
			for ( i = 0; i < nproc; i++ )
			{
				if ( read( pipefds[i][PIPEIN], &msg, sizeof(msg) ) != 0 )
				{
					
					fprintf( logf, "%d %d\n", msg.pid, msg.number );
					close ( pipefds[i][PIPEIN] );
					
					memcpy( pipefds[i], pipefds[nproc-1], sizeof(pipefds[i]) );
					nproc--;
				}
			}
			
			if ( nproc == 0 )
				break;
		}
		
		fclose(logf);
	}
	
}
