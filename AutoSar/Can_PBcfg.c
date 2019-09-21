/*
 * Can_PBcfg.c
 *
 *  Created on: Sep 20, 2019
 *      Author: Sad MultiVerse
 */

#include "Can.h"

const Can_ConfigType Can_Configurations = {



		/// CanControllers array
		{
				// Can0
				{
						CAN0_ID,					///// can controller identifier
						CAN_CS_STARTED,				////  Controller Status
						CAN_ERRORSTATE_ACTIVE		////  Controller errorState

				},
				// Can1
				{
						CAN1_ID,
						CAN_CS_STARTED,
						CAN_ERRORSTATE_ACTIVE
				}

		},

		///// HardWareObject array of  64 elements :''D
		//// the first 32 element are CAN0 ... then CAN1

		{
				{
						STANDARD,			//// message id type
						1,					//// message Object id
						TRANSMIT,			///  HOH type
						CAN0_ID				//// can controller Identifier
				},
				{
						STANDARD,
						2,
						RECEIVE,
						CAN0_ID
				},
				{
						STANDARD,
						3,
						TRANSMIT,
						CAN0_ID
				},
				{
						STANDARD,
						4,
						TRANSMIT,
						CAN0_ID
				},


				///// the rest of the 64 ..

		}





};
