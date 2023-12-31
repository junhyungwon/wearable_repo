/******************************************************************************
 * XBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_ftp.c
 * @brief
 * @author  hwjun
 * @section MODIFY history
 *     - 2017.06.16 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glob.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <dirent.h>
#include <ctype.h>

#include <fcntl.h>
#include <errno.h>

#include "app_comm.h"
#include "app_ftp.h"
#include "app_dev.h"
#include "app_msg.h"
#include "app_main.h"
#include "app_rec.h"
#include "app_set.h"
#include "dev_common.h"
#include "app_file.h"
#include "app_version.h"
#include "app_ctrl.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#if (FTP_CUR_DEV == FTP_DEV_ETH0)
#define FTP_DEV_NAME		"eth0"
#elif (FTP_CUR_DEV == FTP_DEV_ETH1)
#define FTP_DEV_NAME		"eth1"
#else
#error "invalid ftp device"
#endif

#define FTP_CYCLE_TIME		100
#define FTP_CONNECT_TIME_OUT		2	//# 5 sec
#define FTP_RETRY_CNT       5
#define MSIZE 8192*8  // buffer size
#define RSIZE 1024
//#define USE_SSL             

X509* server_cert ;
X509_NAME *certname = NULL ;


/*----------------------------------------------------------------------------
 Declares variable)s
-----------------------------------------------------------------------------*/
static app_ftp_t t_ftp;
static app_ftp_t *iftp=&t_ftp;

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

void  ssl_info_callback(const SSL *s, int where, int ret)
{
	char * writeString;
	int w;

	w = where & ~SSL_ST_MASK;

	if (w & SSL_ST_CONNECT)
		writeString="SSL_connect";
	else if (w & SSL_ST_ACCEPT)
		writeString="SSL_accept";
	else
		writeString="undefined";

	fprintf(stderr, "======== writeString = [%s]\n", writeString);

	if (where & SSL_CB_LOOP)
	{
		fprintf(stderr, "======== writeString = [%s], SSL_state_string_long(s) = [%s]\n",
				writeString, SSL_state_string_long(s));
	}
	else if (where & SSL_CB_ALERT)
	{
		if (ret == 0)
		{
			fprintf(stderr,"======== writeString = [%s], SSL_state_string_long(s) = [%s]\n",
					writeString, SSL_state_string_long(s));
		}
		else if (ret < 0)
		{
			fprintf(stderr,"======== writeString = [%s], SSL_state_string_long(s) = [%s]\n",
					writeString, SSL_state_string_long(s));
		}
	}
}



static int ftpRecvResponse(int sock, char * buf, int length)
{
    socklen_t lon ;
    int valopt = 0 ;
	int i, len;
    getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
    if(valopt)
    {
        return -1 ;
    }
		
#if defined(USE_SSL)
	if(!length)
		len = SSL_read(iftp->lsslHandle, (void *)buf, RSIZE) ;
	else
		len = SSL_read(iftp->lsslHandle, (void *)buf, length) ;

	ftp_dbg("SSL_READ Len = %d .....using lssHandle = %d\n", len, iftp->lsslHandle) ;
#else
	if(!length)
		len = recv(sock, buf, RSIZE, 0);
	else
		len =recv(sock, buf, length, 0) ;
#endif 

	if (len == -1) {//receive the data
		perror("recv");
		return -1;
	}

	for (i=(len - 1); i>0; i--) {
		if (buf[i]=='.' || buf[i]=='\r') {
	 		buf[i+1]='\0';
	 		break;
		}
	}

	printf("%d,%s\n", len, buf); //print response to the screen

	return 0;
}

