/*
 * uartbuffer.h
 *
 *  Created on: Feb 3, 2015
 *      Author: Crazyfingers
 */

#ifndef UARTBUFFER_H_
#define UARTBUFFER_H_

#include "stm32f4xx.h"

typedef volatile struct {
	uint16_t buffer[40];
	uint16_t  nextout;
	uint16_t nextin;
	uint32_t length;
} RxBuffer;



void Rx_BufInit(RxBuffer* buf);
uint16_t Rx_AddToBuf(RxBuffer*, uint16_t);
uint32_t Rx_ReadFromBuf(RxBuffer*);

#endif /* UARTBUFFER_H_ */
