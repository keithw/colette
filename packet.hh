#ifndef PACKET_HH
#define PACKET_HH

#include <string>

#include "address.hh"
#include "int64.hh"

/* Packet class */
class Packet {
private:
  Integer64 sequence_number_;
  Integer64 send_timestamp_ = 0;

  Integer64 ack_sequence_number_ = 0;
  Integer64 ack_send_timestamp_ = 0;

  static const unsigned int HEADER_SIZE = sizeof( Integer64 ) * 4;
  static const unsigned int DATA_PACKET_SIZE = 1472;

  /* Length of the payload when the packet is transmitted.
     In Datagrump the payload is just filled with garbage data. */
  unsigned int payload_len_;

public:
  /* Make outgoing data packet */
  Packet( const uint64_t sequence_number );

  /* Make ACK */
  Packet( const uint64_t sequence_number, const Packet & other );

  /* Make incoming packet from wire */
  Packet( const std::string & str );

  /* Prepare to send */
  void set_send_timestamp( void );

  /* Make wire representation of packet */
  std::string str( void ) const;

  /* Getters */
  const uint64_t & sequence_number( void ) const { return sequence_number_; }
  const uint64_t & send_timestamp( void ) const { return send_timestamp_; }
  const uint64_t & ack_sequence_number( void ) const { return ack_sequence_number_; }
  const uint64_t & ack_send_timestamp( void ) const { return ack_send_timestamp_; }

  const unsigned int & payload_len( void ) const { return payload_len_; }

  /* An ACK has an ack_sequence_number less than 2^64 - 1. */
  bool is_ack( void ) const;
};

#endif
