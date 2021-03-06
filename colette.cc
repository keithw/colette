#include <cstdlib>

#include "exception.hh"
#include "transport.hh"

int main( int argc, char *argv[] )
{
  try {
    colette( argc, argv );
  } catch ( const Exception & e ) {
    e.perror();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
