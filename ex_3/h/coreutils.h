/***************************** FILE HEADER *********************************/
/*!
*  \file      ex3_child.c
*
*  \brief     Shared resource exercise. Common definitons and includes
*
*  \author    Octav Teodorescu
*  \copyright 2018 Luxoft Romania
*
*  \version 1 Created on: 03/07/2018
*
*//**************************** FILE HEADER *********************************/

#ifndef COREUTILS_H_
#define COREUTILS_H_
/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <errno.h>
#include <string.h>


/*****************************************************************************
 * Defines
 *****************************************************************************/
#define DBG_ON
#define MAX_DATA_SIZE	100

#define SHDMEM_FILEPATH		"/tmp/ex3/mmapped.bin"
#define SEM_FILE_DATA		"/OIT_ex3_data_sem"
#define SEM_FILE_WRPRIORITY	"/OIT_ex3_wrpriority_sem"
#define SEM_FILE_RDMUTEX	"/OIT_ex3_rdmutex_sem"


#define FATAL_ERROR(err) { \
	fprintf(stderr,"%d |" err "\t%s\n",getpid(), strerror(errno)); \
	return -1;}


#ifdef DBG_ON

#define DBGPRINT(str) {\
	fprintf(stdout, str);}

#endif // DBG_ON

#endif /* COREUTILS_H_ */
