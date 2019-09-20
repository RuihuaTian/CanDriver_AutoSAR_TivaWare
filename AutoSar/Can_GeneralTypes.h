/*
 * Can_GeneralTypes.h
 *
 *  Created on: Sep 20, 2019
 *      Author: r
 */

#ifndef CAN_GENERALTYPES_H_
#define CAN_GENERALTYPES_H_



typedef uint32 	Can_IdType ; 			// ambiguous type of message Id
typedef uint8 Can_HwHandleType;			// hardware object of can hw unit ---- message object id i think



typedef struct {


	pduIdType 	swPduHandle ;
	uint8 		length ;			/// obviously the length of the data
	Can_IdType	id ;				/// ambiguous type of message id
	uint8 * 	sdu ;				/// the data itself

}Can_PduType ;


/*
 * this type is a data structre of the hardware object ..
 *
 * the L-PDU id (message id)
 * the hardware handle (message object id)
 * the controller .. ( base Address)
 */

typedef struct {

	Can_IdType 			CanId ;
	Can_HwHandleType 	Hoh ;
	uint8 				ControllerId ;

}Can_HwType ;


typedef enum {

	CAN_ERRORSTATE_ACTIVE,
	CAN_ERRORSTATE_PASSIVE,
	CAN_ERRORSTARE_BUSOFF


}Can_ErrorStateType ;


typedef enum {

	CAN_CS_UINIT,
	CAN_CS_STARTED,
	CAN_CS_STOPPED,
	CAN_CS_SLEEP

} Can_ControllerStateType ;




#endif /* CAN_GENERALTYPES_H_ */
