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
#include <errno.h>
#include <termios.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
// true if normalized timespec equal or greater than zero

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
void gpsd_zero_satellites(struct gps_data_t *out)
{
    int sat;

    (void)memset(out->skyview, '\0', sizeof(out->skyview));
    out->satellites_visible = 0;
    
	/* zero is good inbound data for ss, elevation, and azimuth.  */
    /* we need to set them to invalid values */
    for (sat = 0; sat < MAXCHANNELS; sat++ ) {
        out->skyview[sat].azimuth = NAN;
        out->skyview[sat].elevation = NAN;
        out->skyview[sat].ss = NAN;
        out->skyview[sat].freqid = -1;
    }
}

void gps_clear_fix(struct gps_fix_t *fixp)
{
    memset(fixp, 0, sizeof(struct gps_fix_t));
    fixp->altitude = NAN;        // DEPRECATED, undefined
    fixp->altHAE = NAN;
    fixp->altMSL = NAN;
    fixp->climb = NAN;
    fixp->depth = NAN;
    fixp->epc = NAN;
    fixp->epd = NAN;
    fixp->eph = NAN;
    fixp->eps = NAN;
    fixp->ept = NAN;
    fixp->epv = NAN;
    fixp->epx = NAN;
    fixp->epy = NAN;
    fixp->latitude = NAN;
    fixp->longitude = NAN;
    fixp->magnetic_track = NAN;
    fixp->magnetic_var = NAN;
    fixp->mode = MODE_NOT_SEEN;
    fixp->sep = NAN;
    fixp->speed = NAN;
    fixp->track = NAN;
    /* clear ECEF too */
    fixp->ecef.x = NAN;
    fixp->ecef.y = NAN;
    fixp->ecef.z = NAN;
    fixp->ecef.vx = NAN;
    fixp->ecef.vy = NAN;
    fixp->ecef.vz = NAN;
    fixp->ecef.pAcc = NAN;
    fixp->ecef.vAcc = NAN;
    fixp->NED.relPosN = NAN;
    fixp->NED.relPosE = NAN;
    fixp->NED.relPosD = NAN;
    fixp->NED.velN = NAN;
    fixp->NED.velE = NAN;
    fixp->NED.velD = NAN;
    fixp->geoid_sep = NAN;
    fixp->dgps_age = NAN;
    fixp->dgps_station = -1;
}

void gps_clear_dop(struct dop_t *dop)
{
    dop->xdop = dop->ydop = dop->vdop = dop->tdop = dop->hdop = dop->pdop =
    dop->gdop = NAN;
}

void gps_merge_fix(struct gps_fix_t *to, gps_mask_t transfer, struct gps_fix_t *from)
{
    if ((NULL == to) || (NULL == from))
		return;
    
	if ((transfer & TIME_SET) != 0)
		to->time = from->time;
    
	if ((transfer & LATLON_SET) != 0) {
		to->latitude = from->latitude;
		to->longitude = from->longitude;
    }
    
	if ((transfer & MODE_SET) != 0)
		to->mode = from->mode;
    
	if ((transfer & ALTITUDE_SET) != 0) {
		if (0 != isfinite(from->altHAE)) {
			to->altHAE = from->altHAE;
		}
		if (0 != isfinite(from->altMSL)) {
			to->altMSL = from->altMSL;
		}
		if (0 != isfinite(from->depth)) {
			to->depth = from->depth;
		}
    }
	
    if ((transfer & TRACK_SET) != 0)
        to->track = from->track;
    
    if ((transfer & SPEED_SET) != 0)
		to->speed = from->speed;
    if ((transfer & CLIMB_SET) != 0)
		to->climb = from->climb;
    if ((transfer & TIMERR_SET) != 0)
		to->ept = from->ept;
    if (0 != isfinite(from->epx) &&
        0 != isfinite(from->epy)) {
		to->epx = from->epx;
		to->epy = from->epy;
    }
    if (0 != isfinite(from->epd)) {
		to->epd = from->epd;
    }
    if (0 != isfinite(from->eph)) {
	to->eph = from->eph;
    }
    if (0 != isfinite(from->eps)) {
	to->eps = from->eps;
    }
    /* spherical error probability, not geoid separation */
    if (0 != isfinite(from->sep)) {
	to->sep = from->sep;
    }
    /* geoid separation, not spherical error probability */
    if (0 != isfinite(from->geoid_sep)) {
	to->geoid_sep = from->geoid_sep;
    }
    if (0 != isfinite(from->epv)) {
	to->epv = from->epv;
    }
    if ((transfer & SPEEDERR_SET) != 0)
	to->eps = from->eps;
    if ((transfer & ECEF_SET) != 0) {
	to->ecef.x = from->ecef.x;
	to->ecef.y = from->ecef.y;
	to->ecef.z = from->ecef.z;
	to->ecef.pAcc = from->ecef.pAcc;
    }
    if ((transfer & VECEF_SET) != 0) {
	to->ecef.vx = from->ecef.vx;
	to->ecef.vy = from->ecef.vy;
	to->ecef.vz = from->ecef.vz;
	to->ecef.vAcc = from->ecef.vAcc;
    }
    if ((transfer & NED_SET) != 0) {
	to->NED.relPosN = from->NED.relPosN;
	to->NED.relPosE = from->NED.relPosE;
	to->NED.relPosD = from->NED.relPosD;
    }
    if ((transfer & VNED_SET) != 0) {
	to->NED.velN = from->NED.velN;
	to->NED.velE = from->NED.velE;
	to->NED.velD = from->NED.velD;
    }
    if ('\0' != from->datum[0]) {
        strlcpy(to->datum, from->datum, sizeof(to->datum));
    }
    if (0 != isfinite(from->dgps_age) &&
        0 <= from->dgps_station) {
        /* both, or neither */
	to->dgps_age = from->dgps_age;
	to->dgps_station = from->dgps_station;
    }
}

/* mkgmtime(tm)
 * convert struct tm, as UTC, to seconds since Unix epoch
 * This differs from mktime() from libc.
 * mktime() takes struct tm as localtime.
 *
 * The inverse of gmtime(time_t)
 */
time_t mkgmtime(struct tm * t)
{
    int year;
    time_t result;
    static const int cumdays[MONTHSPERYEAR] =
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

    year = 1900 + t->tm_year + t->tm_mon / MONTHSPERYEAR;
    result = (year - 1970) * 365 + cumdays[t->tm_mon % MONTHSPERYEAR];
    result += (year - 1968) / 4;
    result -= (year - 1900) / 100;
    result += (year - 1600) / 400;
    if ((year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0) &&
	(t->tm_mon % MONTHSPERYEAR) < 2)
	result--;
    result += t->tm_mday - 1;
    result *= 24;
    result += t->tm_hour;
    result *= 60;
    result += t->tm_min;
    result *= 60;
    result += t->tm_sec;
    /* this is UTC, no DST
     * if (t->tm_isdst == 1)
     * result -= 3600;
     */
    return (result);
}

