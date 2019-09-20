/*
 * can.c
 *
 *  Created on: Sep 20, 2019
 *      Author: r
 */

/*
 *
 * Important Includes of TivaWare Libraries
 *
 */
#include "../TivaWare/stdint.h"
#include "../TivaWare/hw_can.h"
#include "../TivaWare/hw_ints.h"
#include "../TivaWare/hw_nvic.h"
#include "../TivaWare/hw_memmap.h"
#include "../TivaWare/hw_sysctl.h"
#include "../TivaWare/hw_types.h"

#include "../TivaWare/can.h"
#include "../TivaWare/interrupt.h"

/**************************************************/

#include "Can.h"
#include "CanIf_Cbk.h"
#include "Det.h"


/*

STATIC Can_HwType MsgObject [MAX_NO_OF_OBJECTS] ;
/*
 *
	but if init function did its job we can use it from the configurations itself
*/
STATIC Can_ConfigType * g_Config_Ptr ;

Std_ReturnType Can_Write (

		Can_HwHandleType 	Hth,
		const Can_PduType* 	PduInfo
)
{

	Std_ReturnType returnVal = E_OK ;


	uint8 index ;

	uint32_t 		uiBase ;
	uint32_t 		ui32ObjID ;
	tCANMsgObject *	psMsgObject ;
	tMsgObjType 	eMsgType ;

	/*
	 * choosing what can controller ..
	 */

	for (index = 0 ; index < MAX_NO_OF_OBJECTS ; index++ )
	{
		if (g_Config_Ptr->HardWareObject[index].CanObjectId == Hth )
		{
			switch (g_Config_Ptr->HardWareObject[index].CanContrlloerRef)
			{
				case 0 : uiBase = CAN0_BASE ; break ;
				case 1 : uiBase = CAN1_BASE ; break ;
			}
		}
	}

	/*
	 * Message Object Id
	 */

	ui32ObjID = Hth ;


	/*
	 * PDU .. message object itself
	 */
	psMsgObject->ui32MsgID = PduInfo->id ;
	psMsgObject->ui32MsgLen = PduInfo->length ;
	psMsgObject->pui8MsgData = PduInfo->sdu ;
	psMsgObject->ui32Flags = 0;									/// don't forget me
	psMsgObject->ui32MsgIDMask = 0 ;							// and me .. or you just can delete us from Tivaware itself, if you don't love us anymore !


	/*
	 * the type of the CAN_write () function is obviously Tx
	 */
	eMsgType = MSG_OBJ_TYPE_TX ;

	/*
	 * call the Tivaware function now !
	 */
	CANMessageSet( uiBase , ui32ObjID, psMsgObject, eMsgType  );




	return E_OK ;
}
