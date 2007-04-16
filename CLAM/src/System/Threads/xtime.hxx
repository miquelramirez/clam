/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __XTIME__
#define __XTIME__

#ifdef WIN32
#include "CLAM_windows.h" // for FTIME
#undef GetClassName
#else
#include <sys/time.h> // for GetTimeOfDay
#endif
#include <pthread.h>

namespace CLAM
{

	enum 
	{
		TIME_UTC=1,
		TIME_TAI,
		TIME_MONOTONIC,
		TIME_PROCESS,
		TIME_THREAD,
		TIME_LOCAL,
		TIME_SYNC,
		TIME_RESOLUTION
	};

	struct xtime
	{
		unsigned int sec;
		unsigned int nsec;
	};

	inline int xtime_get( xtime* xtp, int clock_type )
	{
		if ( clock_type == TIME_UTC )
			{
#ifdef WIN32
				FILETIME ft;
				GetSystemTimeAsFileTime(&ft);
				const unsigned __int64 TIMESPEC_TO_FILETIME_OFFSET = ((unsigned __int64)27111902UL << 32) + (unsigned __int64)3577643008UL;
				xtp->sec = (int)((*( __int64*)&ft - TIMESPEC_TO_FILETIME_OFFSET) / 10000000);
				xtp->nsec = (int)((*( __int64*)&ft - TIMESPEC_TO_FILETIME_OFFSET -
								   (( __int64)xtp->sec * ( __int64)10000000)) * 100);
				return clock_type;
#else
				struct timeval tv;
				gettimeofday(&tv, 0);
				xtp->sec = tv.tv_sec;
				xtp->nsec = tv.tv_usec * 1000;
				return clock_type;
#endif			 
			}
		return clock_type;
	}


    const int MILLISECONDS_PER_SECOND = 1000;
    const int NANOSECONDS_PER_SECOND = 1000000000;
    const int NANOSECONDS_PER_MILLISECOND = 1000000;

    const int MICROSECONDS_PER_SECOND = 1000000;
    const int NANOSECONDS_PER_MICROSECOND = 1000;

    inline void to_time(int milliseconds, xtime& xt)
    {

        xtime_get(&xt, TIME_UTC);

        xt.sec += (milliseconds / MILLISECONDS_PER_SECOND);
        xt.nsec += ((milliseconds % MILLISECONDS_PER_SECOND) * NANOSECONDS_PER_MILLISECOND);

        if (xt.nsec > static_cast<const int>(NANOSECONDS_PER_SECOND))
        {
            ++xt.sec;
            xt.nsec -= NANOSECONDS_PER_SECOND;
		}
		

        
    }


    inline void to_timespec(const xtime& xt, timespec& ts)
    {
        ts.tv_sec = static_cast<int>(xt.sec);
        ts.tv_nsec = static_cast<int>(xt.nsec);
        if(ts.tv_nsec > static_cast<const int>(NANOSECONDS_PER_SECOND))
        {
            ts.tv_sec += ts.tv_nsec / NANOSECONDS_PER_SECOND;
            ts.tv_nsec %= NANOSECONDS_PER_SECOND;
        }
    }

    inline void to_time(int milliseconds, timespec& ts)
    {
        xtime xt;
        to_time(milliseconds, xt);
        to_timespec(xt, ts);


        	
    }

    inline void to_timespec_duration(const xtime& xt, timespec& ts)
    {
        xtime cur;
        
		xtime_get(&cur, TIME_UTC);
        
        if (xt.sec < cur.sec || (xt.sec == cur.sec && xt.nsec < cur.nsec))
        {
            ts.tv_sec = 0;
            ts.tv_nsec = 0;
        }
        else
        {
            ts.tv_sec = xt.sec - cur.sec;
            ts.tv_nsec = xt.nsec - cur.nsec;

            if( ts.tv_nsec < 0 )
            {
                ts.tv_sec -= 1;
                ts.tv_nsec += NANOSECONDS_PER_SECOND;
            }
           if(ts.tv_nsec > static_cast<const int>(NANOSECONDS_PER_SECOND))
           {
               ts.tv_sec += ts.tv_nsec / NANOSECONDS_PER_SECOND;
               ts.tv_nsec %= NANOSECONDS_PER_SECOND;
           }
        }
    }



} // end of namespace CLAM

#endif // XTime.hxx