timespec_t iso8601_to_timespec(char *isotime)
/* ISO8601 UTC to Unix timespec, no leapsecond correction. */
{
    timespec_t ret;

#ifndef __clang_analyzer__
#ifndef USE_QT
    char *dp = NULL;
    double usec = 0;
    struct tm tm;
    memset(&tm,0,sizeof(tm));

#ifdef HAVE_STRPTIME
    dp = strptime(isotime, "%Y-%m-%dT%H:%M:%S", &tm);
#else
    /* Fallback for systems without strptime (i.e. Windows)
       This is a simplistic conversion for iso8601 strings only,
       rather than embedding a full copy of strptime() that handles all formats */
    double sec;
    unsigned int tmp; // Thus avoiding needing to test for (broken) negative date/time numbers in token reading - only need to check the upper range
    bool failed = false;
    char *isotime_tokenizer = strdup(isotime);
    if (isotime_tokenizer) {
      char *tmpbuf;
      char *pch = strtok_r(isotime_tokenizer, "-T:", &tmpbuf);
      int token_number = 0;
      while (pch != NULL) {
	token_number++;
	// Give up if encountered way too many tokens.
	if (token_number > 10) {
	  failed = true;
	  break;
	}
	switch (token_number) {
	case 1: // Year token
	  tmp = atoi(pch);
	  if (tmp < 9999)
	    tm.tm_year = tmp - 1900; // Adjust to tm year
	  else
	    failed = true;
	  break;
	case 2: // Month token
	  tmp = atoi(pch);
	  if (tmp < 13)
	    tm.tm_mon = tmp - 1; // Month indexing starts from zero
	  else
	    failed = true;
	  break;
	case 3: // Day token
	  tmp = atoi(pch);
	  if (tmp < 32)
	    tm.tm_mday = tmp;
	  else
	    failed = true;
	  break;
	case 4: // Hour token
	  tmp = atoi(pch);
	  if (tmp < 24)
	    tm.tm_hour = tmp;
	  else
	    failed = true;
	  break;
	case 5: // Minute token
	  tmp = atoi(pch);
	  if (tmp < 60)
	    tm.tm_min = tmp;
	  else
	    failed = true;
	  break;
	case 6: // Seconds token
	  sec = safe_atof(pch);
	  // NB To handle timestamps with leap seconds
	  if (0 == isfinite(sec) &&
	      sec >= 0.0 && sec < 61.5 ) {
	    tm.tm_sec = (unsigned int)sec; // Truncate to get integer value
	    usec = sec - (unsigned int)sec; // Get the fractional part (if any)
	  }
	  else
	    failed = true;
	  break;
	default: break;
	}
	pch = strtok_r(NULL, "-T:", &tmpbuf);
      }
      free(isotime_tokenizer);
      // Split may result in more than 6 tokens if the TZ has any t's in it
      // So check that we've seen enough tokens rather than an exact number
      if (token_number < 6)
	failed = true;
    }
    if (failed)
      memset(&tm,0,sizeof(tm));
    else {
      // When successful this normalizes tm so that tm_yday is set
      //  and thus tm is valid for use with other functions
      if (mktime(&tm) == (time_t)-1)
	// Failed mktime - so reset the timestamp
	memset(&tm,0,sizeof(tm));
    }
#endif
    if (dp != NULL && *dp == '.')
	usec = strtod(dp, NULL);
    /*
     * It would be nice if we could say mktime(&tm) - timezone + usec instead,
     * but timezone is not available at all on some BSDs. Besides, when working
     * with historical dates the value of timezone after an ordinary tzset(3)
     * can be wrong; you have to do a redirect through the IANA historical
     * timezone database to get it right.
     */
    ret.tv_sec = mkgmtime(&tm);
    ret.tv_nsec = usec * 1e9;;
#else
    double usec = 0;

    QString t(isotime);
    QDateTime d = QDateTime::fromString(isotime, Qt::ISODate);
    QStringList sl = t.split(".");
    if (sl.size() > 1)
	usec = sl[1].toInt() / pow(10., (double)sl[1].size());
    ret.tv_sec = d.toTime_t();
    ret.tv_nsec = usec * 1e9;;
#endif
#endif /* __clang_analyzer__ */
    return ret;
}

/* Unix timespec UTC time to ISO8601, no timezone adjustment */
/* example: 2007-12-11T23:38:51.033Z */
char *timespec_to_iso8601(timespec_t fixtime, char isotime[], size_t len)
{
    struct tm when;
    char timestr[30];
    long fracsec;

    if (0 > fixtime.tv_sec) {
        // Allow 0 for testing of 1970-01-01T00:00:00.000Z
        return strncpy(isotime, "NaN", len);
    }
    if (999499999 < fixtime.tv_nsec) {
        /* round up */
        fixtime.tv_sec++;
        fixtime.tv_nsec = 0;
    }
	
    (void)gmtime_r(&fixtime.tv_sec, &when);

    /*
     * Do not mess casually with the number of decimal digits in the
     * format!  Most GPSes report over serial links at 0.01s or 0.001s
     * precision.  Round to 0.001s
     */
    fracsec = (fixtime.tv_nsec + 500000) / 1000000;

    (void)strftime(timestr, sizeof(timestr), "%Y-%m-%dT%H:%M:%S", &when);
    (void)snprintf(isotime, len, "%s.%03ldZ",timestr, fracsec);

    return isotime;
}

const char *timespec_str(const struct timespec *ts, char *buf, size_t buf_size)
{
    char sign = ' ';

    if (!TS_GEZ(ts)) {
		sign = '-';
    }

    (void)snprintf(buf, buf_size, "%c%lld.%09ld",
		   sign,
		   (long long)llabs(ts->tv_sec),
		   (long)labs(ts->tv_nsec));
    return  buf;
}