static int ftpNewCmd(int sock, char * buf, char * cmd, char * param)
{
    socklen_t lon ;
    int valopt = 0 ;

	strcpy(buf,cmd);
    if(strcmp(cmd, "CDUP"))
    {
	    if (strlen(param) > 0) {
		    strcat(buf," ");
		    strcat(buf,param);
	    }
    }

	strcat(buf,"\r\n");
	printf("ftpNewCmd *%s", buf); //print the cmd to the screen

    getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
    if(valopt)
    {
        return -1 ;
    }
    else
    {
#if defined(USE_SSL)
	    if (SSL_write(iftp->lsslHandle, (void *)buf, strlen(buf)) == -1) {
#else
	    if (send(sock, buf, strlen(buf), 0) == -1) {
#endif
		    perror("send");
		    return -1;
	    }
		else
			ftp_dbg("SSL_WRITE .......\n") ;
    }  

	//clear the buffer
	return 0;
}

void connection_check(int sd)
{
    char buf[RSIZE] ;

	if(ftpRecvResponse(sd, buf, 0) == 0) {  //wait for ftp server to start talking
		if(strncmp(buf,"220",3) == 0) {  //make sure it ends with a 220
			ftp_dbg("SSL_connection check %s.......\n",buf) ;
		}
	}
}


static int ftpConvertAddy(char * buf, char * hostname, int * port)
{
	unsigned int i,t = 0;
	int flag = 0, decCtr = 0;
	int tport1 = 0, tport2 = 0;
	char tmpPort[6];

	//example data in quotes below:
	//"227 Listening on (149,122,52,162,4,20)"
	//4 * 256 + 20 = 1044
	for (i = 0; i < strlen(buf); i++) {
		if (buf[i]=='(') {
			flag = 1;
			i++;
		}

		if (buf[i]==')') {
			hostname[t]='\0';
			break;
		}

		if (flag==1) {
			if (buf[i] != ',') {
				hostname[t]=buf[i];
				t++;
			} else {
				hostname[t]='.';
				t++;
			}
		}
	}

	t = 0;
	for (i=0; i<strlen(hostname); i++) {
		if (hostname[i]=='.')
			decCtr++;

		if (decCtr==4 && hostname[i]!='.') {
			tmpPort[t]=hostname[i];
			t++;

			if (hostname[i+1]=='.') {
				tmpPort[t]='\0';
				tport1=atoi(tmpPort);
				t=0;
			}
		}

		if (decCtr==5 && hostname[i]!='.') {
			tmpPort[t]=hostname[i];
			t++;

			if (hostname[i+1]=='\0') {
				tmpPort[t]='\0';
				tport2 = atoi(tmpPort);
				t=0;
			}
		}
	}

	*port = tport1 * 256 + tport2;
	decCtr=0;

	for (i = 0;i < strlen(hostname);i++) {
		if (hostname[i]=='.') {
			decCtr++;
		}
		if (decCtr > 3)
			hostname[i]='\0';
	}

	ftp_dbg("host [%s,%d]\n", hostname, *port); //print response to the screen

	return 0;
}

static int ftp_transfer_file(int sd, char *filename)
{
	FILE *in;
	char buf[MSIZE];
	size_t len;
    int retval = 0, valopt = 0, ret = 0 ;
    socklen_t lon ;

	in = fopen(filename, "rb");
	if (in == NULL)
		return (-1);

	while ((len = fread(buf, sizeof(char), MSIZE, in)) != 0)
	{
        if(iftp->ftp_state != FTP_STATE_SENDING)
        {
            break ;
        }

        getsockopt(sd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
        if(valopt)
        {
            retval = -1 ;
            break ;
        }  
        else
        {

#if defined(USE_SSL)
	    	ret = SSL_write(iftp->dsslHandle, (void *)buf, len) ; 
	    	if (ret <= 0) {
				switch(SSL_get_error(iftp->dsslHandle, ret))
				{
					case SSL_ERROR_NONE :
						ftp_dbg("SSL_ERROR_NONE\n");
						break ;
					case SSL_ERROR_WANT_READ : 
						ftp_dbg("SSL_ERROR_WANT_READ\n") ;
						break ;
					case SSL_ERROR_WANT_X509_LOOKUP :
					    ftp_dbg("SSL_ERROR_WANT_X509_LOOKUP\n") ; 
						break ;
					case SSL_ERROR_WANT_WRITE :
						ftp_dbg("SSL_ERROR_WANT_WRITE\n") ;
						break ;
					case SSL_ERROR_ZERO_RETURN :
						ftp_dbg("SSL_ERROR_ZERO_RETURN\n") ;
						break ;
					case SSL_ERROR_SYSCALL :
						ftp_dbg("SSL_ERROR_SYSCALL\n") ;
						break ;

				}
				perror("send using SSL") ;
				retval = -1 ;
				break ;
			}
#else
			if (send(sd, buf, len, MSG_NOSIGNAL) == -1) {        
	 		 	perror("send");
  	          	retval = -1 ;
  	          	break ;
			}
#endif			
		}
		
        OSA_waitMsecs(5) ;
	}
	fclose(in);

	return retval;
}

static int createDataSock(char * host, int port)
{
	int sd, reuse = 1;

	struct sockaddr_in pin;
	struct ifreq interface ;
    struct timeval tv ;
    struct linger stLinger ;

    int keepalive = 1, keepcnt = 1, keepidle = 1, keepintvl = 1 ;

    stLinger.l_onoff = 1 ;
    stLinger.l_linger = 0 ;
	
	ftp_dbg("createDataSock [host = %s,port = %d]\n", host, port); 
 	memset(&pin, 0, sizeof(pin));
	pin.sin_family = AF_INET;
	pin.sin_addr.s_addr = inet_addr(host);
	pin.sin_port = htons(port);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
	    perror("socket");
		return -1;
    }

    tv.tv_sec = FTP_CONNECT_TIME_OUT;
    tv.tv_usec = 0;

    strncpy(interface.ifr_ifrn.ifrn_name, FTP_DEV_NAME, IFNAMSIZ);
    setsockopt (sd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) ;
    setsockopt (sd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) ;
    setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse)) ;

    setsockopt (sd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&interface, sizeof(interface)) ;
    setsockopt (sd, SOL_TCP,    TCP_KEEPCNT, &keepcnt, sizeof(keepcnt)) ;
    setsockopt (sd, SOL_TCP,    TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) ;
    setsockopt (sd, SOL_TCP,    TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl)) ;

    setsockopt (sd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof (keepalive)) ;
    setsockopt (sd, IPPROTO_TCP, SO_KEEPALIVE, &keepalive, sizeof (keepalive)) ;
//    setsockopt (sd, SOL_SOCKET, SO_LINGER, &stLinger, sizeof(stLinger)) ;


	if (connect(sd,(struct sockaddr *)  &pin, sizeof(pin)) == -1) {
		perror("connect");
		close(sd); //close the socket
		return -1;
	}

	return sd ;
}
static int ftpSendFile(int sd, char *buf)
{
	if (ftp_transfer_file(sd, buf) != 0) {
		perror("send");
		close(sd); //close the socket
		return -1;
	}

	close(sd); //close the socket

	return 0;
}

static int ftp_change_dir(int sd, char *dirname, int day_dir)
{
    char buf[RSIZE] ;
 
    if(day_dir > 0)
    {
        if(ftpNewCmd(sd, buf, "CDUP", "") != 0)
            return -1 ;

        if(ftpRecvResponse(sd, buf, 0) != 0)
            return -1;

        if(ftpNewCmd(sd, buf, "CDUP", "") != 0)
            return -1 ;

        if(ftpRecvResponse(sd, buf, 0) != 0)
            return -1;

		return 0 ;
    }

    if(ftpNewCmd(sd, buf, "CWD", dirname) != 0)
        return -1 ;

    if(ftpRecvResponse(sd, buf, 0) != 0)
        return -1;

    if(strncmp(buf, "250", 3) != 0) // do not exist
    {
        if(ftpNewCmd(sd, buf, "MKD", dirname) != 0) 
            return -1 ;

        if(ftpRecvResponse(sd, buf, 0) != 0)
            return -1 ;

        if(strncmp(buf, "257", 3) == 0) // create success
        {
            if(ftpNewCmd(sd, buf, "CWD", dirname) != 0)
                return -1 ;

            if(ftpRecvResponse(sd, buf, 0) != 0)
                return -1 ;

            if(strncmp(buf, "250", 3) == 0) // chdir success
            {
                return 0 ; 
            }
        }        
    }
    else
        return 0 ;
    
    return -1 ;

}

