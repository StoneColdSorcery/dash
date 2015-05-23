/*
 * uartbuffer.c
 *
 *  Created on: Feb 3, 2015
 *      Author: Crazyfingers
 */

#include "uartbuffer.h"



void Rx_BufInit(RxBuffer* buf)
{
	buf->nextin = 0;
	buf->nextout = 0;
	buf->length = 0;
}

uint16_t Rx_AddToBuf(RxBuffer* buf, uint16_t inbyte)
{
	uint16_t newNextin =  (buf->nextin + 1) % 40;
	if(newNextin == buf->nextout) // full
		return 0;  //return false, did not add byte
	else
	{
		buf->buffer[buf->nextin] = inbyte;
		buf->length += 1;
		buf->nextin = newNextin;
		return 1;
	}


}

uint32_t Rx_ReadFromBuf(RxBuffer* buf)
{
	uint32_t rbyte;
	rbyte = buf->buffer[buf->nextout];
	buf->nextout = (buf->nextout + 1) % 40;
	buf->length -= 1;
	return rbyte;
}