bool matrix_invert(double mat[4][4], double inverse[4][4])
/* selected elements from 4x4 matrox inversion */
{
    // Find all NECESSARY 2x2 subdeterminants
    double Det2_12_01 = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];
    double Det2_12_02 = mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0];
    //double Det2_12_03 = mat[1][0]*mat[2][3] - mat[1][3]*mat[2][0];
    double Det2_12_12 = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
    //double Det2_12_13 = mat[1][1]*mat[2][3] - mat[1][3]*mat[2][1];
    //double Det2_12_23 = mat[1][2]*mat[2][3] - mat[1][3]*mat[2][2];
    double Det2_13_01 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
    //double Det2_13_02 = mat[1][0]*mat[3][2] - mat[1][2]*mat[3][0];
    double Det2_13_03 = mat[1][0] * mat[3][3] - mat[1][3] * mat[3][0];
    //double Det2_13_12 = mat[1][1]*mat[3][2] - mat[1][2]*mat[3][1];
    double Det2_13_13 = mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1];
    //double Det2_13_23 = mat[1][2]*mat[3][3] - mat[1][3]*mat[3][2];
    double Det2_23_01 = mat[2][0] * mat[3][1] - mat[2][1] * mat[3][0];
    double Det2_23_02 = mat[2][0] * mat[3][2] - mat[2][2] * mat[3][0];
    double Det2_23_03 = mat[2][0] * mat[3][3] - mat[2][3] * mat[3][0];
    double Det2_23_12 = mat[2][1] * mat[3][2] - mat[2][2] * mat[3][1];
    double Det2_23_13 = mat[2][1] * mat[3][3] - mat[2][3] * mat[3][1];
    double Det2_23_23 = mat[2][2] * mat[3][3] - mat[2][3] * mat[3][2];

    // Find all NECESSARY 3x3 subdeterminants
    double Det3_012_012 = mat[0][0] * Det2_12_12 - mat[0][1] * Det2_12_02
	+ mat[0][2] * Det2_12_01;
    //double Det3_012_013 = mat[0][0]*Det2_12_13 - mat[0][1]*Det2_12_03
    //                            + mat[0][3]*Det2_12_01;
    //double Det3_012_023 = mat[0][0]*Det2_12_23 - mat[0][2]*Det2_12_03
    //                            + mat[0][3]*Det2_12_02;
    //double Det3_012_123 = mat[0][1]*Det2_12_23 - mat[0][2]*Det2_12_13
    //                            + mat[0][3]*Det2_12_12;
    //double Det3_013_012 = mat[0][0]*Det2_13_12 - mat[0][1]*Det2_13_02
    //                            + mat[0][2]*Det2_13_01;
    double Det3_013_013 = mat[0][0] * Det2_13_13 - mat[0][1] * Det2_13_03
	+ mat[0][3] * Det2_13_01;
    //double Det3_013_023 = mat[0][0]*Det2_13_23 - mat[0][2]*Det2_13_03
    //                            + mat[0][3]*Det2_13_02;
    //double Det3_013_123 = mat[0][1]*Det2_13_23 - mat[0][2]*Det2_13_13
    //                            + mat[0][3]*Det2_13_12;
    //double Det3_023_012 = mat[0][0]*Det2_23_12 - mat[0][1]*Det2_23_02
    //                            + mat[0][2]*Det2_23_01;
    //double Det3_023_013 = mat[0][0]*Det2_23_13 - mat[0][1]*Det2_23_03
    //                            + mat[0][3]*Det2_23_01;
    double Det3_023_023 = mat[0][0] * Det2_23_23 - mat[0][2] * Det2_23_03
	+ mat[0][3] * Det2_23_02;
    //double Det3_023_123 = mat[0][1]*Det2_23_23 - mat[0][2]*Det2_23_13
    //                            + mat[0][3]*Det2_23_12;
    double Det3_123_012 = mat[1][0] * Det2_23_12 - mat[1][1] * Det2_23_02
	+ mat[1][2] * Det2_23_01;
    double Det3_123_013 = mat[1][0] * Det2_23_13 - mat[1][1] * Det2_23_03
	+ mat[1][3] * Det2_23_01;
    double Det3_123_023 = mat[1][0] * Det2_23_23 - mat[1][2] * Det2_23_03
	+ mat[1][3] * Det2_23_02;
    double Det3_123_123 = mat[1][1] * Det2_23_23 - mat[1][2] * Det2_23_13
	+ mat[1][3] * Det2_23_12;

    // Find the 4x4 determinant
    static double det;
    det = mat[0][0] * Det3_123_123
	- mat[0][1] * Det3_123_023
	+ mat[0][2] * Det3_123_013 - mat[0][3] * Det3_123_012;

    // Very small determinants probably reflect floating-point fuzz near zero
    if (fabs(det) < 0.0001)
		return false;

    inverse[0][0] = Det3_123_123 / det;
    //inverse[0][1] = -Det3_023_123 / det;
    //inverse[0][2] =  Det3_013_123 / det;
    //inverse[0][3] = -Det3_012_123 / det;

    //inverse[1][0] = -Det3_123_023 / det;
    inverse[1][1] = Det3_023_023 / det;
    //inverse[1][2] = -Det3_013_023 / det;
    //inverse[1][3] =  Det3_012_023 / det;

    //inverse[2][0] =  Det3_123_013 / det;
    //inverse[2][1] = -Det3_023_013 / det;
    inverse[2][2] = Det3_013_013 / det;
    //inverse[2][3] = -Det3_012_013 / det;

    //inverse[3][0] = -Det3_123_012 / det;
    //inverse[3][1] =  Det3_023_012 / det;
    //inverse[3][2] = -Det3_013_012 / det;
    inverse[3][3] = Det3_012_012 / det;

    return true;
}

const char *gpsd_hexdump(char *scbuf, size_t scbuflen,
					  char *binbuf, size_t binbuflen)
{
    size_t i, j = 0;
    size_t len =
	(size_t) ((binbuflen >
		   MAX_PACKET_LENGTH) ? MAX_PACKET_LENGTH : binbuflen);
    const char *ibuf = (const char *)binbuf;
    const char *hexchar = "0123456789abcdef";

    if (NULL == binbuf || 0 == binbuflen)
		return "";

    for (i = 0; i < len && j < (scbuflen - 3); i++) {
		scbuf[j++] = hexchar[(ibuf[i] & 0xf0) >> 4];
		scbuf[j++] = hexchar[ibuf[i] & 0x0f];
    }
	
    scbuf[j] = '\0';
    return scbuf;
}

const char *gpsd_packetdump(char *scbuf, size_t scbuflen,
			    char *binbuf, size_t binbuflen)
{
    char *cp;
    bool printable = true;

    assert(binbuf != NULL);
    for (cp = binbuf; cp < binbuf + binbuflen; cp++)
	if (!isprint((unsigned char) *cp) && !isspace((unsigned char) *cp)) {
	    printable = false;
	    break;	/* no need to keep iterating */
    }
    
	if (printable)
		return binbuf;
    else
		return gpsd_hexdump(scbuf, scbuflen, binbuf, binbuflen);
}

