/***************************** FILE HEADER *********************************/
/*!
*  \file      ex2_main.c
*
*  \brief     Parenting Process Exercise
*  
*  Usage:
*  		Call from cmdline specifying the number of processes with -n=x where x
*  		is that number.
*  		
*  		Exp:
*  			./ex2.exe -n=5
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
#include <ctype.h>


/*****************************************************************************
 * Defines
 *****************************************************************************/
#define MAX_NPROC	10

#define PIPEIN		0
#define PIPEOUT		1

#define LOGFILE 	"/home/oiteodorescu/WindRiver/Workspace/ex2.log"


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
 *  \fn                 isprime
 *************************************************************************//**
 *  \brief              Checks the primality of a given integer.
 *  
 *  					No error handling - might break for large input numbers! (TODO: check & fix)
 *  					
 *  \note               Implements 6k1 algorithm. See https://en.wikipedia.org/wiki/Primality_test
 * 
 *  \return             1 if prime, else 0
 ****************************************************************************/
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

	int pipefds[MAX_NPROC][2];
	int nproc;
	int pid;
	int number;
	
	int i;
	int c;
	
	child_pipe_message_t msg;
	
	FILE* logf;
	
	opterr = 0;
	
	//////////////////////////////
	
	// used getopt for scalability and easy err handling
	do
	{
		c = getopt(argc,argv,"n:");
		
		switch (c)
		{
			case 'n':
				printf("%s",optarg);
				sscanf(optarg,"%d",&nproc);
				break;
				
			case '?':
				if (optopt == 'n')			
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))	
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else 
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				
				return 1;
				
			case ':':
				fprintf(stderr,"WTF");
				break;
				
			default:
				fprintf(stderr,"Error reading cmdline args (defaulted case) : c = %d\n",c);
				return -1;
		}
		
	} while ( c != -1 );
	
	

	for ( i = 0; i < nproc; i++ )
	{
		if ( pipe( pipefds[i] ) == -1 )
		{
			fprintf(stderr,"Error creating pipe %d.\n",i+1);
			return -1;
		}
	}
	
	for ( i = 0; i < nproc; i++ )
	{
		pid = fork();
		
		if ( pid == -1 )
		{
			fprintf(stderr,"Failed to fork at process #%d.\n",i);
			return -1;
		}
		
		if ( pid == 0 )
		{
			if ( close( pipefds[i][PIPEIN] ) == -1 )
			{
				fprintf(stderr,"Error closing pipe %d input. (child)",i);
				return -1;
			}
			break;
		}
		
		if ( close( pipefds[i][PIPEOUT] ) == -1 )
		{
			fprintf(stderr,"Error closing pipe %d output.",i);
			return -1;
		}
	}	
	
	
	if ( pid == 0 )
	{
		
		while (1)
		{
			// Used for true randomness; see SEI CERT C Coding Standard 2016: MSC30-C
			struct timespec ts;
			
			if (timespec_get(&ts, TIME_UTC) == 0) {
				fprintf(stderr,"Error fetching time. Attempting again...\n");
				sleep(1);
				
				if (timespec_get(&ts, TIME_UTC) == 0) fprintf("Failed to fetch time. Using default seed.\n");
					srandom(getpid());
			}
			else
				srandom(ts.tv_nsec ^ ts.tv_sec);
			
			number = random();
			
			if ( isprime(number) )
			{				
				msg.pid = getpid();
				msg.number = number;
				
				write( pipefds[i][PIPEOUT], &msg, sizeof(msg) );
				
				if ( close( pipefds[i][PIPEOUT] ) == -1 )
				{
					fprintf(stderr,"Error closing pipe %d output. (child)",i);
					return -1;
				}
				
				return 0;
			}
		}
	}
	else
	{
		logf = fopen(LOGFILE, "wt");
		if ( logf == NULL )
			fprintf(stderr,"Error opening logfile. Printing to stdout.");

		
		while (1)
		{
			for ( i = 0; i < nproc; i++ )
			{
				if ( read( pipefds[i][PIPEIN], &msg, sizeof(msg) ) != 0 )
				{
					if ( logf != NULL )	
						fprintf( logf, "%d %d\n", msg.pid, msg.number );
					else
						fprintf( stdout, "%d %d\n", msg.pid, msg.number );
					
					if ( close( pipefds[i][PIPEIN] ) == -1 )
					{
						fprintf(stderr,"Error closing pipe %d input.",i);
						return -1;
					}
					
					memcpy( pipefds[i], pipefds[nproc-1], sizeof(pipefds[i]) );
					nproc--;
				}
			}
			
			if ( nproc == 0 )
				break;
		}
		
		if ( logf != NULL )
			fclose(logf);
	}
	
}