static int ftp_send_file(int sd, char *filename)
{
    char tmpHost[100];
    int tmpPort, data_sock;
    char buf[RSIZE];
	char source_fname[64], dest_fname[64] ;

#if defined(USE_SSL)
	if(ftpNewCmd(sd, buf, "PROT", "P") != 0)
		return -1 ;
	if(ftpRecvResponse(sd, buf, 0) != 0)
		return -1 ;
    if(strncmp(buf,"200",3) != 0)
		return -1 ;
#endif 

	if(ftpNewCmd(sd, buf, "TYPE", "I") != 0)
		return -1 ;
	if(ftpRecvResponse(sd, buf, 0) != 0)
		return -1 ;
    if(strncmp(buf,"200",3) != 0)
		return -1 ;

	if(ftpNewCmd(sd, buf, "PASV", "") != 0)
        return -1 ;

	if(ftpRecvResponse(sd, buf, 0) != 0)
        return -1 ;

	if (strncmp(buf, "227", 3) == 0) 
	{
		ftpConvertAddy(buf, tmpHost, &tmpPort);

#if defined(USE_SSL)
        iftp->dsdFtp = createDataSock(tmpHost, tmpPort) ;
		SSL_Create(1) ;
		if(iftp->dsdFtp > 0)
		{
//			connection_check(iftp->dsdFtp) ;
#else		
        data_sock = createDataSock(tmpHost, tmpPort) ;
		if(data_sock > 0)
		{
#endif
			sprintf(source_fname, "_%s",&filename[strlen(iftp->path) + 1]) ;
			sprintf(dest_fname, "%s",&filename[strlen(iftp->path) + 1]) ;

		    if(ftpNewCmd(sd,buf,"STOR", source_fname) != 0)
                return -1 ;

		    if(ftpRecvResponse(sd, buf, 0) != 0)
            {
                return -1 ;
            }

	        ftp_dbg("ftpRecvResponse After STORE = %s\n", buf);
		    if (strncmp(buf, "150", 3) == 0) 
			{ //make sure response is a 150
#if defined(USE_SSL)
			    if (ftpSendFile(iftp->dsdFtp, filename) == 0) 
#else
			    if (ftpSendFile(data_sock, filename) == 0) 
#endif
				{
				    if(ftpRecvResponse(sd, buf, 0) == 0)
					{
				        if (strncmp(buf, "226", 3) == 0) 
					    {
					        printf("%s transfer completed.\n", source_fname);

							if(ftpNewCmd(sd,buf,"RNFR", source_fname) != 0)
								return -1 ;

							if(ftpRecvResponse(sd, buf, 0) == 0)
							{		
								if (strncmp(buf, "350", 3) == 0) 
								{
									if(ftpNewCmd(sd,buf,"RNTO", dest_fname) != 0)
										return -1 ;

									if(ftpRecvResponse(sd, buf, 0) == 0)
									{		
										if (strncmp(buf, "250", 3) == 0)
										{
											printf("Rename from %s to %s \n",source_fname, dest_fname) ;
										    return 0 ;
										}
										else if(strncmp(buf, "553", 3) == 0)
										{
											ftp_dbg("destination file exist !!\n") ;
											if(ftpNewCmd(sd,buf,"DELE", source_fname) == 0)
                                            {
												ftp_dbg("delete file on FTP Server\n") ;  //one time try delete file on FTP SERVER 
											}
											return 1 ;
										}
										else
											return -1 ;
									}
								}
							}	
							else
								return -1 ;
				        }
					}
					else
					{
						return -1 ;
					}
				}	
			}
		}
	}

	return -1;
}

static int ftp_login(int sd, char *username, char *password)
{
    char buf[RSIZE];

    if (ftpNewCmd(sd, buf, "USER", username) == 0) {  //issue the command to login
        if(ftpRecvResponse(sd,buf, 0) == 0) {  //wait for response
            if(strncmp(buf,"331",3) == 0) {  //make sure response is a 331
                if(ftpNewCmd(sd,buf,"PASS",password) == 0) {  //send your password
                    if(ftpRecvResponse(sd,buf, 0) == 0) { //wait for response
                        if(strncmp(buf,"230",3) == 0) { //make sure its a 230
                                        return 0;
                        }
                    }
                }
            }
        }
    }

    return -1;
}

static int ftp_connect(char *hostname, int port)
{
	char buf[RSIZE];
	int sd, res, reuse = 1;
	struct timeval tv;
    int keepalive = 1, keepcnt = 1, keepidle = 1, keepintvl = 1 ;

	struct ifreq interface ;
	struct sockaddr_in pin;
	struct hostent *hp;
#if 0	
	socklen_t lon;
	fd_set myset;
	int valopt;
#endif
	
    if ((hp = gethostbyname(hostname)) == 0) 
	{
	    perror ("gethostbyname");
	    return -1;
    }

    memset (&pin, 0, sizeof(pin));
    pin.sin_family 		= AF_INET;
    pin.sin_addr.s_addr = ((struct in_addr *) (hp->h_addr))->s_addr;
    pin.sin_port 		= htons(port);
	
	//#--- create socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return -1;
	}

#if 0
	//#--- set non-blocking
	arg = fcntl(sd, F_GETFL, NULL);
	arg |= O_NONBLOCK;
	fcntl(sd, F_SETFL, arg);
#endif

    tv.tv_sec = FTP_CONNECT_TIME_OUT;
    tv.tv_usec = 0;

    strncpy(interface.ifr_ifrn.ifrn_name, FTP_DEV_NAME, IFNAMSIZ);

    setsockopt (sd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) ;
    setsockopt (sd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) ;
    setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse)) ;

    setsockopt (sd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&interface, sizeof(interface)) ;
    setsockopt (sd, SOL_TCP,    TCP_KEEPCNT, &keepcnt, sizeof(keepcnt)) ;
    setsockopt (sd, SOL_TCP,    TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) ;
    setsockopt (sd, SOL_TCP,    TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl)) ;

    setsockopt (sd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof (keepalive)) ;
    setsockopt (sd, IPPROTO_TCP, SO_KEEPALIVE, &keepalive, sizeof (keepalive)) ; 

	//# trying to connect with timeout
	res = connect(sd, (struct sockaddr *)&pin, sizeof(pin));
	if(res < 0)
	{
/*		
		if (errno == EINPROGRESS)
		{
			tv.tv_sec = FTP_TIME_OUT;
	        tv.tv_usec = 0;
	        FD_ZERO(&myset);
	        FD_SET(sd, &myset);

	        if(select(sd+1, NULL, &myset, NULL, &tv) > 0)
			{
	           lon = sizeof(int);
	           getsockopt(sd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
	           if (valopt) {
	              ftp_dbg("Error in connection() %d - %s\n", valopt, strerror(valopt));
	           }
	        } else {
	        	ftp_dbg("Timeout or error() %d - %s\n", valopt, strerror(valopt));
        	}
		} else {
        	ftp_dbg("Error connecting %d - %s\n", errno, strerror(errno));
   	 	}
*/
		goto ftp_err;
	}

#if 0
	//# set to blocking mode again...
	arg = fcntl(sd, F_GETFL, NULL);
	arg &= (~O_NONBLOCK);
	fcntl(sd, F_SETFL, arg);
#endif

#if defined(USE_SSL)
	return sd ;
#endif 

	if(ftpRecvResponse(sd, buf, 0) == 0) {  //wait for ftp server to start talking
		if(strncmp(buf,"220",3) == 0) {  //make sure it ends with a 220
			return sd;
		}
	}

ftp_err:
	shutdown(sd, SHUT_RDWR);
	return -1;
}

static int ftp_close(int sd)
{
	char buf[RSIZE];

	ftpNewCmd(sd, buf, "QUIT", "");
	ftpRecvResponse(sd, buf, 0);

	if (strncmp(buf, "221", 3) == 0) {
		printf("FTP OK.\n");
	}

#if defined(USE_SSL)
	SSL_free(iftp->lsslHandle) ;
	iftp->lsdFtp = -1 ;
	SSL_CTX_free(iftp->dsslContext) ;
#endif

	close(sd); //close the socket
	return 0;
}

