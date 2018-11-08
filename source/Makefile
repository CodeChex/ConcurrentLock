######################################################################
#
#     Copyright (c) Checco Computer Services. 1992, 1999.          
#     All Rights Reserved.                                               
#
#     Unix PMAKE file
#
#######################################################################
#
.SILENT:
WORK_ROOT = /work
PRJ_ROOT = $(WORK_ROOT)/ConcurrentLock
SRC_ROOT = $(PRJ_ROOT)/source
######################################################################
.include "$(WORK_ROOT)/_build/unix/makefile.pre"
######################################################################
.PATH:	$(WORK_ROOT)/common $(PRJ_ROOT)/redist
######################################################################
#       Variables for BASE system
######################################################################
CCFLAGS+= -g -DUNIX -DLinuxi686 -O2
LDFLAGS+= -lpthread 
A_OUT=$(PRJ_ROOT)/redist/linux/libConcurrentLock
CPP_FILES= ConcurrentLock.cpp
######################################################################
.include "$(WORK_ROOT)/_build/unix/makefile.rul"
######################################################################
.MAIN: shared_library $(PRJ_ROOT)/redist/ConcurrentLock.h
######################################################################
$(PRJ_ROOT)/redist/ConcurrentLock.h: $(SRC_ROOT)/ConcurrentLock.h
	echo - Copying Public Header File
	cp $(SRC_ROOT)/ConcurrentLock.h $(PRJ_ROOT)/redist/
######################################################################
.include "$(WORK_ROOT)/_build/unix/makefile.pst"
######################################################################
# Dependencies created through MAKEDEPS
######################################################################
# DO NOT DELETE