double safe_atof(const char *string)
{
    static int maxExponent = 511;   /* Largest possible base 10 exponent.  Any
				     * exponent larger than this will already
				     * produce underflow or overflow, so there's
				     * no need to worry about additional digits.
				     */
    static double powersOf10[] = {  /* Table giving binary powers of 10.  Entry */
	10.,			/* is 10^2^i.  Used to convert decimal */
	100.,			/* exponents into floating-point numbers. */
	1.0e4,
	1.0e8,
	1.0e16,
	1.0e32,
	1.0e64,
	1.0e128,
	1.0e256
    };

    bool sign, expSign = false;
    double fraction, dblExp, *d;
    const char *p;
    int c;
    int exp = 0;		/* Exponent read from "EX" field. */
    int fracExp = 0;		/* Exponent that derives from the fractional
				 * part.  Under normal circumstatnces, it is
				 * the negative of the number of digits in F.
				 * However, if I is very long, the last digits
				 * of I get dropped (otherwise a long I with a
				 * large negative exponent could cause an
				 * unnecessary overflow on I alone).  In this
				 * case, fracExp is incremented one for each
				 * dropped digit. */
    int mantSize;		/* Number of digits in mantissa. */
    int decPt;			/* Number of mantissa digits BEFORE decimal
				 * point. */
    const char *pExp;		/* Temporarily holds location of exponent
				 * in string. */

    /*
     * Strip off leading blanks and check for a sign.
     */

    p = string;
    while (isspace((unsigned char) *p)) {
	p += 1;
    }
    if (*p == '\0') {
        return NAN;
    } else if (*p == '-') {
	sign = true;
	p += 1;
    } else {
	if (*p == '+') {
	    p += 1;
	}
	sign = false;
    }

    /*
     * Count the number of digits in the mantissa (including the decimal
     * point), and also locate the decimal point.
     */

    decPt = -1;
    for (mantSize = 0; ; mantSize += 1)
    {
	c = *p;
	if (!isdigit(c)) {
	    if ((c != '.') || (decPt >= 0)) {
		break;
	    }
	    decPt = mantSize;
	}
	p += 1;
    }

    /*
     * Now suck up the digits in the mantissa.  Use two integers to
     * collect 9 digits each (this is faster than using floating-point).
     * If the mantissa has more than 18 digits, ignore the extras, since
     * they can't affect the value anyway.
     */

    pExp  = p;
    p -= mantSize;
    if (decPt < 0) {
	decPt = mantSize;
    } else {
	mantSize -= 1;			/* One of the digits was the point. */
    }
    if (mantSize > 18) {
	fracExp = decPt - 18;
	mantSize = 18;
    } else {
	fracExp = decPt - mantSize;
    }
    if (mantSize == 0) {
	fraction = 0.0;
	//p = string;
	goto done;
    } else {
	int frac1, frac2;
	frac1 = 0;
	for ( ; mantSize > 9; mantSize -= 1)
	{
	    c = *p;
	    p += 1;
	    if (c == '.') {
		c = *p;
		p += 1;
	    }
	    frac1 = 10*frac1 + (c - '0');
	}
	frac2 = 0;
	for (; mantSize > 0; mantSize -= 1)
	{
	    c = *p;
	    p += 1;
	    if (c == '.') {
		c = *p;
		p += 1;
	    }
	    frac2 = 10*frac2 + (c - '0');
	}
	fraction = (1.0e9 * frac1) + frac2;
    }

    /*
     * Skim off the exponent.
     */

    p = pExp;
    if ((*p == 'E') || (*p == 'e')) {
	p += 1;
	if (*p == '-') {
	    expSign = true;
	    p += 1;
	} else {
	    if (*p == '+') {
		p += 1;
	    }
	    expSign = false;
	}
	while (isdigit((unsigned char) *p)) {
	    exp = exp * 10 + (*p - '0');
	    p += 1;
	}
    }
    if (expSign) {
	exp = fracExp - exp;
    } else {
	exp = fracExp + exp;
    }

    /*
     * Generate a floating-point number that represents the exponent.
     * Do this by processing the exponent one bit at a time to combine
     * many powers of 2 of 10. Then combine the exponent with the
     * fraction.
     */

    if (exp < 0) {
	expSign = true;
	exp = -exp;
    } else {
	expSign = false;
    }
    if (exp > maxExponent) {
	exp = maxExponent;
	errno = ERANGE;
    }
    dblExp = 1.0;
    for (d = powersOf10; exp != 0; exp >>= 1, d += 1) {
	if (exp & 01) {
	    dblExp *= *d;
	}
    }
    if (expSign) {
	fraction /= dblExp;
    } else {
	fraction *= dblExp;
    }

done:
    if (sign) {
	return -fraction;
    }
    return fraction;
}

int do_lat_lon(char *field[], struct gps_fix_t *out)
{
    double d, m;
    double lon;
    double lat;

    if ('\0' == field[0][0] ||
        '\0' == field[1][0] ||
        '\0' == field[2][0] ||
        '\0' == field[3][0]) {
        return 1;
    }

    lat = safe_atof(field[0]);
    m = 100.0 * modf(lat / 100.0, &d);
    lat = d + m / 60.0;
    if ('S' == field[1][0])
        lat = -lat;

    lon = safe_atof(field[2]);
    m = 100.0 * modf(lon / 100.0, &d);
    lon = d + m / 60.0;
    if ('W' == field[3][0])
        lon = -lon;

    if (0 == isfinite(lat) ||
        0 == isfinite(lon)) {
        return 2;
    }

    out->latitude = lat;
    out->longitude = lon;
    return 0;
}

void gpsd_century_update(struct gps_device_t *session, int century)
{
    session->context->valid |= CENTURY_VALID;
    if (century > session->context->century) {
		/*
		* This mismatch is almost certainly not due to a GPS week
		* rollover, because that would throw the ZDA report backward
		* into the last rollover period instead of forward.  Almost
		* certainly it means that a century mark has passed while
		* gpsd was running, and we should trust the new ZDA year.
		*/
		dprintf("century rollover detected.\n");
		session->context->century = century;
    } else if (session->context->start_time >= GPS_EPOCH && century < session->context->century) {
		/*
		* This looks like a GPS week-counter rollover.
		*/
		dprintf("ZDA year less than clock year, "
			"probable GPS week rollover lossage\n");
		session->context->valid &=~ CENTURY_VALID;
    }
}

size_t strlcpy(char *dst, const char *src, size_t siz)
{
    size_t len = strlen(src);
    if (siz != 0) {
	if (len >= siz) {
	    memcpy(dst, src, siz - 1);
	    dst[siz - 1] = '\0';
	} else
	    memcpy(dst, src, len + 1);
    }
    return len;
}

void TS_NORM( struct timespec *ts)
{
    if ( (  1 <= ts->tv_sec ) ||
         ( (0 == ts->tv_sec ) && (0 <= ts->tv_nsec ) ) ) {
        /* result is positive */
	if ( NS_IN_SEC <= ts->tv_nsec ) {
            /* borrow from tv_sec */
	    ts->tv_nsec -= NS_IN_SEC;
	    ts->tv_sec++;
	} else if ( 0 > (ts)->tv_nsec ) {
            /* carry to tv_sec */
	    ts->tv_nsec += NS_IN_SEC;
	    ts->tv_sec--;
	}
    }  else {
        /* result is negative */
	if ( -NS_IN_SEC >= ts->tv_nsec ) {
            /* carry to tv_sec */
	    ts->tv_nsec += NS_IN_SEC;
	    ts->tv_sec--;
	} else if ( 0 < ts->tv_nsec ) {
            /* borrow from tv_sec */
	    ts->tv_nsec -= NS_IN_SEC;
	    ts->tv_sec++;
	}
    }
}

int clock_gettime(clockid_t clk_id, struct timespec *ts)
{
    (void) clk_id;
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0)
	return -1;
    ts->tv_sec = tv.tv_sec;
    ts->tv_nsec = tv.tv_usec * 1000;
    return 0;
}

#define GEOID_ROW       37
#define GEOID_COL       73

/* This table is EGM2008.  Values obtained from GeoidEval, part of
 * geographiclib., by using devtools/get_geoid_table.py
 *
 * geoid_delta[][] has the geoid separation, in cm, on a 5 degree by 5
 * degree grid for the entire planet.
 */
