/***************************** FILE HEADER *********************************/
/*!
*  \file      utils.c
*
*  \brief     Utility functions for Induction Project.
*
*  \author    Octav Teodorescu
*  \copyright 2018 Luxoft Romania
*  
*  \version 1 Created on: 04/JUL/2018
*
*//**************************** FILE HEADER *********************************/

 
/*****************************************************************************
 * Includes
 *****************************************************************************/
#include "../h/utils.h"

 
 
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
