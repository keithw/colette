/* -*-mode:c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "util.hh"
#include "exception.hh"

/* error-checking wrapper for most syscalls */
int SystemCall( const std::string & s_attempt, const int return_value )
{
    if ( return_value >= 0 ) {
        return return_value;
    }

    throw Exception( s_attempt );
}
