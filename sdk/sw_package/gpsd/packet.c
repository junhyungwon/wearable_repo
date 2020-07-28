/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_msg.c
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2014.10.01 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#include <time.h>
#include <stdint.h>
#include <termios.h>

#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

static char *state_table[] = {
	"GROUND_STATE",	/* we don't know what packet type to expect */

	"COMMENT_BODY",	/* pound comment for a test load */
	"COMMENT_RECOGNIZED",	/* comment recognized */
	"NMEA_DOLLAR",		/* we've seen first character of NMEA leader */

	"NMEA_BANG",		/* we've seen first character of an AIS message '!' */
	"NMEA_PUB_LEAD",	/* seen second character of NMEA G leader */
	"NMEA_VENDOR_LEAD",	/* seen second character of NMEA P leader */
	"NMEA_LEADER_END",	/* seen end char of NMEA leader, in body */
	"NMEA_PASHR_A",	/* grind through recognizing $PASHR */
	"NMEA_PASHR_S",	/* grind through recognizing $PASHR */
	"NMEA_PASHR_H",	/* grind through recognizing $PASHR */
	"NMEA_BINARY_BODY",	/* Ashtech-style binary packet body, skip until \r\n */
	"NMEA_BINARY_CR",	/* \r on end of Ashtech-style binary packet */
	"NMEA_BINARY_NL",	/* \n on end of Ashtech-style binary packet */
	"NMEA_CR",	   	/* seen terminating \r of NMEA packet */
	"NMEA_RECOGNIZED",	/* saw trailing \n of NMEA packet */
};

enum {
	GROUND_STATE,	/* we don't know what packet type to expect */

	COMMENT_BODY,	/* pound comment for a test load */
	COMMENT_RECOGNIZED,	/* comment recognized */
	NMEA_DOLLAR,		/* we've seen first character of NMEA leader */

	NMEA_BANG,		/* we've seen first character of an AIS message '!' */
	NMEA_PUB_LEAD,	/* seen second character of NMEA G leader */
	NMEA_VENDOR_LEAD,	/* seen second character of NMEA P leader */
	NMEA_LEADER_END,	/* seen end char of NMEA leader, in body */
	NMEA_PASHR_A,	/* grind through recognizing $PASHR */
	NMEA_PASHR_S,	/* grind through recognizing $PASHR */
	NMEA_PASHR_H,	/* grind through recognizing $PASHR */
	NMEA_BINARY_BODY,	/* Ashtech-style binary packet body, skip until \r\n */
	NMEA_BINARY_CR,	/* \r on end of Ashtech-style binary packet */
	NMEA_BINARY_NL,	/* \n on end of Ashtech-style binary packet */
	NMEA_CR,	   	/* seen terminating \r of NMEA packet */
	NMEA_RECOGNIZED,	/* saw trailing \n of NMEA packet */
};

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static void packet_accept(struct gps_lexer_t *lexer, int packet_type)
/* packet grab succeeded, move to output buffer */
{
    size_t packetlen = lexer->inbufptr - lexer->inbuffer;

    if (packetlen < sizeof(lexer->outbuffer)) {
		memcpy(lexer->outbuffer, lexer->inbuffer, packetlen);
		lexer->outbuflen = packetlen;
		lexer->outbuffer[packetlen] = '\0';
		lexer->type = packet_type;
		
		if (0)
		{
			char scratchbuf[MAX_PACKET_LENGTH*4+1];
			dprintf("Packet type %d accepted %zu = %s\n",
				packet_type, packetlen,
				gpsd_packetdump(scratchbuf,  sizeof(scratchbuf),
						(char *)lexer->outbuffer,
						lexer->outbuflen));
		}
    } else {
		dprintf("Rejected too long packet type %d len %zu\n",
			packet_type, packetlen);
    }
}

static void packet_discard(struct gps_lexer_t *lexer)
/* shift the input buffer to discard all data up to current input pointer */
{
    size_t discard = lexer->inbufptr - lexer->inbuffer;
    size_t remaining = lexer->inbuflen - discard;
    lexer->inbufptr = memmove(lexer->inbuffer, lexer->inbufptr, remaining);
    lexer->inbuflen = remaining;
    
	#if 0
	{
		char scratchbuf[MAX_PACKET_LENGTH*4+1];
		GPSD_LOG(LOG_RAW + 1, &lexer->errout,
			"Packet discard of %zu, chars remaining is %zu = %s\n",
			discard, remaining,
			gpsd_packetdump(scratchbuf, sizeof(scratchbuf),
						(char *)lexer->inbuffer, lexer->inbuflen));
    }
	#endif
}

