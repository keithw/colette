#ifndef INT64_HH
#define INT64_HH

/* Helper class to represent a 64-bit integer that
   will be transmitted in network byte order */

#include <endian.h>
#include <string>

#include "exception.hh"

class Integer64 {
private:
  uint64_t contents_ = 0;

public:
  Integer64( void ) {}

  Integer64( const uint64_t contents ) : contents_( contents ) {}

  /* Construct integer from network-byte-order string */
  Integer64( const std::string & contents )
  {
    if ( contents.size() != sizeof( uint64_t ) ) {
      throw Exception( "int64 constructor", "size mismatch" );
    }

    contents_ = be64toh( *(uint64_t *)contents.data() );
  }

  /* Produce network-byte-order representation of integer */
  operator std::string () const
  {
    const uint64_t network_order = htobe64( contents_ );
    return std::string( (char *)&network_order, sizeof( network_order ) );
  }

  /* access underlying contents */
  operator const uint64_t & () const { return contents_; }
};

#endif