/* *INDENT-OFF* */
const short geoid_delta[GEOID_ROW][GEOID_COL] = {
    /* -90 */
    { -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015,
      -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015,
      -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015,
      -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015,
      -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015,
      -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015,
      -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015, -3015,
      -3015, -3015, -3015},
    /* -85 */
    { -3568, -3608, -3739, -3904, -4039, -4079, -4033, -3946, -3845, -3734,
      -3603, -3458, -3310, -3163, -2994, -2827, -2695, -2667, -2737, -2823,
      -2840, -2757, -2634, -2567, -2547, -2540, -2452, -2247, -1969, -1704,
      -1540, -1507, -1552, -1592, -1573, -1513, -1465, -1478, -1542, -1577,
      -1483, -1256, -1029,  -957, -1066, -1216, -1262, -1194, -1118, -1129,
      -1231, -1370, -1504, -1641, -1813, -2028, -2255, -2455, -2630, -2811,
      -3022, -3242, -3436, -3578, -3658, -3676, -3640, -3578, -3527, -3490,
      -3532, -3570, -3568},
    /* -80 */
    { -5232, -5276, -5275, -5301, -5286, -5276, -5218, -5001, -4775, -4580,
      -4319, -4064, -3854, -3691, -3523, -3205, -2910, -2608, -2337, -2355,
      -2417, -2445, -2471, -2350, -2230, -2136, -1869, -1689, -1732, -1748,
      -1540, -1236, -1048,  -794,  -569,  -603,  -501,  -305,  -166,    19,
        146,   274,   444,   510,   534,   550,   458,   373,   473,   575,
        607,   732,   562,   153,  -271,  -825, -1300, -1861, -2475, -2866,
      -3434, -4001, -4196, -4533, -4989, -5152, -5094, -4983, -4987, -5065,
      -5055, -5115, -5232},
    /* -75 */
    { -6155, -6339, -6266, -6344, -6282, -6100, -6009, -5492, -5088, -4547,
      -4187, -3901, -3586, -3234, -3051, -2886, -2577, -2289, -1981, -1655,
      -1435, -1096,  -557,  -617,  -998,  -961,  -655,  -464,  -170,    79,
       -103,   -64,   150,   223,   819,  1006,  1174,  1136,  1211,  1278,
       1467,  1686,  1783,  1706,  1833,  1721,  1653,  1580,  1267,   953,
        629,   807,   774,   607,   217,  -386,  -814, -1354, -2452, -3542,
      -3833, -3932, -4259, -4962, -4977, -5536, -5753, -5800, -6012, -5835,
      -5751, -5820, -6155},
    /* -70 */
    { -6218, -6432, -6333, -6150, -6021, -5948, -5705, -5480, -5213, -4789,
      -4365, -4003, -3757, -3514, -3250, -3000, -2672, -2541, -2138, -1220,
       -844,  -277,   249,   906,   458,    69,    26,    98,   166,   130,
        118,   253,   303,   437,  1010,  1341,  1423,  1558,  1682,  1825,
       1766,  1917,  2027,  2047,  2164,  2909,  2882,  2997,  3010,  2687,
       1749,  1703,  1799,  1438,  1099,   346,  -813, -1432, -2149, -2320,
      -2704, -3085, -3907, -4172, -4287, -4846, -5466, -5592, -5576, -5525,
      -5800, -5954, -6218},
    /* -65 */
    { -5152, -5115, -5049, -4943, -4858, -4714, -4580, -4369, -4202, -4060,
      -3806, -3454, -3210, -3007, -2749, -2484, -2264, -1928, -1501, -1113,
       -614,    31,   642,  1502,  1833,  1844,  1268,  1442,  1441,  1302,
       1164,  1041,   945,   874,   896,  1059,  1368,  1680,  1736,  1975,
       1891,  1979,  2131,  2338,  2672,  2861,  3114,  3097,  2801,  2695,
       2422,  2022,  1648,  1340,   713,   352,  -127,  -895, -1740, -2040,
      -2854, -3292, -3453, -3922, -4395, -4538, -4554, -4356, -4445, -4669,
      -4988, -5122, -5152},
    /* -60 */
    { -4598, -4449, -4278, -4056, -3732, -3417, -3205, -3094, -3008, -2876,
      -2669, -2478, -2350, -2272, -2218, -1969, -1660, -1381, -1123,  -716,
       -350,   247,   924,  1712,  2016,  2066,  2032,  1556,  2123,  2322,
       2384,  1034,  2121,  1923,  1720,  1571,  1517,  1668,  2008,  2366,
       2546,  2736,  2914,  3169,  3395,  3467,  3315,  3286,  3279,  3073,
       2930,  2727,  2502,  1783,   893,   311,  -328,  -778, -1364, -1973,
      -2467, -2833, -3143, -3283, -3311, -3120, -2956, -3027, -3485, -3972,
      -4454, -4679, -4598},
    /* -55 */
    { -3414, -3429, -3223, -3013, -2704, -2474, -2292, -2185, -1962, -1818,
      -1828, -1485, -1259, -1284, -1327, -1304, -1097, -1071,  -628,  -326,
        174,   340,  1331,  1217,  1712,  1441,  1467,  1578,  1654,  2179,
        764,  1486,  2074,  2245,  2462,  2655,  2720,  2581,  2423,  2731,
       3145,  3383,  3436,  3909,  4448,  4422,  4032,  3938,  3665,  3461,
       3465,  3317,  2487,  1908,  1311,   683,    52,  -582, -1196, -1798,
      -2158, -2450, -2475, -2429, -2277, -2011, -2140, -2306, -2551, -2726,
      -3016, -3319, -3414},
    /* -50 */
    { -1615, -1938, -1875, -1827, -1839, -1793, -1605, -1650, -1737, -1773,
      -1580, -1237, -1010,  -983, -1051, -1025,  -838,  -653,  -316,    48,
        502,  1382,  1186,  1114,  1264,   785,   231,   329,   353,   556,
       1084,  1597,  2065,  2475,  2744,  2701,  2518,  2545,  2584,  2963,
       3323,  3537,  3792,  4085,  4520,  4505,  4459,  4287,  3818,  4112,
       3975,  3293,  2748,  2043,  1272,   569,  -207,  -898, -1498, -1990,
      -2242, -2358, -2212, -1968, -1843, -1695, -1705, -1688, -1400, -1177,
      -1013, -1168, -1615},
    /* -45 */
    {   338,   -20,  -606,  -849,  -777,  -838, -1123, -1322, -1485, -1503,
      -1413, -1203, -1077, -1004,  -960,  -829,  -662,  -371,   -88,   322,
        710,  1323,  1831,  1202,   908,    47,  -292,  -367,  -495,  -174,
        688,  1500,  2194,  2673,  2568,  2423,  2099,  2168,  2617,  2834,
       3254,  3328,  3443,  4442,  4639,  4588,  4524,  4223,  3575,  3187,
       3101,  2651,  2155,  1506,   774,   -55,  -961, -1719, -2355, -2719,
      -2731, -2670, -2430, -2026, -1715, -1477, -1144,  -901,  -646,  -303,
        871,   565,   338},
    /* -40 */
    {  2048,  1283,   637,   317,   109,  -156,  -679, -1023, -1186, -1277,
      -1275, -1202, -1282, -1150, -1022,  -881,  -690,  -300,   -84,   130,
        694,   937,  2220,  1511,  1341,   558,  -266,  -623,  -670,  -209,
        643,  1459,  2101,  2385,  2307,  2000,  1765,  1992,  2496,  2733,
       2941,  3431,  3298,  3327,  3877,  4306,  4069,  3446,  2844,  2601,
       2333,  1786,  1318,   599,  -238, -1184, -2098, -2786, -3250, -3406,
      -3351, -3095, -2741, -2101, -1482,  -148,  -201,   221,   491,  1179,
       1877,  1206,  2048},
    /* -35 */
    {  2833,  2556,  1700,  1059,   497,   -21,  -370,  -752,  -959, -1103,
      -1093, -1104, -1198, -1097,  -960,  -785,  -596,  -362,  -211,   103,
        739,  1300,  3029,  2021,  1712,  1269,   -23,  -616,  -701,  -255,
        684,  1237,  1701,  1903,  1696,  1789,  1795,  2034,  2398,  2561,
       3187,  2625,  2609,  2897,  2564,  3339,  3118,  3121,  2240,  2102,
       1529,   991,   387,  -559, -1464, -2380, -3138, -3999, -3899, -3446,
      -3473, -3300, -2823, -1043,   143,   970,  2058,  1555,  1940,  2621,
       3154,  3839,  2833},
    /* -30 */
    {  4772,  3089,  2257,  1381,   566,    64,  -136,  -612,  -868, -1186,
      -1309, -1131, -1033,  -903,  -780,  -625,  -443,  -242,   100,   269,
        815,  1489,  3633,  2424,  1810,  1138,   297,  -720,  -847,    -2,
        347,   579,  1025,  1408,  1504,  1686,  2165,  2353,  2599,  3182,
       3332,  3254,  3094,  2042,  1369,  1945,  1468,  1487,  1505,  1048,
        613,    26,  -904, -1757, -2512, -3190, -3751, -3941, -3939, -2896,
      -2222, -1766, -1442,    70,  1262,  2229,  3189,  2910,  3371,  3608,
       4379,  4520,  4772},
    /* -25 */
    {  4984,  2801,  2475,  1374,   798,   198,  -269,  -628, -1063, -1262,
      -1090,  -970,  -692,  -516,  -458,  -313,  -143,    19,   183,   403,
        837,  1650,  3640,  2990,  2084,   628,   422,  -597, -1130,  -712,
       -474,  -110,   446,  1043,  1349,  1571,  2008,  2572,  2405,  3175,
       2766,  2407,  2100,  1130,   367,   840,    89,   114,    49,   -25,
       -494, -1369, -2345, -3166, -3804, -4256, -4141, -3730, -3337, -1814,
       -901,  -388,   298,  1365,  2593,  3490,  4639,  4427,  4795,  4771,
       5325,  5202,  4984},
    /* -20 */
    {  4994,  5152,  2649,  1466,   935,   427,  -115,  -518,  -838, -1135,
      -1134,  -917,  -525,  -280,  -218,  -310,  -396,  -306,  -137,   148,
        811,  1643,  3496,  4189,  1958,   358,  -784,  -684,  -740,  -800,
       -579,  -638,   -49,   704,  1221,  1358,  1657,  1957,  2280,  2639,
       2157,  1246,   728,  -364, -1021,  -586, -1098, -1055, -1032, -1244,
      -2065, -3158, -4028, -4660, -4802, -4817, -4599, -3523, -2561, -1260,
        446,  1374,  2424,  3310,  4588,  5499,  5724,  5479,  5698,  5912,
       6400,  6116,  4994},
    /* -15 */
    {  4930,  4158,  2626,  1375,   902,   630,   150,  -275,  -667, -1005,
       -954,  -847,  -645,  -376,  -315,  -479,  -639,  -681,  -550,  -268,
        709,  2996,  4880,  2382,  1695,  -136,  -964, -1211, -1038, -1045,
       -695,  -595,    23,   733,  1107,  1318,  1348,  1376,  1630,  2240,
       1248,   454,  -737, -1252, -2001, -2513, -1416, -2169, -2269, -3089,
      -4063, -5194, -5715, -6105, -5700, -4873, -3919, -2834, -1393,  -112,
       1573,  3189,  3907,  4863,  5437,  6548,  6379,  6281,  6289,  5936,
       6501,  5794,  4930},
    /* -10 */
    {  3525,  2747,  2135,  1489,  1078,   739,   544,   -39,  -268,  -588,
       -917, -1025, -1087,  -940,  -771,  -923, -1177, -1114,  -919,  -383,
       -108,  2135,  2818,  1929,   386, -1097, -1911, -1619, -1226, -1164,
       -952,  -583,   399,  1070,  1280,  1345,  1117,   993,  1306,  1734,
        538,  -463, -1208, -1602, -2662, -3265, -3203, -3408, -3733, -5014,
      -6083, -7253, -7578, -7096, -6418, -4658, -2647,  -586,   -87,  1053,
       3840,  3336,  5240,  6253,  6898,  7070,  7727,  7146,  6209,  5826,
       5068,  4161,  3525},
    /* -5 */
    {  2454,  1869,  1656,  1759,  1404,  1263,  1012,   605,   108,  -511,
       -980, -1364, -1620, -1633, -1421, -1342, -1412, -1349, -1006,  -229,
       1711,  1293,  1960,   605,  -793, -2058, -2108, -2626, -1195,  -606,
       -513,  -108,   671,  1504,  1853,  1711,  1709,   940,   570,   296,
       -913, -1639, -1471, -1900, -3000, -4164, -4281, -4062, -5366, -6643,
      -7818, -8993, -9275, -8306, -6421, -4134, -1837,  1367,  2850,  4286,
       5551,  5599,  5402,  6773,  7736,  7024,  8161,  6307,  5946,  4747,
       3959,  3130,  2454},
    /* 0 */
    {  2128,  1774,  1532,  1470,  1613,  1589,  1291,   783,    79,  -676,
      -1296, -1941, -2298, -2326, -2026, -1738, -1412, -1052,  -406,    82,
       1463,  1899,  1352,  -170, -1336, -2446, -2593, -2328, -1863,  -833,
        245,  1005,  1355,  1896,  1913,  1888,  1723,  1642,   940,  -127,
      -1668, -1919, -1078, -1633, -2762, -4357, -4885, -5143, -6260, -7507,
      -8947, -10042, -10259, -8865, -6329, -3424,  -692,  1445,  3354,  5132,
       5983,  4978,  7602,  7274,  7231,  6941,  6240,  5903,  4944,  4065,
       3205,  2566,  2128},
    /* 5 */
    {  1632,  1459,  1243,  1450,  1643,  1432,   867,   283,  -420, -1316,
      -1993, -2614, -3012, -3016, -2555, -1933, -1256,  -688,  -133,   634,
       1369,  2095,   -92,  -858, -1946, -3392, -3666, -3110, -1839,  -371,
        674,  1221,  1657,  1994,  2689,  2577,  2020,  2126,  1997,   987,
       -739,  -989, -1107, -1369, -1914, -3312, -4871, -5365, -6171, -7732,
      -9393, -10088, -10568, -9022, -6053, -4104, -1296,   373,  2310,  4378,
       6279,  6294,  6999,  6852,  6573,  6302,  5473,  5208,  4502,  3445,
       2790,  2215,  1632},
    /* 10 */
    {  1285,  1050,  1212,  1439,  1055,   638,   140,  -351, -1115, -2060,
      -2904, -3593, -3930, -3694, -2924, -2006, -1145,  -441,   164,  1059,
         91,  -440, -1043, -2791, -4146, -4489, -4259, -3218, -1691,  -683,
        306,  1160,  1735,  3081,  3275,  2807,  2373,  2309,  2151,  1245,
        207,  -132,  -507,  -564,  -956, -1917, -3167, -5067, -5820, -7588,
      -9107, -9732, -9732, -8769, -6308, -4585, -2512,  -891,  1108,  3278,
       5183,  6391,  5985,  5969,  6049,  5616,  4527,  4156,  3531,  2776,
       2456,  1904,  1285},
    /* 15 */
    {   862,   804,   860,   969,   544,    89,  -417, -1008, -1641, -2608,
      -3607, -4234, -4482, -4100, -3232, -2092, -1105, -1092,   238,   330,
       -571, -1803, -2983, -3965, -5578, -4864, -3777, -2572, -1690,  -536,
        806,  2042,  2323,  3106,  3019,  2833,  2260,  2064,  2036,  1358,
       1030,   908,   391,   -54,  -377,  -885, -2172, -3359, -5309, -6686,
      -8058, -8338, -8695, -8322, -6404, -5003, -3420, -2060,  -255,  1833,
       4143,  4218,  4771,  5031,  5241,  5504,  4399,  3471,  2832,  2266,
       1643,  1190,   862},
    /* 20 */
    {   442,   488,   986,   877,   757,  1175,  -696, -1473, -2285, -3128,
      -3936, -4520, -4739, -4286, -3350, -2092,  -747, -1894, -1083, -1508,
      -2037, -2528, -4813, -6316, -4698, -4222, -3279, -1814, -1001,   212,
       1714,  2273,  2535,  3367,  3112,  2736,  3086,  2742,  2679,  2071,
       1422,  1333,   922,   619,   183,  -945, -3070, -3680, -4245, -5461,
      -6064, -6652, -6806, -6210, -5947, -5177, -3814, -2589, -1319,   551,
       2150,  3262,  3799,  4177,  4898,  4658,  4149,  2833,  2148,  1410,
        899,   551,   442},
    /* 25 */
    {  -248,    12,   716,   415,   327,  -187, -1103, -1729, -2469, -3296,
      -4040, -4545, -4642, -4232, -3466, -2064, -1667, -3232, -2660, -2685,
      -2789, -4262, -5208, -5084, -4935, -4077, -2622,  -804,   131,   946,
       1859,  2203,  3038,  3433,  3758,  3029,  2757,  3524,  3109,  2511,
       2300,  1554,  1316,  1114,   954,   -81, -2642, -3389, -3167, -4211,
      -4634, -5193, -6014, -6245, -5347, -5313, -3846, -3149, -2130,  -354,
       1573,  2760,  3310,  3713,  4594,  3862,  2827,  1939,  1019,   313,
       -142,  -378,  -248},
    /* 30 */
    {  -720,  -717,  -528,  -573,  -867, -1224, -1588, -2135, -2796, -3432,
      -4036, -4329, -4246, -3464, -2996, -2389, -2323, -2844, -2744, -2884,
      -3238, -4585, -5164, -4463, -4064, -3238, -1751,   150,  1657,  2501,
       3023,  3007,  3404,  3976,  4354,  4648,  3440,  2708,  2813,  2968,
       2611,  2104,  1606,  1808,  1086,  -392, -1793,  -689, -1527, -2765,
      -3766, -4709, -3687, -2800, -3375, -3793, -3365, -4182, -2385, -1115,
        785,  2302,  3020,  3564,  4178,  2993,  1940,  1081,   331,  -364,
       -683,  -690,  -720},
    /* 35 */
    { -1004, -1222, -1315, -1304, -1463, -1680, -2160, -2675, -3233, -3746,
      -4021, -4053, -3373, -3012, -2447, -2184, -2780, -3219, -2825, -3079,
      -3181, -4284, -4548, -3867, -3123, -2302,  -785,   943,  2687,  4048,
       4460,  4290,  4118,  4585,  4282,  4437,  4898,  3818,  3696,  3414,
       2299,  2057,   627,  1915,  1833,   451,   678,  -876, -1602, -2167,
      -3344, -2549, -2860, -3514, -4043, -4207, -4005, -3918, -3121, -1521,
        471,  2023,  2980,  3679,  3465,  2405,  1475,   553,  -142,  -880,
      -1178,  -963, -1004},
    /* 40 */
    { -1223, -1218, -1076, -1116, -1298, -1541, -2085, -2648, -3120, -3473,
      -3679, -3342, -2334, -1912, -1787, -1756, -2482, -3182, -3322, -3429,
      -3395, -3374, -3372, -3341, -2654, -1509,   105,  1620,  3250,  4603,
       5889,  5776,  5198,  4840,  4903,  5370,  5086,  4536,  4519,  4601,
       3395,  4032,  3890,  3537,  3113,  2183, -1769, -1552, -2856, -3694,
      -4092, -3614, -5468, -6518, -6597, -5911, -5476, -4465, -2802, -1076,
        232,  1769,  2305,  3018,  3768,  1721,  1694,   667,  -154,  -799,
      -1068, -1196, -1223},
    /* 45 */
    {  -634,  -460,  -330,  -267,  -413,  -818, -1310, -1763, -2352, -2738,
      -2632, -2685, -1929, -1340,  -737, -1441, -2254, -2685, -3358, -3488,
      -3635, -3187, -2665, -2142, -1515,  -124,  1727,  2798,  3965,  5065,
       6150,  6513,  6089,  5773,  5044,  4471,  4677,  5052,  3938,  4537,
       4425,  3652,  3063,  2178,  1267,    84, -1109, -1974, -2905, -3650,
      -4264, -4741, -4136, -6324, -5826, -5143, -4851, -4344, -3225, -1386,
          5,  1153,  2198,  2833,  2835,  2563,  1337,  1194,   503,  -329,
       -289,  -754,  -634},
    /* 50 */
    {  -578,   -40,   559,   880,   749,   464,     0,  -516, -1140, -1655,
      -1818, -1589, -1555, -1337, -1769, -1919, -2372, -2981, -3485, -3976,
      -3941, -3565, -2614, -2223, -1253,   802,  2406,  3239,  4434,  5428,
       6265,  6394,  6180,  5690,  5855,  5347,  4506,  4685,  4799,  4445,
       3972,  3165,  2745,  1601,  1084,    41, -1170, -1701, -1916, -2914,
      -3305, -3790, -4435, -4128, -4163, -4535, -4190, -3891, -2951, -1869,
       -414,   851,  1494,  2097,  2268,  1939,  2031,  2460,   638,   578,
        325,    98,  -578},
    /* 55 */
    {   -18,   482,   905,  1562,  1739,   983,  1097,   568,    34,  -713,
       -695, -1072, -1576, -1879, -2479, -2884, -3275, -3971, -4456, -4654,
      -4461, -3688, -2697, -1623,  -823,  1270,  2523,  3883,  4967,  5977,
       6049,  6149,  6095,  5776,  5820,  5575,  4642,  4099,  4025,  3462,
       2679,  2447,  1951,  1601,  1151,   663,   157,  -603,  -952, -1987,
      -2609, -3316, -3600, -3684, -3717, -3836, -4024, -3452, -2950, -1861,
       -903,    89,   975,  1499,  1560,  1601,  1922,  2031,  2326,   -58,
        506,  -177,   -18},
    /* 60 */
    {    93,   673,   969,  1168,  1498,  1486,  1439,  1165,  1128,   720,
          5,  -689, -1610, -2409, -3094, -3585, -4193, -4772, -4678, -4521,
      -4184, -2955, -2252,  -834,   503,  1676,  2882,  4130,  4892,  5611,
       6390,  6338,  6069,  5974,  5582,  5461,  4788,  4503,  4080,  2957,
       1893,  1773,  1586,  1544,  1136,  1026,   622,    50,  -389, -1484,
      -2123, -2625, -3028, -3143, -3366, -3288, -3396, -3069, -2770, -2605,
      -1663,  -555,    25,   491,  1168,  1395,  1641,  1597,  1426,  1299,
        921,  -160,    93},
    /* 65 */
    {   419,   424,   443,   723,   884,  1030,  1077,  1191,  1065,   734,
        265, -1052, -1591, -2136, -2773, -3435, -3988, -3978, -3698, -3509,
      -3370, -2490, -1347,  -263,  1647,  2582,  3291,  4802,  4447,  5609,
       5879,  6454,  6709,  6606,  5988,  5365,  5103,  4385,  3996,  3250,
       2526,  1766,  1817,  1751,  1275,   857,   636,    29,   -12,  -918,
      -1364, -1871, -2023, -2102, -2258, -2441, -2371, -2192, -1908, -1799,
      -1720, -1662,  -385,    86,   466,   880,   715,   834,  1010,  1105,
        877,   616,   419},
    /* 70 */
    {   242,    93,    98,    62,   -54,   -25,  -127,  -156,  -253,  -412,
       -805, -1106, -1506, -1773, -2464, -2829, -2740, -2579, -2559, -2271,
      -1849,  -853,   294,  1055,  2357,  2780,  2907,  3909,  4522,  5272,
       5594,  5903,  5966,  5930,  5592,  5188,  4878,  4561,  4190,  3834,
       2963,  2451,  1981,  1525,  1064,   694,   253,   -70,  -318,  -781,
       -979, -1048, -1274, -1413, -1175, -1313, -1449, -1206,  -850, -1087,
       -828,  -933,  -540,  -301,   -35,    53,   279,   267,   345,   371,
        334,   289,   242},
    /* 75 */
    {   128,   228,   376,    46,  -173,  -355,  -417,  -548,  -764,  -925,
       -419,  -950, -1185, -1102, -1293, -1355, -1075,  -713,  -365,   167,
        516,  1381,  1882,  1826,  1956,  2492,  3192,  3541,  3750,  4123,
       4462,  4592,  4472,  4705,  4613,  4559,  4340,  4392,  4144,  3973,
       3119,  2582,  2057,  1684,  1199,   834,   477,   325,   295,  -198,
       -459,  -670,  -706,  -677,  -766,  -852,  -939,  -905,  -637,  -601,
       -531,  -433,  -292,  -158,    88,    85,   118,   121,   147,   179,
        173,   149,   128},
    /* 80 */
    {   342,   293,   244,   159,    38,    20,    15,   -15,  -109,  -119,
       -240,  -182,    16,   397,   550,   264,   350,   670,   865,   681,
       1188,  1136,   703,  1153,  1930,  2412,  2776,  3118,  3351,  3634,
       3653,  3272,  3177,  3161,  3354,  3671,  3615,  3572,  3522,  3274,
       2914,  2682,  2426,  2185,  1845,  1584,  1297,  1005,   809,   507,
        248,   314,   230,    96,   149,   240,   274,   297,   153,   109,
        164,    91,   104,    43,    12,   153,   243,   170,   184,    59,
         99,   158,   342},
    /* 85 */
    {   912,   961,  1013,  1013,   997,  1032,  1026,  1050,  1072,  1132,
       1156,  1253,  1310,  1389,  1441,  1493,  1508,  1565,  1621,  1642,
       1768,  1888,  2036,  2089,  2117,  2106,  2010,  2120,  2276,  2376,
       2426,  2427,  2526,  2582,  2493,  2534,  2628,  2564,  2471,  2509,
       2407,  2332,  2214,  2122,  1987,  1855,  1714,  1619,  1517,  1474,
       1406,  1351,  1308,  1264,  1181,  1081,  1047,  1084,  1043,   964,
        851,   755,   732,   706,   697,   785,   864,   762,   686,   729,
        789,   856,   912},
    /* 90 */
    {  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,
       1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,
       1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,
       1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,
       1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,
       1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,
       1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,  1490,
       1490,  1490,  1490}
};