static int ftp_data_close(int sd)
{
#if defined(USE_SSL)
	SSL_free(iftp->dsslHandle) ;
	iftp->dsdFtp = -1 ;
	SSL_CTX_free(iftp->dsslContext) ;
#endif
	close(sd) ;
    return 0 ;

}
static int get_netdev_link_status(void)
{
#if (FTP_CUR_DEV == FTP_DEV_ETH0)	
	int state = app_cfg->ste.b.cradle_net_run;
#elif (FTP_CUR_DEV == FTP_DEV_ETH1)
	int state = app_cfg->ste.b.usbnet_run;
#else
#error "invalid ftp device"
#endif	
	return (state ? 1: 0);
}				 								 													    

void SSL_Create(int sock_type)
{
	int ret = 0 ;
	char *str ;
	// Register the error strings for libcrypto &libssl

    signal(SIGPIPE, SIG_IGN) ;

	if(!sock_type)
	{
	 	SSL_library_init() ;
		SSLeay_add_ssl_algorithms();
		SSL_load_error_strings() ;
	
	// Register the available ciphers and digests
	// New context saying we are a client, and using SSL 2 or 3

//		iftp->lsslContext = SSL_CTX_new(TLSv1_2_client_method()) ;
		iftp->lsslContext = SSL_CTX_new(TLS_method()) ;
		if(iftp->lsslContext == NULL)
			ERR_print_errors_fp (stderr) ;

	// Disabling SSLv2 will leaave v3 and TLSv1 for negotiation

//		SSL_CTX_set_options(iftp->lsslContext, SSL_OP_NO_TLSv1_2) ;
		SSL_CTX_set_options(iftp->lsslContext, SSL_OP_NO_SSLv2) ;
		SSL_CTX_set_info_callback(iftp->lsslContext, ssl_info_callback) ;

		if(SSL_CTX_use_certificate_file(iftp->lsslContext, CLIENT_CERTF, SSL_FILETYPE_PEM) <= 0) // 공개키 ?
		{
			ERR_print_errors_fp(stderr) ;
			exit(3) ;
		}
		if(SSL_CTX_use_PrivateKey_file(iftp->lsslContext, CLIENT_KEYF, SSL_FILETYPE_PEM) <= 0) // 개인키(RSA)
		{
			ERR_print_errors_fp(stderr) ;
			exit(4) ;
		}

		if(!SSL_CTX_check_private_key(iftp->lsslContext))
		{
			ERR_print_errors_fp(stderr) ;
			fprintf(stderr, "Private key does not match the cerificate public key\n") ;
			exit(5) ;
		}
	
		if(!SSL_CTX_load_verify_locations(iftp->lsslContext, CLIENT_CA_CERTF, NULL))  // 공인인증서(CA = .crt 파일)
		{
			ERR_print_errors_fp(stderr) ;
			exit(1) ;
		}

		SSL_CTX_set_verify(iftp->lsslContext, SSL_VERIFY_PEER, NULL) ;
		SSL_CTX_set_verify_depth(iftp->lsslContext, 1);


	// Create an SSL struct for the connection
		iftp->lsslHandle = SSL_new(iftp->lsslContext) ;
		if(iftp->lsslHandle == NULL)
			ERR_print_errors_fp(stderr) ;

	// Connect the SSL struct to our connection 
		if(!SSL_set_fd(iftp->lsslHandle, iftp->lsdFtp))
			ERR_print_errors_fp(stderr);

	// Initiate SSL handshake
//	if(SSL_connect(iftp->lsslHandle) != 1)
		ret = SSL_connect(iftp->lsslHandle) ;
	    if(ret != 1)
		{
			ftp_dbg("SSL_connect.. FAIL ret = %d\n",ret);
			int error = SSL_get_error(iftp->lsslHandle, ret) ;
			printf("SSL_connect error no = %d\n",error) ;
			ERR_print_errors_fp(stderr);
			SSL_free(iftp->lsslHandle) ;
			SSL_CTX_free(iftp->lsslContext) ;
			close(iftp->lsdFtp) ;
		}
		else
		{
			ftp_dbg("SSL_connect.. OK");
			printf("SSL_connect.. OK\n") ;
			printf("SSL connection using %s\n", SSL_get_cipher(iftp->lsslHandle)) ;
			printf("SSL connection using %s\n", SSL_CIPHER_get_name(SSL_get_current_cipher(iftp->lsslHandle)));

			SSL_CTX_set_cipher_list(iftp->lsslContext, SSL_get_cipher(iftp->lsslHandle)) ;

			server_cert = SSL_get_peer_certificate (iftp->lsslHandle) ;

			if(server_cert != NULL)
			{
				if(SSL_get_verify_result(iftp->lsslHandle) == X509_V_OK)
					printf("client verification with SSL_get_verify_result() succeeded. \n") ;
	            else
					printf("client verification with SSL_get_verify_result() failed.\n") ;

				printf("Server cerificate:\n") ;

				str = X509_NAME_oneline(X509_get_subject_name (server_cert), 0, 0) ;
				printf("\t subject: %s\n",str) ;

				OPENSSL_free(str) ;

				str = X509_NAME_oneline(X509_get_issuer_name (server_cert), 0, 0) ;
				printf("\t issuer: %s\n",str) ;
			
				OPENSSL_free(str) ;

				certname = X509_NAME_new() ;
				certname = X509_get_subject_name(server_cert) ;

				X509_free(server_cert) ;

			}
			else
			{
				printf("Server certificated fail..\n") ;
				SSL_free(iftp->lsslHandle) ;
			}
		}
	}
	else
	{
//	 	SSL_library_init() ;
//		SSLeay_add_ssl_algorithms();
//		SSL_load_error_strings() ;

		iftp->dsslContext = SSL_CTX_new(TLS_method()) ;
		if(iftp->dsslContext == NULL)
			ERR_print_errors_fp (stderr) ;
	// Disabling SSLv2 will leaave v3 and TLSv1 for negotiation

		SSL_CTX_set_options(iftp->dsslContext, SSL_OP_NO_SSLv2) ;
//		SSL_CTX_set_info_callback(iftp->dsslContext, ssl_info_callback) ;
		if(SSL_CTX_use_certificate_file(iftp->dsslContext, CLIENT_CERTF, SSL_FILETYPE_PEM) <= 0) // 공개키 ?
		{
			ERR_print_errors_fp(stderr) ;
			exit(3) ;
		}
		if(SSL_CTX_use_PrivateKey_file(iftp->dsslContext, CLIENT_KEYF, SSL_FILETYPE_PEM) <= 0) // 개인키(RSA)
		{
			ERR_print_errors_fp(stderr) ;
			exit(4) ;
		}
		if(!SSL_CTX_check_private_key(iftp->dsslContext))
		{
			ERR_print_errors_fp(stderr) ;
			fprintf(stderr, "Private key does not match the cerificate public key\n") ;
			exit(5) ;
		}
	
		if(!SSL_CTX_load_verify_locations(iftp->dsslContext, CLIENT_CA_CERTF, NULL))  // 공인인증서(CA = .crt 파일)
		{
			ERR_print_errors_fp(stderr) ;
			exit(1) ;
		}

		SSL_CTX_set_verify(iftp->dsslContext, SSL_VERIFY_PEER, NULL) ;
		SSL_CTX_set_verify_depth(iftp->dsslContext, 1);

	// Create an SSL struct for the connection
		iftp->dsslHandle = SSL_new(iftp->dsslContext) ;
		if(iftp->dsslHandle == NULL)
			ERR_print_errors_fp(stderr) ;

	// Connect the SSL struct to our connection 
		if(!SSL_set_fd(iftp->dsslHandle, iftp->dsdFtp))
			ERR_print_errors_fp(stderr);

	// Initiate SSL handshake
		ret = SSL_connect(iftp->dsslHandle) ;
	    if(ret != 1)
		{
			ftp_dbg("SSL_connect.. FAIL ret = %d\n",ret);
			int error = SSL_get_error(iftp->dsslHandle, ret) ;
			printf("SSL_connect error no = %d\n",error) ;
			ERR_print_errors_fp(stderr);
		}
		else
		{
			ftp_dbg("SSL_connect.. OK");
			printf("SSL_connect.. OK\n") ;
			printf("SSL connection using %s\n", SSL_get_cipher(iftp->dsslHandle)) ;
			printf("SSL connection using %s\n", SSL_CIPHER_get_name(SSL_get_current_cipher(iftp->dsslHandle)));

			SSL_CTX_set_cipher_list(iftp->dsslContext, SSL_get_cipher(iftp->dsslHandle)) ;

			server_cert = SSL_get_peer_certificate (iftp->dsslHandle) ;

			if(server_cert != NULL)
			{
				if(SSL_get_verify_result(iftp->dsslHandle) == X509_V_OK)
					printf("client verification with SSL_get_verify_result() succeeded. \n") ;
	            else
					printf("client verification with SSL_get_verify_result() failed.\n") ;

				printf("Server cerificate:\n") ;

				str = X509_NAME_oneline(X509_get_subject_name (server_cert), 0, 0) ;
				printf("\t subject: %s\n",str) ;

				OPENSSL_free(str) ;

				str = X509_NAME_oneline(X509_get_issuer_name (server_cert), 0, 0) ;
				printf("\t issuer: %s\n",str) ;
			
				OPENSSL_free(str) ;

				certname = X509_NAME_new() ;
				certname = X509_get_subject_name(server_cert) ;

				X509_free(server_cert) ;

			}
			else
				printf("Server certificated fail..\n") ;
		}
	}		
}


