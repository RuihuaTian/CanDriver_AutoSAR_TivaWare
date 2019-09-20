/*
 * ComStack_Types.h
 *
 *  Created on: Sep 20, 2019
 *      Author: r
 */

#ifndef COMSTACK_TYPES_H_
#define COMSTACK_TYPES_H_


typedef uint8 PduIdType ;


typedef uint8 PduLengthType ;


typedef struct {

	uint8* 			SduDataPtr ;
	uint8* 			MetaDatePtr ;
	PduLengthType 	SduLength ;


}PduInfoType;

#endif /* COMSTACK_TYPES_H_ */
