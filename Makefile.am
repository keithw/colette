AM_CXXFLAGS = $(PICKY_CXXFLAGS)

bin_PROGRAMS = colette linguini

common_source = address.cc address.hh exception.hh ezio.cc ezio.hh \
	file_descriptor.hh poller.cc poller.hh \
	socket.cc socket.hh socket_type.hh timestamp.cc timestamp.hh \
	util.hh util.cc transport.hh transport.cc packet.hh packet.cc \
	int64.hh

colette_SOURCES = colette.cc $(common_source)

linguini_SOURCES = linguini.cc $(common_source)
