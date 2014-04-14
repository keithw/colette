#include <string>
#include <iostream>

#include "transport.hh"
#include "exception.hh"
#include "socket.hh"
#include "packet.hh"
#include "poller.hh"
#include "timestamp.hh"

using namespace std;
using namespace PollerShortNames;

/* linguini just receives packets and sends ACKs */
void linguini( const int argc, const char * const argv[] )
{
  if ( argc <= 0 ) {
    throw Exception( "Invalid command line", "argc <= 0" );
  }

  if ( argc != 2 ) {
    throw Exception( "Usage", string( argv[ 0 ] ) + " PORT" );
  }

  Socket listener( UDP );
  listener.bind( Address( "0", argv[ 1 ], UDP ) );

  cerr << "Listening on " << listener.local_addr().str() << "..." << endl;

  uint64_t sequence_number = 0;

  while ( true ) {
    /* receive the packet */
    const auto addr_packet = listener.recvfrom();
    const Packet received_packet( addr_packet.second );

    /* send an acknowledgment */
    const Packet ack( sequence_number++, received_packet );
    listener.sendto( addr_packet.first, ack.str() );

    cerr << "Acked " << received_packet.sequence_number() << " to " << addr_packet.first.str() << endl;
  }
}

/* colette sends packets per a RemyCC */
void colette( const int argc, const char * const argv[] )
{
  if ( argc <= 0 ) {
    throw Exception( "Invalid command line", "argc <= 0" );
  }

  if ( argc != 3 ) {
    throw Exception( "Usage", string( argv[ 0 ] ) + " HOSTNAME PORT" );
  }

  Socket destination( UDP );
  destination.connect( Address( argv[ 1 ], argv[ 2 ], UDP ) );

  cerr << "Sending packets to " << destination.peer_addr().str() << "..." << endl;

  Poller poller;
  poller.add_action( Poller::Action( destination.fd(), Direction::In,
				     [&] () {
				       Packet ack( destination.read() );
				       const auto now = timestamp();
				       if ( ack.is_ack() ) {
					 cerr << "Got ack for " << ack.ack_sequence_number()
					      << " with RTT " << now - ack.ack_send_timestamp() << endl;
				       } else {
					 cerr << "Error, got non-ack " << ack.sequence_number() << endl;
				       }
				       return ResultType::Continue;
				     } ) );

  uint64_t sequence_number = 0;
  uint64_t next_outgoing_packet_time = timestamp();
  const uint64_t intersend_time = 1000;

  while ( true ) {
    const uint64_t now = timestamp();
    if ( now < next_outgoing_packet_time ) {
      if ( poller.poll( next_outgoing_packet_time - now ).result == Poller::Result::Type::Exit ) {
	return;
      }
    } else {
      Packet outgoing_packet( sequence_number++ );
      outgoing_packet.set_send_timestamp();
      destination.write( outgoing_packet.str() );
      next_outgoing_packet_time += intersend_time;
      cerr << "Sent " << outgoing_packet.sequence_number() << " at " << outgoing_packet.send_timestamp() << endl;
    }
  }
}
