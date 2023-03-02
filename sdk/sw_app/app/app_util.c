/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_util.c
 * @brief	application utility functions
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>		//# fork...
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>	//# waitpid
#include <sys/socket.h>
#include <sys/vfs.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "dev_common.h"
#include "app_comm.h"
#include "app_set.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static const unsigned int crc32_tab[] = {
         0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
         0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
         0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
         0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
         0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
         0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
         0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
         0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
         0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
         0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
         0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
         0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
         0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
         0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
         0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
         0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
         0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
         0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
         0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
         0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
         0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
         0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
         0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
         0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
         0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
         0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
         0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
         0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
         0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
         0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
         0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
         0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
         0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
         0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
         0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
         0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
         0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
         0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
         0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
         0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
         0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
         0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
         0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

static int uinput_fd = -1;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    util mem_alloc/free function
* @section  [desc]
*****************************************************************************/
void *util_mem_alloc(unsigned int msize)
{
	void *ptr;

	//--- memory alloc ------------------------------------
	ptr = malloc (msize);

	return ptr;
}

void util_mem_free(void *ptr)
{
	//--- memory free ------------------------------------
	free (ptr);
}

/*****************************************************************************
* @brief    util memory copy function
* @section  [desc]
*****************************************************************************/
int util_mem_copy(void *pDes, void *pSrc, int size)
{
    if (pDes == NULL || pSrc == NULL){
        return -1;
    }

	memcpy(pDes, pSrc, size);

    return 0;
}

/*****************************************************************************
 *   @brief    util disk info function
 *   @section  DESC Description
 *   - desc
 ******************************************************************************/
int util_disk_info(disk_info_t *idisk, const char *path)
{
	struct statvfs s;

	if (statvfs(path, &s) != 0) {
		TRACE_INFO("%s fault\n", path);
		return -1;
	}

	idisk->total = (s.f_blocks * (s.f_bsize/KB));
	idisk->avail = (s.f_bavail * (s.f_bsize/KB));
	idisk->used =  ((s.f_blocks - s.f_bfree) * (s.f_bsize/KB));
	
	//TRACE_INFO("cur disk block size %ld frag size %ld\n", s.f_bsize, s.f_frsize);
	//TRACE_INFO("cur disk total %ld(KB), avail %ld(KB), used %ld(KB)\n", idisk->total, idisk->avail, idisk->used);

    return 0;
}

/*****************************************************************************
* @brief    system exec function
* @section  [desc]
*****************************************************************************/
int util_sys_exec(char *arg)
{
    int numArg, i, j, k;
    int len, status;

    char exArg[10][64];
	pid_t chId;

    if (arg[0] == '\0')
        return 0;

    j = 0; 	k = 0;
	len = strlen(arg);

    for (i = 0; i < len; i++) {
        if (arg[i] == ' ') {
		    exArg[j][k] = '\0';
		    j ++; k = 0;
		} else {
		    exArg[j][k] = arg[i];
		    k ++;
		}
	}

    if (exArg[j][k - 1] == '\n') {
	    exArg[j][k - 1] = '\0';
	} else {
	    exArg[j][k] = '\0';
	}

	numArg = j + 1;

	if (numArg > 10) {
	    TRACE_INFO("The no of arguments are greater than 10" \
	    		"calling standard system function...\n");
	    return (system(arg));
	}

    chId = fork();
	if (chId == 0) {
	    // child process
	    switch (numArg) {
	    case 1:
	        execlp(exArg[0],exArg[0],NULL);
	        break;
	    case 2:
	        execlp(exArg[0],exArg[0],exArg[1],NULL);
	        break;
	    case 3:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],NULL);
	        break;
	    case 4:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],NULL);
	        break;
	    case 5:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               NULL);
	        break;
	    case 6:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               exArg[5],NULL);
	        break;
	    case 7:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               exArg[5],exArg[6],NULL);
	        break;
	    case 8:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               exArg[5],exArg[6],exArg[7],NULL);
	        break;
	    case 9:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               exArg[5],exArg[6],exArg[7],exArg[8],NULL);
	        break;
	    case 10:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               exArg[5],exArg[6],exArg[7],exArg[8],exArg[9],NULL);
	        break;
		}
        TRACE_INFO("execlp failed...\n");
	    exit(0);
	} else if(chId < 0) {
		TRACE_INFO("Failed to create child process\n");
		return -1;
	} else {
		/* parent process */
		/* wait for the completion of the child process */
		/* 3th option WNOHANG->non-block 0->block */
		waitpid(chId, &status, 0);
		#if 0
		if (WIFEXITED(status))
			TRACE_INFO("Chiled exited with the code %d\n", WEXITSTATUS(status));
		else
			TRACE_INFO("Child terminated abnormally..\n");
		#endif
	}

    return 0;
}

