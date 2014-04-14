/* -*-mode:c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <ctime>

#include "timestamp.hh"
#include "util.hh"

/* milliseconds per second */
static const uint64_t THOUSAND = 1000;

/* nanoseconds per millisecond */
static const uint64_t MILLION = THOUSAND * THOUSAND;

/* nanoseconds per second */
static const uint64_t BILLION = THOUSAND * MILLION;

/* epoch */
static const uint64_t EPOCH = 1397448000000;

uint64_t timestamp( void )
{
    timespec ts;
    SystemCall( "clock_gettime", clock_gettime( CLOCK_REALTIME, &ts ) );

    return timestamp( ts );
}

uint64_t timestamp( const timespec & ts )
{
    const uint64_t nanos = ts.tv_sec * BILLION + ts.tv_nsec;
    return nanos / MILLION - EPOCH;
}
