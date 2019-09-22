/*
 * can.c
 *
 *  Created on: Sep 20, 2019
 *      Author: Sad MultiVerse
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
STATIC uint8 Can_DriverState =  CAN_INITIALIZED ;						/// assume that init function has initialized it !

STATIC PduIdType 	Saved_swPduHandle ;

uint8 HTH_Semaphore[MAX_NO_OF_OBJECTS] = 0 ;							//// simulation of a semaphore by a sad global variable to protect HTH or generally all tha HOH


Std_ReturnType Can_Write (

		Can_HwHandleType 	Hth,
		const Can_PduType* 	PduInfo
)
{

	Std_ReturnType returnVal = E_OK ;

/*
 * check if you left the DevErrorDectect Open ..
 */
#if (CanDevErrorDetect == TRUE)
	if (Can_DriverState == CAN_NOT_INITIALIZED)
	{
		//// call the DET function .. CAN_E_UNIT
		returnVal = E_NOT_OK ;
	}

	else if (Hth > 64)
	{
		//// call the DET .... CAN_E_PARAM_HANDLE
		returnVal = E_NOT_OK ;
	}
	else if (PduInfo == NULL_PTR)
		{
			//// call the DET .... CAN_E_PARAM_POINTER
			returnVal = E_NOT_OK ;
		}
	else if (PduInfo->length > 64)
		{
			//// call the DET .... CAN_E_PARAM_DATA_LENGTH
			returnVal = E_NOT_OK ;
		}
	else

#endif
	{

		/*
		 * if every thing went well after the error detection .. prepare the parameters of CanMessageSet() [search for it]
		 */

		uint32_t 		uiBase ;
		uint32_t 		ui32ObjID ;
		tCANMsgObject *	psMsgObject ;
		tMsgObjType 	eMsgType ;


		if (HTH_Semaphore[Hth] == 0 ) 					//// 0 : no one uses it at the moment !

		{
			///// start protecting your shared stuff man !
			HTH_Semaphore[Hth] = 1 ;											//// acquire me

			/*
			 * choosing what can controller .. to detect what is the base address
			 */

			/*
			for (index = 0 ; index < MAX_NO_OF_OBJECTS ; index++ )
			{
				if (g_Config_Ptr->HardWareObject[index].CanObjectId == Hth )
				{
					switch (g_Config_Ptr->HardWareObject[index].CanContrlloerRef)
					{
						case 0 : uiBase = CAN0_BASE ; break ;
						case 1 : uiBase = CAN1_BASE ; break ;
					}

					psMsgObject->ui32Flags = ( g_Config_Ptr->HardWareObject[index].CanIdType == STANDARD )? MSG_OBJ_NO_FLAGS : MSG_OBJ_EXTENDED_ID ; /// flags are not handled in this Api except for stand vs extend

				}
			}
*/

			/*
			 * another method to decide what can controller to use.
			 * if the argument HTH is bigger that 32 .. CAN0 .. else CAN1
			 * and assign the message object id with a proper value
			 */

			if (Hth > 32)
			{
				uiBase = CAN1_BASE ;
				ui32ObjID = Hth - 32 ;
			}
			else
			{
				uiBase = CAN0_BASE ;
				ui32ObjID = Hth ;
			}

			/// what is the type of the message id ?

			psMsgObject->ui32Flags = ( g_Config_Ptr->HardWareObject[Hth].CanIdType == STANDARD )? MSG_OBJ_NO_FLAGS : MSG_OBJ_EXTENDED_ID ;

			// also from the can_id_type itself
/*
			psMsgObject->ui32Flags = (PduInfo->id & (1<<31))?  MSG_OBJ_EXTENDED_ID  : MSG_OBJ_NO_FLAGS  ;			check the most significant bit
*/

			/*
			 * PDU .. message object itself
			 */

			psMsgObject->ui32MsgID =  (PduInfo->id & (1<<31) )? ( PduInfo->id & 0x1fffffff ) : ( PduInfo->id & 0x07FF )  ;				//// 11 bits or 29 bits to go
			psMsgObject->ui32MsgLen = PduInfo->length ;
			psMsgObject->pui8MsgData = PduInfo->sdu ;
			psMsgObject->ui32MsgIDMask = 0 ;							//  no filtering mode .. maybe later


			/*
			 * the type of the CAN_write() function is obviously Tx
			 */
			eMsgType = MSG_OBJ_TYPE_TX ;


			/////// stop protecting your stuff man !
			HTH_Semaphore[Hth] = 0 ; 											//// release me

			/*
			 * check if the controller is busy
			 */

			if (HWREG(uiBase + CAN_O_IF1CRQ) & CAN_IF1CRQ_BUSY)
			{
				returnVal = CAN_BUSY ;
			}

			else {

				/*
				 *
				 * call the Tivaware function now !
				 */
				CANMessageSet( uiBase , ui32ObjID, psMsgObject, eMsgType  );
				Saved_swPduHandle = PduInfo->swPduHandle ;						//// save the
				returnVal = E_OK ;

			}

		}
		else															/////// CAN Is busy
		{
			returnVal = CAN_BUSY ;
		}



	}

	return returnVal ;
}




/*---------------------------------------------------------------------
Function Name:  <Can_MainFunction_Mode>
Service ID:     <0x0c>
Description:
- Scheduled function.
- This function performs the polling of CAN controller mode transitions.
- it implements the polling of CAN status register flags to detect transition of CAN Controller state.
----------------------------------------------------------------------*/
void Can_MainFunction_Mode( void )
{
    uint8 ControllerIndex;
    uint32 ui32Base;


    for (ControllerIndex = 0 ; ControllerIndex < NO_OF_CONTROLLERS_IN_HW  ; ControllerIndex++  )
    {
        switch (ControllerIndex)
        {
			case 0: ui32Base = CAN0_BASE ;break;
			case 1: ui32Base = CAN1_BASE ;break;
        }

        if (HWREG(ui32Base + CAN_O_CTL) & CAN_CTL_INIT)								////// the controller is in intialization state
        {
            if (g_Config_Ptr->CanControllers[ControllerIndex].ControllerStatus == CAN_CS_SLEEP)
            {
                CanIf_ControllerModeIndication(ControllerIndex,CAN_CS_SLEEP);				/// callback function
            }
            else
            {
                CanIf_ControllerModeIndication(ControllerIndex,CAN_CS_STOPPED);
            }
        }
        else
        {
        	if (g_Config_Ptr->CanControllers[ControllerIndex].CanControllerErrorState == CAN_ERRORSTARE_BUSOFF )
        		CanIf_ControllerModeIndication(ControllerIndex,CAN_CS_UINIT);

        	else
        		CanIf_ControllerModeIndication(ControllerIndex,CAN_CS_STARTED);
        }
    }

}