static int version_check()
{
	char model_buff[32];	
	char sw_buff[32];	
	int len = 0, ret = 0, retval = 0 ;
	
	sprintf(model_buff,"%s", MODEL_NAME) ;
	sprintf(sw_buff,"%s", FITT360_SW_VER) ;
    len = strlen(model_buff) ;  

ftp_dbg("version check ...\n") ;
ftp_dbg("iftp->fota_firmfname = %s MODEL_NAME = %s\n",iftp->fota_firmfname, model_buff) ;

	if (!strncmp(iftp->fota_firmfname, model_buff, len)) 
	{
	    ret = strncmp(&iftp->fota_firmfname[len + 1], sw_buff, 7) ;
		if(ret == 0)
		{
		    ftp_dbg("The Firmware version is current version \n");
		}
		if(ret == 1)
		{
		    ftp_dbg("The Firmware version is up to date\n") ;
			retval = 1 ;
		}

		if(ret == -1)
		{
		    ftp_dbg("The Firmware version is out of date\n") ;
		}
	}

	// remove conf
	return retval ;

}

static int ftpRecvFile(int sd, char *filename)
{
	FILE *in ;

	char buf[MSIZE] = {0, } ;
	char Iname[64] = {0, } ;
    
	size_t len, fsize = 0;

	int retval = -1;

	sprintf(Iname, "/mmc/%s\n",filename) ;
	Iname[strlen(Iname) - 1] = '\0' ;

	in = fopen((const char *)Iname, "w+") ;
	if(in == NULL)
		return -1 ;

	while(1)
	{
		len = recv(sd, buf, MSIZE, 0);
		if(len > 0)
		{
			fsize += fwrite(buf, 1, len, in) ;
			if(fsize == iftp->fota_filesize)
			{
				retval = 0;
				break ;
			}
			else if(fsize > iftp->fota_filesize)
			{
				retval = -1 ;
				break ;
			}
		}
		else
		{
			retval = -1 ;
			break ;
		}
		OSA_waitMsecs(10) ;
	}
    close(sd) ;

	fclose(in) ;
	return  retval;
}

static int ftpRecvConFile(int sd, char *filename)
{
	char buf[RSIZE] = {0, } ;
    
	char *fstr = NULL, *fs = NULL, *bstr = NULL ;
	size_t len;

	int  retval = S_OK, length = 0 ;

	len = recv(sd, buf, RSIZE, 0) ;

	if(len > 0)
	{
		fs = strstr(&buf[0], "filesize=") ;
		if(fs != NULL)
		{
			fs += 9 ;
		    iftp->fota_filesize = atoi(fs) ;
		}

		fstr = strstr(buf, "dat");
		if(fstr != NULL)
		{
			fstr += 3;
			*fstr = '\0' ;
			length = strlen(buf) ;
		    strncpy(iftp->fota_firmfname, &buf[5], length) ;
        }
        else
		{
			bstr = strstr(buf, "out") ;
			if(bstr != NULL)
			{
				bstr += 3; 
				*bstr = '\0' ;
				length = strlen(buf) ;
				strncpy(iftp->fota_firmfname, &buf[5], length) ;
			}
		}

		retval = 0 ;

	}
	else
		retval = -1 ;

	return retval ;
}

static int fota_receive_firmfile(int sd)
{
	char tmpHost[64] = {0, }, buf[1024] = {0, };
	char RemoteFullPath[64] = {0, } ;
	int tmpPort, retval = -1, data_sock = 0;

	strcpy(RemoteFullPath, "/FOTA/NEXX") ;
 
	ftpNewCmd(sd, buf, "CWD", RemoteFullPath) ;
	ftpRecvResponse(sd, buf, 0) ;

	if(strncmp(buf, "250", 3) != 0)
	{
		retval = -1 ;
	}
	else
	{
		ftpNewCmd(sd, buf, "PASV", "") ;
		ftpRecvResponse(sd, buf, 0) ;

		if(strncmp(buf, "227", 3) == 0)
		{
			ftpConvertAddy(buf, tmpHost, &tmpPort) ;
            
            data_sock = createDataSock(tmpHost, tmpPort) ;
            if(data_sock > 0)
	        {			
				if(ftpNewCmd(sd, buf, "RETR", iftp->fota_firmfname) != 0)  // nexx_firmware
					return -1 ;

				if(ftpRecvResponse(sd, buf, 0) !=0 )
					return -1 ;

				ftp_dbg("ftpRecResponse After RETR = %s\n",buf) ;
				if(strncmp(buf, "150", 3) == 0)
				{
					if(ftpRecvFile(data_sock, iftp->fota_firmfname) == 0)
					{
						if(ftpRecvResponse(sd, buf, 0) == 0)
						{
							if(strncmp(buf, "226", 3) == 0)
							{
								ftp_dbg("%s Receive Done \n",iftp->fota_firmfname) ;
								retval = 0 ;
							}
						}
					}
					else
					{
						retval = -1 ;
					}
				}
            } 
		}
	}
	close(data_sock) ;
	return retval ;
}

