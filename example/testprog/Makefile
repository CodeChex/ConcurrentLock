######################################################################
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
WORK_ROOT = /export/dev
PRJ_ROOT = $(WORK_ROOT)/ConcurrentLock
SRC_ROOT = $(PRJ_ROOT)/testprog
######################################################################
.include "$(WORK_ROOT)/_build/unix/makefile.pre"
######################################################################
.PATH:	$(WORK_ROOT)/common ..
######################################################################
#       Variables for BASE system
######################################################################
INC_DIR+= -I/usr/include/sp/include -I$(PRJ_ROOT)/redist
CCFLAGS+= -g -DUNIX -DLinuxi686 -O2
LDFLAGS+= -lpthread 
LDFLAGS+= -L$(PRJ_ROOT)/redist/linux -lConcurrentLock
A_OUT=$(PRJ_ROOT)/redist/linux/testprog
CPP_FILES= benchmark.cpp benchmarkDlg.cpp benchmarkThreadDlg.cpp
HDRS= benchmark.h benchmarkDlg.h benchmarkThreadDlg.h $(PRJ_ROOT)/redist/ConcurrentLock.h
######################################################################
.include "$(WORK_ROOT)/_build/unix/makefile.rul"
######################################################################
.MAIN: build_shared
######################################################################
.include "$(WORK_ROOT)/_build/unix/makefile.pst"
######################################################################
# Dependencies created through MAKEDEPS
######################################################################
# DO NOT DELETE
benchmark.o: benchmark.cpp benchmark.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/gconv.h /usr/include/bits/stdio_lim.h \
  /usr/include/malloc.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h /usr/include/pthread.h /usr/include/sched.h \
  /usr/include/time.h /usr/include/bits/time.h /usr/include/signal.h \
  /usr/include/bits/sigset.h /usr/include/bits/sigthread.h \
  /export/dev/ConcurrentLock/redist/ConcurrentLock.h \
  /usr/include/g++-3/stl.h /usr/include/g++-3/algorithm \
  /usr/include/g++-3/stl_algobase.h /usr/include/g++-3/stl_config.h \
  /usr/include/g++-3/stl_relops.h /usr/include/g++-3/stl_pair.h \
  /usr/include/g++-3/type_traits.h /usr/include/string.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/sys/sysmacros.h \
  /usr/include/alloca.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/new.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/new \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception \
  /usr/include/g++-3/iostream.h /usr/include/g++-3/streambuf.h \
  /usr/include/g++-3/stl_iterator.h /usr/include/g++-3/stl_construct.h \
  /usr/include/g++-3/stl_uninitialized.h /usr/include/g++-3/stl_tempbuf.h \
  /usr/include/g++-3/stl_algo.h /usr/include/g++-3/stl_heap.h \
  /usr/include/g++-3/deque /usr/include/g++-3/stl_alloc.h \
  /usr/include/assert.h /usr/include/g++-3/stl_deque.h \
  /usr/include/g++-3/functional /usr/include/g++-3/stl_function.h \
  /usr/include/g++-3/iterator /usr/include/g++-3/list \
  /usr/include/g++-3/stl_list.h /usr/include/g++-3/map \
  /usr/include/g++-3/stl_tree.h /usr/include/g++-3/stl_map.h \
  /usr/include/g++-3/stl_multimap.h /usr/include/g++-3/memory \
  /usr/include/g++-3/stl_raw_storage_iter.h /usr/include/g++-3/numeric \
  /usr/include/g++-3/stl_numeric.h /usr/include/g++-3/set \
  /usr/include/g++-3/stl_set.h /usr/include/g++-3/stl_multiset.h \
  /usr/include/g++-3/stack /usr/include/g++-3/stl_stack.h \
  /usr/include/g++-3/utility /usr/include/g++-3/vector \
  /usr/include/g++-3/stl_vector.h /usr/include/g++-3/stl_bvector.h \
  /usr/include/glib.h /usr/lib/glib/include/glibconfig.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h \
  /usr/include/sys/time.h benchmarkDlg.h benchmarkThreadDlg.h
