/******************************************************************************
 * NEXTT360 Board
 * Copyright by LF, Incoporated. All Rights Reserved.
 * based on gpsd.
 *---------------------------------------------------------------------------*/
 /**
 * @file    fifo.h
 * @brief
 * @section MODIFY history
 *     - 2020.07.21 : First Created
 */
/*****************************************************************************/

#ifndef __FIFO_H__
#define __FIFO_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int FIFO_init(FIFO *fifo, int address, int len);
int FIFO_get(FIFO *fifo, unsigned int addr, int size); 
int FIFO_put(FIFO *fifo, unsigned int addr, unsigned int size); 
int FIFO_isEmpty(FIFO *fifo);
int FIFO_isFull(FIFO *fifo); 
int FIFO_clear(FIFO *fifo); 

#endif	/* __FIFO_H__ */
