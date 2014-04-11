/* -*-mode:c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef UTIL_HH
#define UTIL_HH

#include <cstring>
#include <string>

template <typename T> void zero( T & x ) { memset( &x, 0, sizeof( x ) ); }

/* error-checking wrapper for most syscalls */
int SystemCall( const std::string & s_attempt, const int return_value );

#endif /* UTIL_HH */
