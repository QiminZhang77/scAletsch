bin_PROGRAMS=linkmerge

linkmerge_CPPFLAGS = -pthread -I$(top_srcdir)/lib/gtf -std=c++11
linkmerge_LDFLAGS = -L$(top_builddir)/lib/gtf -pthread
linkmerge_LDADD = -lgtf

linkmerge_SOURCES = genome1.h genome1.cc \
				   scAletsch.h scAletsch.cc \
				   config.h config.cc \
				   main.cc
