/******************************************************************************
 * FITT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_main.c
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2017.03.22 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 main thread function - use only communication
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
int main(int argc, char **argv)
{
	struct uinput_user_dev uidev;
	struct input_event ie;
	int fd = -1;
	
	printf("--- FITT360 Record Process Start ---\n\n");
	
	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);\
	if (fd < 0) {
    	printf("Opening of uinput failed!\n");
    	return 1;
  	}
	
	ioctl(fd, UI_SET_EVBIT, EV_KEY); 
	ioctl(fd, UI_SET_KEYBIT, KEY_SPACE);
	  
	memset(&uidev, 0, sizeof(uidev));
  	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "Simple Keypad");  
	
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor  = 0x1234;
	uidev.id.product = 0x4321;
	uidev.id.version = 2;

	if (write(fd, &uidev, sizeof(uidev)) < 0) //writing settings
  	{
    	printf("error: write");
    	return 1;
  	}
  	
	if(ioctl(fd, UI_DEV_CREATE) < 0) //writing ui dev create
  	{
    	printf("error: ui_dev_create");
    	return 1;
  	}
	  
	ie.type = EV_KEY;
	ie.code = KEY_SPACE;
	ie.value = 1; // press
	/* timestamp values below are ignored */
	ie.time.tv_sec = 0;
	ie.time.tv_usec = 0;

   	write(fd, &ie, sizeof(ie));
	   
	ie.type = EV_KEY;
	ie.code = KEY_SPACE;
	ie.value = 0; // release

   	write(fd, &ie, sizeof(ie));       
	
	ie.type = EV_SYN;
	ie.code = 0;
	ie.value = 0; // release

   	write(fd, &ie, sizeof(ie));    
	      
	while(1);   
	
	ioctl(fd, UI_DEV_DESTROY);
	close(fd);
	
	return 0;
}
