#
# Copyright (c) 2004 by FORCE Computers.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
# file and program are licensed under a BSD style license.  See
# the Copying file included with the OpenHPI distribution for
# full licensing terms.
#
# Authors:
#    Thomas Kanngieser <thomas.kanngieser@fci.com>
#

.NOTPARALLEL:

MAINTAINERCLEANFILES 	= Makefile.in *~

INCLUDES 		= @OPENHPI_INCLUDES@
# just to clear LIBS
LIBS			=

# marshal and connection used by the daemon and client library
lib_LTLIBRARIES = libopenhpitransport.la

libopenhpitransport_la_SOURCES    = \
	strmsock.h \
	strmsock.cpp

libopenhpitransport_la_LDFLAGS= -version-info @HPI_LIB_VERSION@

clean-local:
	rm -f *~ core core.*