static int fota_receive_confile(int sd)
{
	char tmpHost[64] = {0, }, buf[1024] = {0, };
	char RemoteFullPath[64] = {0, } ;
	char model_buff[64] = {0, } ;
	int tmpPort, retval = -1, data_sock = 0, confname_len = 0;

    sprintf(model_buff, "%s.conf", MODEL_NAME) ;
	confname_len = strlen(model_buff) ;

	strcpy(RemoteFullPath, "/FOTA/NEXX") ;

	ftpNewCmd(sd, buf, "CWD", RemoteFullPath) ;
	ftpRecvResponse(sd, buf, 0) ;

	if(strncmp(buf, "250", 3) != 0)
	{
		retval = -1 ;
	}
	else
	{
		if(ftpNewCmd(sd, buf, "TYPE", "I") != 0)
			return -1 ;
		if(ftpRecvResponse(sd, buf, 0) != 0)
			return -1 ;
		if(strncmp(buf,"200",3) != 0)
			return -1 ;
	
		ftpNewCmd(sd, buf, "PASV", "") ;
		ftpRecvResponse(sd, buf, 0) ;

		if(strncmp(buf, "227", 3) == 0)
		{
			ftpConvertAddy(buf, tmpHost, &tmpPort) ;
            
            data_sock = createDataSock(tmpHost, tmpPort) ;
            if(data_sock > 0)
	        {			
				if(ftpNewCmd(sd, buf, "RETR", app_set->fota_info.confname) != 0)  // nexx_series.conf
					return -1 ;

				if(ftpRecvResponse(sd, buf, 73 + confname_len) !=0 )  // filezilla server 0.9.2xxx 버전의 response 오류 로 모델별 사이즈를 receive 하고 다음 receive에서 다음 command 처리 
					return -1 ;

				ftp_dbg("ftpRecResponse After RETR = %s\n",buf) ;
				if(strncmp(buf, "150", 3) == 0)
				{
					if(ftpRecvConFile(data_sock, app_set->fota_info.confname) == 0)
					{
						if(ftpRecvResponse(sd, buf, 0) == 0) // 1021 line의 confname_len 길이 없이 receive 시  1029 line의 receive시 data가 없어 resource is not available .. 에러 발생 
						{
							if(strncmp(buf, "226", 3) == 0)
							{
								ftp_dbg("%s Fota info file receive completed\n", app_set->fota_info.confname) ;
								retval = 0 ;
							}
						}
					}
					else
					{
						retval = -1 ;
					}
				}
            } 
		}
	}
	close(data_sock) ;
	return retval ;
}

int fota_proc()
{
	int retry_cnt = 0, ret = 0, retval = 0 ;
	char cmd[64] = {0, } ;

    iftp->fota_state = FOTA_STATE_CONNECTING ;

	while(iftp->fota_state == FOTA_STATE_CONNECTING)
	{
		if(!app_cfg->ftp_enable)
		{
			iftp->fota_state = FOTA_STATE_NONE ;
			ftp_dbg(" \n[FOTA] FOTA stop process == FOTA DISABLED!! \n") ;
			break ;
		}

		//#--- ping check for ftp IP
		if(iftp->lsdFtp >= 0)
			close(iftp->lsdFtp);

		// ethX off in ftp running
		if(!get_netdev_link_status()) { 	
			iftp->fota_state = FOTA_STATE_NONE;
			break;
		}

		if(app_set->fota_info.type) // use manual input ipaddress
			iftp->lsdFtp = ftp_connect(app_set->fota_info.ipaddr, app_set->fota_info.port);
		else
			iftp->lsdFtp = ftp_connect(app_set->ftp_info.ipaddr, app_set->ftp_info.port);

		if(iftp->lsdFtp != -1)
		{
#if defined(USE_SSL)
			SSL_Create(0) ;
			connection_check(iftp->lsdFtp) ;
#endif
		 //read result
			while(1)
			{
				if(app_set->fota_info.type) // use manual input ipaddress
		        	ret = ftp_login(iftp->lsdFtp, app_set->fota_info.id, app_set->fota_info.pwd);
				else
				    ret = ftp_login(iftp->lsdFtp, app_set->ftp_info.id, app_set->ftp_info.pwd);

	            if(ret == 0)
                {
			        iftp->fota_state = FOTA_STATE_RECEIVE_INFO;
                    retry_cnt = 0 ;
					break ;
                }
			    else
                {     
                    if(retry_cnt >= FTP_RETRY_CNT)
                    {
                        iftp->fota_state = FOTA_STATE_RECEIVE_DONE ;
                        retry_cnt = 0 ;
						ftp_close(iftp->lsdFtp);
					    break ;
                    }
                    else
                        retry_cnt += 1 ;

			        ftp_dbg("fota login failure ID:%s, PWD:%s \n",app_set->fota_info.id,app_set->fota_info.pwd);
                    app_leds_eth_status_ctrl(LED_FTP_ERROR);
                }
			}
		}
		else 
        {
            if(retry_cnt >= FTP_RETRY_CNT)
            {
                iftp->fota_state = FOTA_STATE_RECEIVE_DONE ;
                retry_cnt = 0 ;
				break ;
            }
            retry_cnt += 1 ;

		    ftp_dbg("fota connection failure ip:%s port = %d\n", app_set->fota_info.ipaddr, app_set->fota_info.port);
            app_leds_eth_status_ctrl(LED_FTP_ERROR);
		}
		app_msleep(FTP_CYCLE_TIME);
	}

	if (iftp->fota_state == FOTA_STATE_RECEIVE_INFO)
	{
		if(fota_receive_confile(iftp->lsdFtp) == 0)
		{
			iftp->fota_state = FOTA_STATE_RECEIVE_FIRM ;
		    ftp_dbg("receive fota_configure file\n");
			sysprint("Receive Done Fota configure file !!!\n");
		}
		else
		{
			iftp->fota_state = FOTA_STATE_RECEIVE_DONE ;
			ftp_close(iftp->lsdFtp) ;
			ftp_dbg("%s Receive Fail \n",app_set->fota_info.confname) ;
			sysprint("Did not receive Fota configure file !!!\n");
		}

		sprintf(cmd, "/mmc/%s",app_set->fota_info.confname) ;
		if(access(cmd, F_OK) == 0)
		{
			remove(cmd);
			sync() ;
		}
	}

	if (iftp->fota_state == FOTA_STATE_RECEIVE_FIRM)
	{
		if(version_check())
		{
			if(fota_receive_firmfile(iftp->lsdFtp) == 0)
			{
				iftp->fota_state = FOTA_STATE_RECEIVE_DONE ;
				ftp_dbg("Receive Remote firmware file\n");
				sysprint("Receive Done Remote Firmware file !!!\n");
				retval = 1 ;
			}
			else
			{
				sprintf(cmd, "/mmc/%s",iftp->fota_firmfname) ;
				if(access(cmd, F_OK) == 0)
				{
					remove(cmd);
					sync() ;
					ftp_dbg("%s Receive Fail, Delete unperfect file \n",iftp->fota_firmfname) ;
				}

				iftp->fota_state = FOTA_STATE_RECEIVE_DONE ;
				sysprint("Receive Fail Remote Firmware file !!!\n");
			}

		}
        else  // Same version or older version
		{

			sprintf(cmd, "/mmc/%s",app_set->fota_info.confname) ;
			if(access(cmd, F_OK) == 0)
			{
				remove(cmd);
				ftp_dbg("%s Receive Fail, Delete unperfect file \n",app_set->fota_info.confname) ;
			}
			sysprint("tried fw update with Same version or older verion firmware !!!\n");
		}


		// rm confile, firmware file
		ftp_close(iftp->lsdFtp) ;
		ftp_data_close(iftp->dsdFtp) ;
		iftp->fota_state = FOTA_STATE_RECEIVE_DONE ;
	}

    return retval ;
}