/*****************************************************************************
* @brief    console menu function
* @section  [desc]
*****************************************************************************/
char menu_get_cmd(void)
{
	char buf[32];

	fgets(buf, 32, stdin);

	return (buf[0]);
}

/*****************************************************************************
* @brief    time trace function
* @section  [act] 1 : start time trace
*                 0 : end time trace and print elapsed time
*****************************************************************************/
static unsigned long btime=0;
void __time_trace(int act)
{
	unsigned long elapsed_time;

	if(act) {
		btime = app_get_time();
	}
	else {
		elapsed_time = app_get_time() - btime;
		TRACE_INFO("--- %ld ms\n", elapsed_time);
		btime += elapsed_time;
	}
}

/*
* crc32()는 CRC32 값을 구하는 함수
*
* 파라미터:
*   crc : 처음 호출할 때에는 0, 데이터가 길어서 이어서 호출할 경우에는 이전에 return된 CRC값
*   buf : CRC값을 구할 데이터
*   size : buf의 크기
*
* return : CRC값
*/
unsigned int util_gen_crc32(unsigned int crc, const void *buf, unsigned int size)
{
    const unsigned char *p = buf;

    crc = ~crc;
    while (size--)
    {
        crc = crc32_tab[(crc ^ *p++) & 0xff] ^ (crc >> 8);
    }
    return crc ^ ~0U;
}

//# added by rupy..
void util_hexdump(char *p, int n)
{
	int i, off;

	for (off = 0; n > 0; off += 16, n -= 16) {
		TRACE_INFO("%s%04x:", off == 0 ? "\n" : "", off);
		i = (n >= 16 ? 16 : n);
		do {
			TRACE_INFO(" %02x", *p++ & 0xff);
		} while (--i);
		TRACE_INFO("\n");
	}
}

/* Convert C from hexadecimal character to integer.  */
int hextobin(unsigned char c)
{
	switch (c)
	{
		default: return c - '0';
		case 'a': case 'A': return 10;
		case 'b': case 'B': return 11;
		case 'c': case 'C': return 12;
		case 'd': case 'D': return 13;
		case 'e': case 'E': return 14;
		case 'f': case 'F': return 15;
	}
}

/* referenced from echo.c */
int utf8_unescape(const char *dst, char *src)
{
	char *data_buf = (char *)dst;
	char const *s = src;
	unsigned char c;
	int xfred = 0;
	
	if ((data_buf == NULL) || (s == NULL)) {
		return -1;
	}
	
	while ((c = *s++))
	{
		if (c == '\\' && *s)
		{
			switch (c = *s++) 
			{
				case 'x':
				{
					unsigned char ch = *s;
					
					if (!isxdigit(ch)) {
						/* not an escape */
						data_buf[xfred] = '\\';
						xfred++;
					}
					s++;
					c = hextobin(ch);
					ch = *s;
					if (isxdigit(ch)) {
						s++;
						c = c * 16 + hextobin(ch);
					} 
					
				}
				break;
				case '0':
					c = 0;
					if (!('0' <= *s && *s <= '7'))
						break;
					c = *s++;
				/* Fall through.  */
				case '1': case '2': case '3':
				case '4': case '5': case '6': case '7':
					c -= '0';
					if ('0' <= *s && *s <= '7')
						c = c * 8 + (*s++ - '0');
					if ('0' <= *s && *s <= '7')
						c = c * 8 + (*s++ - '0');
				break;
				case '\\': 
				break;
				default:  
					//putchar('\\');
					data_buf[xfred] = '\\';
					xfred++; 
				break;
			}
		}
		//putchar (c);
		data_buf[xfred] = c;
		xfred++;	
	}
	
	return 0;
}