bool str_starts_with(const char *str, const char *prefix)
{
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

static bool character_pushback(struct gps_lexer_t *lexer, unsigned int newstate)
{
    --lexer->inbufptr;
    --lexer->char_counter;
    lexer->state = newstate;
	
    {
		unsigned char c = *lexer->inbufptr;
		dprintf("%08ld: character '%c' [%02x]  pushed back, state set to %s\n",
			lexer->char_counter,
			(isprint((int)c) ? c : '.'), c,
			state_table[lexer->state]);
    }

    return false;
}

static void character_discard(struct gps_lexer_t *lexer)
{
    memmove(lexer->inbuffer, lexer->inbuffer + 1, (size_t)-- lexer->inbuflen);
    lexer->inbufptr = lexer->inbuffer;
    
	{
		char scratchbuf[MAX_PACKET_LENGTH*4+1];
		dprintf("Character discarded, buffer %zu chars = %s\n",
			lexer->inbuflen,
			gpsd_packetdump(scratchbuf, sizeof(scratchbuf),
						(char *)lexer->inbuffer, lexer->inbuflen));
    }
}

static bool nextstate(struct gps_lexer_t *lexer, unsigned char c)
{
    static int n = 0;
	
    n++;
    switch (lexer->state) {
    case GROUND_STATE:
		n = 0;
		if (c == '#') {
			lexer->state = COMMENT_BODY;
			break;
		}
		if (c == '$') {
			lexer->state = NMEA_DOLLAR;
			break;
		}
		if (c == '!') {
			lexer->state = NMEA_BANG;
			break;
		}
		break;
    case COMMENT_BODY:
		if (c == '\n')
			lexer->state = COMMENT_RECOGNIZED;
		else if (!isprint(c))
			return character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_DOLLAR:
		if (c == 'G')
			lexer->state = NMEA_PUB_LEAD;
		else if (c == 'P')	/* vendor sentence */
			lexer->state = NMEA_VENDOR_LEAD;
		else {
			(void) character_pushback(lexer, GROUND_STATE);
		}
		break;
    case NMEA_PUB_LEAD:
		/*
		* $GP == GPS, $GL = GLONASS only, $GN = mixed GPS and GLONASS,
		* according to NMEA (IEIC 61162-1) DRAFT 02/06/2009.
		* We have a log from China with a Beidou device using $GB
		* rather than $BD.
		*/
		if (c == 'B' || c == 'P' || c == 'N' || c == 'L' || c == 'A')
			lexer->state = NMEA_LEADER_END;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
	case NMEA_VENDOR_LEAD:
		if (c == 'A')
			lexer->state = NMEA_PASHR_A;
		else if (isalpha(c))
			lexer->state = NMEA_LEADER_END;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_PASHR_A:
		if (c == 'S')
			lexer->state = NMEA_PASHR_S;
		else if (isalpha(c))
			lexer->state = NMEA_LEADER_END;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_PASHR_S:
		if (c == 'H')
			lexer->state = NMEA_PASHR_H;
		else if (isalpha(c))
			lexer->state = NMEA_LEADER_END;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_PASHR_H:
		if (c == 'R')
			lexer->state = NMEA_BINARY_BODY;
		else if (isalpha(c))
			lexer->state = NMEA_LEADER_END;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_BINARY_BODY:
		if (c == '\r')
			lexer->state = NMEA_BINARY_CR;
		break;
    case NMEA_BINARY_CR:
		if (c == '\n')
			lexer->state = NMEA_BINARY_NL;
		else
			lexer->state = NMEA_BINARY_BODY;
		break;
    case NMEA_BINARY_NL:
		if (c == '$')
			(void) character_pushback(lexer, NMEA_RECOGNIZED);
		else
			lexer->state = NMEA_BINARY_BODY;
		break;
    case NMEA_BANG:
		return character_pushback(lexer, GROUND_STATE);
    case NMEA_LEADER_END:
		if (c == '\r')
			lexer->state = NMEA_CR;
		else if (c == '\n')
			/* not strictly correct, but helps for interpreting logfiles */
			lexer->state = NMEA_RECOGNIZED;
		else if (c == '$') {
			(void) character_pushback(lexer, GROUND_STATE);
		} else if (!isprint(c))
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_CR:
		if (c == '\n')
			lexer->state = NMEA_RECOGNIZED;
		/*
		* There's a GPS called a Jackson Labs Firefly-1a that emits \r\r\n
		* at the end of each sentence.  Don't be confused by this.
		*/
		else if (c == '\r')
			lexer->state = NMEA_CR;
		else
			(void) character_pushback(lexer, GROUND_STATE);
		break;
    case NMEA_RECOGNIZED:
		if (c == '#')
			lexer->state = COMMENT_BODY;
		else if (c == '$')
			lexer->state = NMEA_DOLLAR;
		else if (c == '!')
			lexer->state = NMEA_BANG;
		else
			return character_pushback(lexer, GROUND_STATE);
		break;
    }

    return true;	/* no pushback */
}

void lexer_init(struct gps_lexer_t *lexer)
{
    lexer->char_counter = 0;
    lexer->retry_counter = 0;
    lexer->start_time.tv_sec = 0;
    lexer->start_time.tv_nsec = 0;
    packet_reset(lexer);
}

void packet_reset(struct gps_lexer_t *lexer)
{
    lexer->type = BAD_PACKET;
    lexer->state = GROUND_STATE;
    lexer->inbuflen = 0;
    lexer->inbufptr = lexer->inbuffer;
}

void packet_parse(struct gps_lexer_t *lexer)
{
    lexer->outbuflen = 0;
    
	while (packet_buffered_input(lexer) > 0) 
	{
		unsigned char c = *lexer->inbufptr++;
		unsigned int oldstate = lexer->state;
	
		if (!nextstate(lexer, c))
			continue;
#if 0	
		dprintf("%08ld: character '%c' [%02x], %s -> %s\n",
			lexer->char_counter, (isprint(c) ? c : '.'), c,
			state_table[oldstate], state_table[lexer->state]);
#endif		
		lexer->char_counter++;

		if (lexer->state == GROUND_STATE) {
			character_discard(lexer);
		} else if (lexer->state == COMMENT_RECOGNIZED) {
			packet_accept(lexer, COMMENT_PACKET);
			packet_discard(lexer);
			lexer->state = GROUND_STATE;
			break;
		}
		else if (lexer->state == NMEA_RECOGNIZED) {
			/*
			* $PASHR packets have no checksum. Avoid the possibility
			* that random garbage might make it look like they do.
			*/
			if (!str_starts_with((const char *)lexer->inbuffer, "$PASHR,"))
			{
				bool checksum_ok = true;
				char csum[3] = { '0', '0', '0' };
				char *end;
				/*
				* Back up past any whitespace.  Need to do this because
				* at least one GPS (the Firefly 1a) emits \r\r\n
				*/
				for (end = (char *)lexer->inbufptr - 1; isspace((unsigned char) *end); end--)
					continue;
				while (strchr("0123456789ABCDEF", *end))
					--end;
				if (*end == '*') {
					unsigned int n, crc = 0;
					for (n = 1; (char *)lexer->inbuffer + n < end; n++)
					crc ^= lexer->inbuffer[n];
					(void)snprintf(csum, sizeof(csum), "%02X", crc);
					checksum_ok = (csum[0] == toupper((unsigned char) end[1])
						&& csum[1] == toupper((unsigned char) end[2]));
				}
				if (!checksum_ok) {
					dprintf("bad checksum in NMEA packet; expected %s.\n", csum);
					packet_accept(lexer, BAD_PACKET);
					lexer->state = GROUND_STATE;
					packet_discard(lexer);
					break;    /* exit case */
				}
	    	}
			/* checksum passed or not present */
			packet_accept(lexer, NMEA_PACKET);
			packet_discard(lexer);
			break;
		}
    }	/* while */
}

ssize_t packet_get(int fd, struct gps_lexer_t *lexer)
{
    ssize_t recvd;

    errno = 0;
    recvd = read(fd, lexer->inbuffer + lexer->inbuflen,
		 sizeof(lexer->inbuffer) - (lexer->inbuflen));
    if (recvd == -1) {
		if ((errno == EAGAIN) || (errno == EINTR)) {
			dprintf("no bytes ready\n");
			recvd = 0;
		} else {
			eprintf("errno: %s\n", strerror(errno));
			return -1;
		}
    }  else {
		#if 0
		{
			char scratchbuf[MAX_PACKET_LENGTH*4+1];
			dprintf("Read %zd chars to buffer offset %zd (total %zd): %s\n",
				recvd, lexer->inbuflen, lexer->inbuflen + recvd,
				gpsd_packetdump(scratchbuf, sizeof(scratchbuf),
						(char *)lexer->inbufptr, (size_t) recvd));
		}
		#endif
		lexer->inbuflen += recvd;
    }
	
    dprintf("packet_get() fd %d -> %zd (%d)\n", fd, recvd, errno);

    if (recvd <= 0 && packet_buffered_input(lexer) <= 0) {
		dprintf("no data from gps device!!\n");
		return recvd;
	}
	
	dprintf("inbufled len %d before parse!!\n", lexer->inbuflen);
	
    packet_parse(lexer);
	
	dprintf("inbufled len %d after parse!!\n", lexer->inbuflen);
	
    /* if input buffer is full, discard */
    if (sizeof(lexer->inbuffer) == (lexer->inbuflen)) {
		/* coverity[tainted_data] */
		packet_discard(lexer);
		lexer->state = GROUND_STATE;
		dprintf("full data from gps device!!\n");
    }

    /*
     * If we gathered a packet, return its length; it will have been
     * consumed out of the input buffer and moved to the output
     * buffer.  We don't care whether the read() returned 0 or -1 and
     * gathered packet data was all buffered or whether it was partly
     * just physically read.
     *
     * Note: this choice greatly simplifies life for callers of
     * packet_get(), but means that they cannot tell when a nonzero
     * return means there was a successful physical read.  They will
     * thus credit a data source that drops out with being alive
     * slightly longer than it actually was.  This is unlikely to
     * matter as long as any policy timeouts are large compared to
     * the time required to consume the greatest possible amount
     * of buffered input, but if you hack this code you need to
     * be aware of the issue. It might also slightly affect
     * performance profiling.
     */
    if (lexer->outbuflen > 0)
		return (ssize_t) lexer->outbuflen;
    else
		/*
		* Otherwise recvd is the size of whatever packet fragment we got.
		* It can still be 0 or -1 at this point even if buffer data
		* was consumed.
		*/
		return recvd;
}