static void ftp_send(void)
{
	int i=0;
	int ret = 0, retry_cnt = 0, file_cnt = 0, retval = 0;
    char FileName[MAX_CHAR_128] ;
    char temp[10] = {0, } ;

	iftp->ftp_state = FTP_STATE_CONNECTING;

	while(iftp->ftp_state == FTP_STATE_CONNECTING)
	{
		if(!app_cfg->ftp_enable)
		{
			iftp->ftp_state = FTP_STATE_NONE;
			ftp_dbg(" \n[ftp] FTP stop process -- FTP DISABLED!! \n");
			break;
		}

		//#--- ping check for ftp IP
		if(iftp->lsdFtp >= 0)
			close(iftp->lsdFtp);

		// ethX off in ftp running
		if(!get_netdev_link_status()) { 	
			iftp->ftp_state = FTP_STATE_NONE;
			break;
		}
 
		iftp->lsdFtp = ftp_connect(app_set->ftp_info.ipaddr, app_set->ftp_info.port);
		if(iftp->lsdFtp != -1)
		{
#if defined(USE_SSL)
			SSL_Create(0) ;
			connection_check(iftp->lsdFtp) ;
#endif
            retry_cnt = 0 ;
		 //read result
			while(1)
			{
		        ret = ftp_login(iftp->lsdFtp, app_set->ftp_info.id, app_set->ftp_info.pwd);
			ftp_dbg(" \n[ftp] FTP login ret == %d !! \n", ret);
	            if(ret == 0)
                {
			        iftp->ftp_state = FTP_STATE_SENDING;
                    retry_cnt = 0 ;
					break ;
                }
			    else
                {     
                    if(retry_cnt >= FTP_RETRY_CNT)
                    {
                        iftp->ftp_state = FTP_STATE_SEND_DONE ;
                        retry_cnt = 0 ;
	           			ftp_close(iftp->lsdFtp);
					    break ;
                    }
                    else
                        retry_cnt += 1 ;

			        ftp_dbg("ftp login failure ID:%s, PWD:%s \n",app_set->ftp_info.id,app_set->ftp_info.pwd);
                    app_leds_eth_status_ctrl(LED_FTP_ERROR);
                }
			}
		}
		else 
        {
            if(retry_cnt >= FTP_RETRY_CNT)
            {
                iftp->ftp_state = FTP_STATE_SEND_DONE ;
                retry_cnt = 0 ;
				break ;
            }
            retry_cnt += 1 ;

		    ftp_dbg("ftp connection failure ip:%s port = %d\n", app_set->ftp_info.ipaddr, app_set->ftp_info.port);
            app_leds_eth_status_ctrl(LED_FTP_ERROR);
		}

		app_msleep(FTP_CYCLE_TIME);
	}

	if (iftp->ftp_state == FTP_STATE_SENDING)
	{
        app_leds_backup_state_ctrl(LED_FTP_ON);
        app_leds_eth_status_ctrl(LED_FTP_ON);

		for (i = 0; i < iftp->file_cnt; i++)   // 전체 파일 리스트(event + normal)
        {
            if (get_ftp_send_file(i, FileName) < 0) {  // -1 -> error
			   /* 파일 목록이 많은 경우 keep alive 신호를 못 보냄 */
			   //eprintf("FTP File not found!!\n");
			   OSA_waitMsecs(5);
			   continue ;
			}

			 // ethX off in ftp running
            if (!get_netdev_link_status()) {
				iftp->ftp_state = FTP_STATE_NONE;
                break;
			}

			if (app_set->ftp_info.file_type) { // ftp send event file
			    if (strstr(FileName, "/mmc/DCIM/R_") != NULL) {
					OSA_waitMsecs(5);
					continue ;
				}
			}
			
            strncpy(temp, &FileName[12], 8) ;  // create folder per date ex) /20190823/DeviceID
		    if (app_cfg->ftp_enable && iftp->ftp_state == FTP_STATE_SENDING)
		    {
                if(ftp_change_dir(iftp->lsdFtp, temp, 0) != 0)
                {
                    iftp->ftp_state = FTP_STATE_NONE ;
                }
				else
				{	
                    if(ftp_change_dir(iftp->lsdFtp, app_set->sys_info.deviceId, 0) != 0)
                    {
                        iftp->ftp_state = FTP_STATE_NONE ;
                    }
					else
					{
                        retval = ftp_send_file(iftp->lsdFtp, FileName) ;
						if(retval >= 0)
                        {
							delete_ftp_send_file(FileName) ;
							if(!retval)
								ftp_dbg(" \n[ftp] Send image -- %s \n", FileName);
							else
								ftp_dbg(" \n[ftp] Destination File exist -- %s \n", FileName) ;
						}
						else
                        {
							if (access(FileName, 0) == -1) 
								ftp_dbg(" \n[ftp] Do not exist file name - %s \n", FileName);
							/* delete를 해야 List에서 삭제하도록 변경함 */
							/* 따라서 전송에 실패할 경우 restore 필요없다*/
							//else
							//	restore_ftp_file(FileName) ;

				            ftp_dbg(" \n[ftp] Send Fail image -- %s \n", FileName);
                            break;
						}

                        if(ftp_change_dir(iftp->lsdFtp, temp, 1) != 0)
                        {
                            iftp->ftp_state = FTP_STATE_NONE ;
						}

					}	
                }
		    }
            else
            {
                break ;
            }

        } 

		if(app_set->ftp_info.file_type) // the status after sending files 
		    file_cnt = get_recorded_efile_count() ;
		else
		    file_cnt = get_recorded_file_count() ;

		if(file_cnt == 0)
		{
			ftp_dbg(" \n[ftp] send file Done --\n");
            app_leds_backup_state_ctrl(LED_FTP_OFF);
            app_leds_eth_status_ctrl(LED_FTP_OFF);
		    iftp->ftp_state = FTP_STATE_SEND_DONE;
            ftp_close(iftp->lsdFtp);
			ftp_data_close(iftp->dsdFtp) ;
			ctrl_sys_halt(1) ;
		}
		else
		{
			ftp_dbg(" \n[ftp] File left after sending file --\n");
            app_leds_eth_status_ctrl(LED_FTP_ERROR);
            app_leds_backup_state_ctrl(LED_FTP_ERROR);
            iftp->ftp_state = FTP_STATE_NONE ;
		}

	} /* end of if (iftp->ftp_state == FTP_STATE_SENDING)*/
}

