/******************************************************************************
 * NEXTT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    main.c
 * @brief
 * @section MODIFY history
 *     - 2020.07.08 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gnss_ipc_cmd_defs.h"
#include "fifo.h"

int FIFO_init(FIFO *fifo, int address, int len)
{
	if (fifo == NULL)
		return -1;
	
	fifo->count = 0;
	fifo->readIndex = 0; /* shared mem = offset 4 */
	fifo->writeIndex = 0; /* shared mem = offset 8 */
	fifo->len = len; //# 2048
	fifo->buf = (unsigned char *)address; 
	
	return 0;
}

int FIFO_get(FIFO *fifo, unsigned int addr, int size) 
{
	if ((fifo->len-fifo->readIndex) < size)
		fifo->readIndex = 0;
	
	memcpy((char *)addr, (char *)&fifo->buf[fifo->readIndex], size);
	fifo->readIndex += size;
	if (fifo->readIndex >= fifo->len)
		fifo->readIndex=0;
	fifo->count--;
	return 0;
}

int FIFO_put(FIFO *fifo, unsigned int addr, unsigned int size) 
{
	if ((fifo->len-fifo->writeIndex) < size)
		fifo->writeIndex = 0;
		
	memcpy((char *)&fifo->buf[fifo->writeIndex], (char *)addr, size);
	fifo->writeIndex += size; 
	
	if (fifo->writeIndex >= fifo->len)
		fifo->writeIndex = 0;
	fifo->count++;
	return 0;
}

int FIFO_isEmpty(FIFO *fifo)
{
	if (fifo->count == 0) {
		return 1;
	} else {
		return 0;
	}
}

int FIFO_isFull(FIFO *fifo) 
{
	if (fifo->count >= fifo->len)
		return 1;
	else
		return 0;
}

int FIFO_clear(FIFO *fifo) 
{
	if (fifo == NULL)
		return -1;

	fifo->count		 = 0;
	fifo->readIndex  = 0;
	fifo->writeIndex = 0;
	return 0;
}
