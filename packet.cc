#include <cassert>

#include "address.hh"
#include "packet.hh"
#include "timestamp.hh"

using namespace std;

/* Make outgoing data packet */
Packet::Packet( const uint64_t sequence_number )
  : sequence_number_( sequence_number ),
    ack_sequence_number_( -1 ),
    payload_len_( DATA_PACKET_SIZE - HEADER_SIZE )
{
  assert( !is_ack() );
}

/* Make ACK */
Packet::Packet( const uint64_t sequence_number, const Packet & other )
  : sequence_number_( sequence_number ),
    ack_sequence_number_( other.sequence_number() ),
    ack_send_timestamp_( other.send_timestamp() ),
    payload_len_( 0 )
{
  assert( is_ack() );
}

/* Make incoming packet from wire */
Packet::Packet( const string & str )
  : sequence_number_(),
    payload_len_()
{
  if ( str.size() < HEADER_SIZE ) {
    throw string( "Incoming datagram not long enough to decode." );
  }

  sequence_number_ = str.substr( 0*sizeof( uint64_t ), sizeof( uint64_t ) );
  send_timestamp_ = str.substr( 1*sizeof( uint64_t ), sizeof( uint64_t ) );
  ack_sequence_number_ = str.substr( 2*sizeof(uint64_t), sizeof(uint64_t) );
  ack_send_timestamp_ = str.substr( 3*sizeof(uint64_t), sizeof(uint64_t) );
  payload_len_ = str.size() - HEADER_SIZE;
}

/* Prepare to send */
void Packet::set_send_timestamp( void )
{
  /* Fill in send timestamp */
  send_timestamp_ = timestamp();
}

/* Make wire representation of packet */
string Packet::str( void ) const
{
  string ret = static_cast<string>( sequence_number_ )
    + static_cast<string>( send_timestamp_ )
    + static_cast<string>( ack_sequence_number_ )
    + static_cast<string>( ack_send_timestamp_ )
    + string( payload_len_, 'x' );

  assert( ret.size() <= DATA_PACKET_SIZE );

  return ret;
}

/* An ACK has an ack_sequence_number less than 2^64 - 1. */
bool Packet::is_ack( void ) const
{
  if ( ack_sequence_number() == uint64_t( -1 ) ) {
    return false;
  }

  /* It's an ACK. */
  assert( payload_len_ == 0 );
  return true;
}
