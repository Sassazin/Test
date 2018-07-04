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


#ifdef DBG_ON
#define DBGPRINT(str) {\
	fprintf(stdout, str); \
}
#endif


#define FATAL_ERROR(err) { \
	fprintf(stderr,"%d |" err "\t%s\n",getpid(), strerror(errno)); \
	return -1;}


#endif /* COREUTILS_H_ */
