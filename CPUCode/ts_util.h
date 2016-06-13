/* $Id: ts_util.h,v 1.11 2009/09/09 22:38:13 alex Exp $ */
/*******************************************************************************

    ts_util.h

    "timespec" Manipulation Definitions.

*******************************************************************************/

// http://www.geonius.com/software/libgpl/ts_util.html

#ifndef  TS_UTIL_H		/* Has the file been INCLUDE'd already? */
#define  TS_UTIL_H

/*******************************************************************************
    Public functions.
*******************************************************************************/

struct  timespec tsAdd(struct timespec time1, struct timespec time2);

int  tsCompare(struct timespec time1, struct timespec time2) ;

struct  timespec  tsCreate(long seconds, long nanoseconds);

double  tsFloat(struct timespec time);

const  char  *tsShow(struct timespec binaryTime,bool inLocal,const char *format);

struct  timespec  tsSubtract(struct timespec time1,struct timespec time2);

#endif				/* If this file was not INCLUDE'd previously. */
