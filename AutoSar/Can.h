/*
 * can.h
 *
 *  Created on: Sep 20, 2019
 *      Author: Sad MultiVerse
 */

#ifndef CAN_H_
#define CAN_H_

#define CAN_BUSY 	0x02

#include "ComStack_Types.h"
#include "Can_GeneralTypes.h"

#include "Can_cfg.h"


#define MAX_NO_OF_OBJECTS 			(32u)
#define NO_OF_CONTROLLERS_IN_HW 	(2u)

#define CAN_INITIALIZED 			(1u)
#define CAN_NOT_INITIALIZED 			(0u)

typedef uint8 Can_ReturnType ;			/// it takes a CAN_BUSY value
typedef uint8 CanControllerIdType ;





typedef enum {

	EXTENDED,
	MIXED,
	STANDARD

}CanIdTypeType ;
typedef enum {

	RECEIVE,
	TRANSMIT

}CanObjectTypeType ;



typedef struct {


	CanIdTypeType 			CanIdType ; 					/// Extended or
	uint8 					CanObjectId ;					/// unique id of the object HTH , HRH
	CanObjectTypeType		CanObjectType ;					/// HTH or HRH
	uint8 					CanContrlloerRef ;				/// the

}CanHardwareObjectType ;






typedef struct {


	CanControllerIdType 		CanControllerId ;
	Can_ControllerStateType 	ControllerStatus ;
	Can_ErrorStateType 			CanControllerErrorState ;


}CanControllerType ;


typedef struct {


	CanControllerType		CanControllers [NO_OF_CONTROLLERS_IN_HW] ;
	CanHardwareObjectType	HardWareObject[MAX_NO_OF_OBJECTS] ;



}Can_ConfigType;




Std_ReturnType Can_Write (

		Can_HwHandleType 	Hth,
		const Can_PduType* 	PduInfo
);



#endif /* CAN_H_ */
