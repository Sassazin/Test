/***************************** FILE HEADER *********************************/
/*!
*  \file      coreutils.h
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

#define SHDMEM_FILEPATH	"/tmp/ex3/mmapped.bin"
#define SHDMEM_FILEPATH		"/tmp/ex3/mmapped.bin"
#define SEM_FILE_DATA		"/OIT_ex3_data_sem"
#define SEM_FILE_WRPRIORITY	"/OIT_ex3_wrpriority_sem"
#define SEM_FILE_RDMUTEX	"/OIT_ex3_rdmutex_sem"


#define FATAL_ERROR(err) { \
	fprintf(stderr,"%d |" err "\t%s\n",getpid(), strerror(errno)); \
	return -1;}


#ifdef DBG_ON

#define DBGPRINT(str) {\
	fprintf(stdout, str); \
}

#endif // DBG_ON

#endif /* COREUTILS_H_ */