//# ------------------------------------------------------------------------------------------------------------------
/*
 * linux usermode input. (virtual Key)
 */
int uinput_init(void)
{				
	struct uinput_user_dev uidev;
	 
	uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if (uinput_fd < 0)
		return -1;
		
	ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY); 
	ioctl(uinput_fd, UI_SET_KEYBIT, KEY_SPACE);
	
	memset(&uidev, 0, sizeof(uidev));
  	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "Simple Keypad");  
	
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor  = 0x1234;
	uidev.id.product = 0x4321;
	uidev.id.version = 2;

	write(uinput_fd, &uidev, sizeof(uidev));
	ioctl(uinput_fd, UI_DEV_CREATE);
	
	return 0;
}

/*
 * usermode input write key
 */
int uinput_emit_key(int type, int code, int val)
{
	struct input_event ie;
	
	if (uinput_fd < 0)
		return -1;
		
	ie.type = type;
	ie.code = code;
	ie.value = val; // press
	/* timestamp values below are ignored */
	ie.time.tv_sec = 0;
	ie.time.tv_usec = 0;

   	write(uinput_fd, &ie, sizeof(ie));
	   
	return 0;
}

/*
 * linux usermode input exit.
 */
int uinput_exit(void)
{			
	if (uinput_fd < 0)
		return -1;
			
	ioctl(uinput_fd, UI_DEV_DESTROY);
	close(uinput_fd);
		
	return 0;
}

/**
* @brief Initialize message queue.

* Initialize message queue.
* @note This API must be used before use any other message driver API.
* @param msgKey [I ] Key number for message queue and share memory.
* @return message ID.
*/
int Msg_Init(int msgKey)
{
	int qid;
	key_t key = msgKey;

	qid = msgget(key,0);
	if (qid < 0) {
		qid = msgget(key,IPC_CREAT|0666);
		TRACE_INFO("Creat queue id: %d\n", qid);
	}

	TRACE_INFO("queue id: %d\n", qid);

	return qid;
}

/**
* @brief Send message .

* Excute send message command.
* @param qid [I ] Message ID.
* @param *pdata [I ] pointer to the data.
* @param size [I ] Data size.
* @return send data to message queue.
*/
int Msg_Send(int qid, void *pdata, int size)
{
	return msgsnd(qid, pdata, size - sizeof(long), 0);///< send msg1
}

/**
* @brief Receive message .

* Excute receive message command.
* @param qid [I ] Message ID.
* @param msg_type [I ] Message type.
* @param *pdata [I ] pointer to the data.
* @param size [I ] Data size.
* @return receive data from message queue.
*/
int Msg_Rsv( int qid, int msg_type, void *pdata , int size )
{
	return msgrcv( qid, pdata, size-sizeof(long), msg_type, 0);
}
/**
* @brief Send and receive message .

* Excute send and receive message command.
* @param qid [I ] Message ID.
* @param msg_type [I ] Message type.
* @param *pdata [I ] pointer to the data.
* @param size [I ] Data size.
* @return receive data from message queue if send success.
*/
int Msg_Send_Rsv( int qid, int msg_type, void *pdata , int size )
{
	int ret = 0;
	
	ret = msgsnd( qid, pdata, size-sizeof(long), 0 );/* send msg1 */
	if (ret < 0) {
		return ret;
	}
	
	return msgrcv( qid, pdata, size-sizeof(long), msg_type, 0);
}

/**
* @brief Kill message queue.

* Kill message queue.
* @param qid [I ] Message ID.
* @retval 0 Queue killed.
*/
int Msg_Kill(int qid)
{
	msgctl(qid, IPC_RMID, NULL);

	TRACE_INFO("Kill queue id: %d\n", qid);

	return 0;
}
