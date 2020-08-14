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
#include <pthread.h>

#include "gnss_ipc_cmd_defs.h"
#include "main.h"
#include "fifo.h"

int FIFO_init(FIFO *fifo, int len)
{
	pthread_mutexattr_t mutex_attr;
	int status = 0;
	
	if (fifo == NULL)
		return -1;
	
	fifo->count = 0;
	fifo->readIndex = 0;
	fifo->writeIndex = 0;
	fifo->len = len;
	fifo->buf = (unsigned char *)malloc(len); 
	if (fifo->buf == NULL) {
    	eprintf("failed to init FIFO!\n");
    	return -1;
  	}
	
	status |= pthread_mutexattr_init(&mutex_attr);
	status |= pthread_mutex_init(&fifo->lock, &mutex_attr);
	if (status != 0)
    	eprintf("failed to create FIFO %d!\n", status);
	
  	pthread_mutexattr_destroy(&mutex_attr);
  	
	return 0;
}

int FIFO_get(FIFO *fifo, unsigned int addr, int size) 
{
	pthread_mutex_lock(&fifo->lock);
	
	if ((fifo->len-fifo->readIndex) < size)
		fifo->readIndex = 0;
	
	memcpy((char *)addr, (char *)&fifo->buf[fifo->readIndex], size);
	fifo->readIndex += size;
	if (fifo->readIndex >= fifo->len)
		fifo->readIndex=0;
	fifo->count--;
	
	pthread_mutex_unlock(&fifo->lock);
	
	return 0;
}

int FIFO_put(FIFO *fifo, unsigned int addr, unsigned int size) 
{
	int status = -1;

  	pthread_mutex_lock(&fifo->lock);
  
	if ((fifo->len-fifo->writeIndex) < size)
		fifo->writeIndex = 0;
		
	memcpy((char *)&fifo->buf[fifo->writeIndex], (char *)addr, size);
	fifo->writeIndex += size; 
	
	if (fifo->writeIndex >= fifo->len)
		fifo->writeIndex = 0;
	fifo->count++;
	
	pthread_mutex_unlock(&fifo->lock);
	
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
	free(fifo->buf);
	
	pthread_cond_destroy(&fifo->condRd);
  	pthread_cond_destroy(&fifo->condWr);
  	pthread_mutex_destroy(&fifo->lock);  
  
	return 0;
}