int app_get_ftp_state(void)
{
	return iftp->ftp_state;
}

void app_ftp_state_reset(void)
{
	iftp->ftp_state = FTP_STATE_NONE;
}

/*****************************************************************************
* @brief    ftp main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_ftp(void *prm)
{
	app_thr_obj *tObj = &iftp->ftpObj;
	int cmd, exit = 0, cradle_status = 0, ret = 0, rec_state = 0;
	
	aprintf("enter...\n");
	tObj->active = 1;

    iftp->ftp_state = FTP_STATE_NONE;
    iftp->fota_state = FOTA_STATE_NONE;
    iftp->retry_cnt = 0 ;

//    app_cfg->ste.b.prerec_state = app_set->rec_info.auto_rec ;

	while (!exit)
	{
		//# wait cmd
        cmd = tObj->cmd ;

		if (cmd == APP_CMD_STOP) {
			break;
		}
		
		if(get_netdev_link_status())
        {
			cradle_status = ON ;

            if(iftp->ftp_state == FTP_STATE_NONE)
            {
				rec_state = app_rec_state() ;
				if (rec_state == 1)  // normal rec status
				{
					app_rec_stop(ON);
					OSA_waitMsecs(200); /* wait for file close */
					app_cfg->ste.b.prerec_state = 1 ;
				}
				else if(rec_state == 2)
				{
					app_rec_stop(ON);
					app_sos_send_stop(ON) ;
					OSA_waitMsecs(200); /* wait for file close */
				}

				if(app_set->fota_info.ON_OFF && iftp->fota_state == FOTA_STATE_NONE)
				{
					app_cfg->ste.b.ftp_run = 1 ;  
					ret = fota_proc() ;
					app_cfg->ste.b.ftp_run = 0 ;
				}
				if(!ret)
				{
					if(app_set->ftp_info.file_type) // ftp send event file
					{
						if(get_recorded_efile_count() > 0)
						{
				            ftp_dbg("get_recorded_efile_count = %d\n", get_recorded_efile_count());
						    iftp->file_cnt = get_recorded_file_count() ;
						}
						else
						    iftp->file_cnt = 0 ;
					}
					else
						iftp->file_cnt = get_recorded_file_count() ;

					if (iftp->file_cnt > 0)  
					{
						if(app_cfg->ftp_enable)
						{
							app_leds_eth_status_ctrl(LED_FTP_ON);

							app_cfg->ste.b.ftp_run = 1 ;  // rec key disable

							ftp_send() ;
							save_filelist();
							app_cfg->ste.b.ftp_run = 0 ;
						}
					}
					else  
					{
						app_cfg->ste.b.ftp_run = 0 ;
						iftp->ftp_state = FTP_STATE_SEND_DONE ;
						app_leds_backup_state_ctrl(LED_FTP_OFF);
						app_leds_eth_status_ctrl(LED_FTP_OFF);
					
					}  
				}
				else
				{
					iftp->ftp_state = FTP_STATE_SEND_DONE ; // firmware update done
					break ;
				}
			}
			else
			{		
				app_cfg->ste.b.ftp_run = 0 ;
			}

			// ftp connection 실패시 auto record 설정 On, 및 현재 record 상태 였으면 이전 상태로 돌리기 위한 작업 

			if (app_cfg->ste.b.prerec_state && iftp->ftp_state == FTP_STATE_SEND_DONE)
			{      
				if(!app_rec_state())
				{
					app_rec_start() ;  // rec start after ftp send
					app_cfg->ste.b.prerec_state = 0 ;
				}
			} 
			
		}
		else
		{	
			// cradle에서 분리 되었을 경우 처리
			if(cradle_status == ON) {
				save_filelist();
				cradle_status = OFF ;
			}
			
			iftp->ftp_state = FTP_STATE_NONE ;
			app_cfg->ste.b.ftp_run = 0 ;

			if (app_cfg->ste.b.prerec_state && app_cfg->en_rec) {
				if(!app_rec_state())
				{
					app_rec_start() ;  // rec start after ftp send
					app_cfg->ste.b.prerec_state = 0 ;
				}
			}
		}

		OSA_waitMsecs(1000) ;
	}

    if(ret) 
		ctrl_sys_halt(0) ;

//  reboot for firmware update
    tObj->active = 0;
    aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    ftp thread start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_ftp_init(void)
{
	app_thr_obj *tObj;

	memset(iftp, 0, sizeof(app_ftp_t));

	sprintf(iftp->path , "%s/%s", SD_MOUNT_PATH, REC_DIR);

	if (access(iftp->path, 0) == -1) {
        mkdir(iftp->path, 0775);
        chmod(iftp->path, 0775);
    }

    if(app_set->ftp_info.ON_OFF)
    {
	//# create ftp thread
	    tObj = &iftp->ftpObj;
	    if (thread_create(tObj, THR_ftp, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
		    eprintf("create ftp thread\n");
		    return EFAIL;
	    }
    }

	dprintf(".done!\n");

	return 0;
}

void app_ftp_exit(void)
{
    app_thr_obj *tObj;

	tObj = &iftp->ftpObj;
    app_ftp_state_reset() ;

    if(app_set->ftp_info.ON_OFF)
    {
        event_send(tObj, APP_CMD_STOP, 0, 0);
	    while(tObj->active)
		    app_msleep(20);

	    thread_delete(tObj);
    }

    dprintf(".. done!\n");
}
