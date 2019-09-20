/*
 * Det.h
 *
 *  Created on: Sep 20, 2019
 *      Author: Sad MultiVerse
 */

#ifndef DET_H_
#define DET_H_


#define  CAN_E_PARAM_POINTER                0x01
#define  CAN_E_PARAM_HANDLE                 0x02
#define  CAN_E_PARAM_DATA_LENGTH            0x03
#define  CAN_E_PARAM_CONTROLLER             0x04
#define  CAN_E_UNINIT                       0x05
#define  CAN_E_TRANSITION                   0x06
#define  CAN_E_PARAM_BAUDRATE               0x07
#define  CAN_E_ICOM_CONFIG_INVALID          0x08
#define  CAN_E_INIT_FAILED                  0x09




void Det_ReportError( uint16 ModuleId,
                      uint8 InstanceId,
                      uint8 ApiId,
					  uint8 ErrorId );


#endif /* DET_H_ */