benchmarkDlg.o: benchmarkDlg.cpp benchmark.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/gconv.h /usr/include/bits/stdio_lim.h \
  /usr/include/malloc.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h /usr/include/pthread.h /usr/include/sched.h \
  /usr/include/time.h /usr/include/bits/time.h /usr/include/signal.h \
  /usr/include/bits/sigset.h /usr/include/bits/sigthread.h \
  /export/dev/ConcurrentLock/redist/ConcurrentLock.h \
  /usr/include/g++-3/stl.h /usr/include/g++-3/algorithm \
  /usr/include/g++-3/stl_algobase.h /usr/include/g++-3/stl_config.h \
  /usr/include/g++-3/stl_relops.h /usr/include/g++-3/stl_pair.h \
  /usr/include/g++-3/type_traits.h /usr/include/string.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/sys/sysmacros.h \
  /usr/include/alloca.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/new.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/new \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception \
  /usr/include/g++-3/iostream.h /usr/include/g++-3/streambuf.h \
  /usr/include/g++-3/stl_iterator.h /usr/include/g++-3/stl_construct.h \
  /usr/include/g++-3/stl_uninitialized.h /usr/include/g++-3/stl_tempbuf.h \
  /usr/include/g++-3/stl_algo.h /usr/include/g++-3/stl_heap.h \
  /usr/include/g++-3/deque /usr/include/g++-3/stl_alloc.h \
  /usr/include/assert.h /usr/include/g++-3/stl_deque.h \
  /usr/include/g++-3/functional /usr/include/g++-3/stl_function.h \
  /usr/include/g++-3/iterator /usr/include/g++-3/list \
  /usr/include/g++-3/stl_list.h /usr/include/g++-3/map \
  /usr/include/g++-3/stl_tree.h /usr/include/g++-3/stl_map.h \
  /usr/include/g++-3/stl_multimap.h /usr/include/g++-3/memory \
  /usr/include/g++-3/stl_raw_storage_iter.h /usr/include/g++-3/numeric \
  /usr/include/g++-3/stl_numeric.h /usr/include/g++-3/set \
  /usr/include/g++-3/stl_set.h /usr/include/g++-3/stl_multiset.h \
  /usr/include/g++-3/stack /usr/include/g++-3/stl_stack.h \
  /usr/include/g++-3/utility /usr/include/g++-3/vector \
  /usr/include/g++-3/stl_vector.h /usr/include/g++-3/stl_bvector.h \
  /usr/include/glib.h /usr/lib/glib/include/glibconfig.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h \
  /usr/include/sys/time.h benchmarkDlg.h benchmarkThreadDlg.h
benchmarkThreadDlg.o: benchmarkThreadDlg.cpp benchmark.h \
  /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/gconv.h /usr/include/bits/stdio_lim.h \
  /usr/include/malloc.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h /usr/include/pthread.h /usr/include/sched.h \
  /usr/include/time.h /usr/include/bits/time.h /usr/include/signal.h \
  /usr/include/bits/sigset.h /usr/include/bits/sigthread.h \
  /export/dev/ConcurrentLock/redist/ConcurrentLock.h \
  /usr/include/g++-3/stl.h /usr/include/g++-3/algorithm \
  /usr/include/g++-3/stl_algobase.h /usr/include/g++-3/stl_config.h \
  /usr/include/g++-3/stl_relops.h /usr/include/g++-3/stl_pair.h \
  /usr/include/g++-3/type_traits.h /usr/include/string.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/sys/sysmacros.h \
  /usr/include/alloca.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/new.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/new \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception \
  /usr/include/g++-3/iostream.h /usr/include/g++-3/streambuf.h \
  /usr/include/g++-3/stl_iterator.h /usr/include/g++-3/stl_construct.h \
  /usr/include/g++-3/stl_uninitialized.h /usr/include/g++-3/stl_tempbuf.h \
  /usr/include/g++-3/stl_algo.h /usr/include/g++-3/stl_heap.h \
  /usr/include/g++-3/deque /usr/include/g++-3/stl_alloc.h \
  /usr/include/assert.h /usr/include/g++-3/stl_deque.h \
  /usr/include/g++-3/functional /usr/include/g++-3/stl_function.h \
  /usr/include/g++-3/iterator /usr/include/g++-3/list \
  /usr/include/g++-3/stl_list.h /usr/include/g++-3/map \
  /usr/include/g++-3/stl_tree.h /usr/include/g++-3/stl_map.h \
  /usr/include/g++-3/stl_multimap.h /usr/include/g++-3/memory \
  /usr/include/g++-3/stl_raw_storage_iter.h /usr/include/g++-3/numeric \
  /usr/include/g++-3/stl_numeric.h /usr/include/g++-3/set \
  /usr/include/g++-3/stl_set.h /usr/include/g++-3/stl_multiset.h \
  /usr/include/g++-3/stack /usr/include/g++-3/stl_stack.h \
  /usr/include/g++-3/utility /usr/include/g++-3/vector \
  /usr/include/g++-3/stl_vector.h /usr/include/g++-3/stl_bvector.h \
  /usr/include/glib.h /usr/lib/glib/include/glibconfig.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h \
  /usr/include/sys/time.h benchmarkDlg.h benchmarkThreadDlg.h
