#ifndef UTILS_H_
#define UTILS_H_
/***************************** FILE HEADER *********************************/
/*!
*  \file      utils.h
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
 * Defines
 *****************************************************************************/
#ifndef FATAL_ERROR(err)
	#define FATAL_ERROR(err) perror(err);\
		return -1
#endif


/*****************************************************************************
 * Functions
 *****************************************************************************/
/*****************************************************************************
 *  \fn                 isprime
 *************************************************************************//**
 *  \brief              Checks if a number is prime.
 *  					Based on 6k1 algorithm. For more details check:
 *  					https://en.wikipedia.org/wiki/Primality_test#Pseudocode
 *  
 *  \return             1 - TRUE / 0 - FALSE
 *  \author             
 *  \date               04/JUL/2018
 ****************************************************************************/
int isprime (int);


#endif UTILS_H_
