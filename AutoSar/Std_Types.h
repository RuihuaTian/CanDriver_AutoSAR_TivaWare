/*
 * Std_Types.h
 *
 *  Created on: Aug 28, 2019
 *      Author: r
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_


#include "Compiler.h"
#include "Platform_Types.h"



#define FALSE 					(0u)
#define TRUE 					(1u)
#define false 					(0u)
#define true 					(1u)


#define STD_ON					(1u)
#define STD_OFF					(0u)

#define LOW 					(0u)
#define HIGH 					(1u)

#define NULL_PTR 				((void*)0)


typedef enum
{
	E_OK,
	E_NOT_OK

}Std_ReturnType;


typedef enum
{
	False,
	True
}Boolean ;

#endif /* STD_TYPES_H_ */
