######################################################################
#
#     BioXAuthServer - Biometric Authentication CGI
#                                                                        
#     Copyright (c) Checco Computer Services. 1992, 1999.          
#     All Rights Reserved.                                               
#
#     Unix PMAKE file
#
######################################################################
.IGNORE:
#
.SILENT:
WORK_ROOT = /work
SDK_ROOT = .
PRJ_ROOT = $(WORK_ROOT)/ConcurrentLock
######################################################################
#include "$(WORK_ROOT)/_build/unix/makefile.pre"
######################################################################
.PATH:	$(MAKEOBJDIR) $(WORK_ROOT)/common ..
######################################################################
#       Variables for BASE system
######################################################################
CFLAGS+= -DUNIX -DLinuxi686 -O2 -I$(WORK_ROOT)/common \
	-I/usr/lib/glib/include

LDFLAGS+= -ldbitk -lpthread -lglib -L $(MAKEOBJDIR)

EXE=ConcurrentLock

CPP_FILES=ConcurrentLock.cpp

C_FILES=

######################################################################
#include "$(WORK_ROOT)/_build/unix/makefile.rul"
######################################################################
.MAIN: extproc_callback
######################################################################
#
#       Dependencies created through MAKEDEPS
#
######################################################################
#UPDATE#
$(MAKEOBJDIR)/ConcurrentLock.o: ConcurrentLock.cpp
#ENDUPDATE#
###.include "./ConcurrentLock.dep"
######################################################################
#include "$(WORK_ROOT)/_build/unix/makefile.pst"
######################################################################