static double bilinear(int x1, int y1, int x2, int y2, double x,
                       double y, int z11, int z12, int z21,
                       int z22)
{
    int delta;
    double xx1 = x - x1;
    double x2x = x2 - x;
    double yy1 = y - y1;
    double y2y = y2 - y;

    /* handle some corner cases */
    if (y1 == y2) {
       if (x1 == x2)
           return z11;
       else
           return (z22 * (xx1) + z11 * (x2x)) / (x2 - x1);
    } else if (x1 == x2) {
       return (z22 * (yy1) + z11 * (y2y)) / (y2 - y1);
    }

    delta = (y2 - y1) * (x2 - x1);

    return (z22 * yy1 * xx1 + z12 * y2y * xx1 +
            z21 * yy1 * x2x + z11 * y2y * x2x) / delta;
}

double wgs84_separation(double lat, double lon)
{
    int ilat, ilon;
    int ilat1, ilat2, ilon1, ilon2;
    const int span = 5;

    if (0 == isfinite(lat) ||
        0 == isfinite(lon)) {
        return 0.0;
    }

    /* ilat is 0 to 18
     * lat -90 (90S) is ilat 0
     * lat 0 is ilat 9
     * lat 90 (90N) is ilat 18 */
    ilat = (int)floor((90. + lat) / span);
    /* ilon is 0 to 36
     * lon -180 is ilon 0
     * long 0 (Prime Median) is ilon 18
     * long 180 is ilon 36 */
    ilon = (int)floor((180. + lon) / span);

    /* sanity checks to prevent segfault on bad data */
    if ((GEOID_ROW <= ilat) || (0 > ilat) ||
        (GEOID_COL <= ilon) || (0 > ilon))
        return 0.0;

    ilat1 = ilat;
    ilon1 = ilon;
    ilat2 = (ilat < GEOID_ROW - 1) ? ilat + 1 : ilat;
    ilon2 = (ilon < GEOID_COL - 1) ? ilon + 1 : ilon;

    /* the "/ 100" is to convert cm to meters */
    return bilinear(ilon1 * span - 180, ilat1 * span - 90,
                    ilon2 * span - 180, ilat2 * span - 90,
                    lon, lat,
                    geoid_delta[ilat1][ilon1],
                    geoid_delta[ilat1][ilon2],
                    geoid_delta[ilat2][ilon1],
                    geoid_delta[ilat2][ilon2]
        ) / 100;
}
