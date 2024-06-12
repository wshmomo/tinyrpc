# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wsh/code_rpc/tinyrpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wsh/code_rpc/tinyrpc

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/wsh/code_rpc/tinyrpc/CMakeFiles /home/wsh/code_rpc/tinyrpc//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/wsh/code_rpc/tinyrpc/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named test_log

# Build rule for target.
test_log: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 test_log
.PHONY : test_log

# fast build rule for target.
test_log/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/build
.PHONY : test_log/fast

#=============================================================================
# Target rules for targets named test_eventloop

# Build rule for target.
test_eventloop: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 test_eventloop
.PHONY : test_eventloop

# fast build rule for target.
test_eventloop/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/build
.PHONY : test_eventloop/fast

testcases/test_eventloop.o: testcases/test_eventloop.cc.o
.PHONY : testcases/test_eventloop.o

# target to build an object file
testcases/test_eventloop.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/testcases/test_eventloop.cc.o
.PHONY : testcases/test_eventloop.cc.o

testcases/test_eventloop.i: testcases/test_eventloop.cc.i
.PHONY : testcases/test_eventloop.i

# target to preprocess a source file
testcases/test_eventloop.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/testcases/test_eventloop.cc.i
.PHONY : testcases/test_eventloop.cc.i

testcases/test_eventloop.s: testcases/test_eventloop.cc.s
.PHONY : testcases/test_eventloop.s

# target to generate assembly for a file
testcases/test_eventloop.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/testcases/test_eventloop.cc.s
.PHONY : testcases/test_eventloop.cc.s

testcases/test_log.o: testcases/test_log.cc.o
.PHONY : testcases/test_log.o

# target to build an object file
testcases/test_log.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/testcases/test_log.cc.o
.PHONY : testcases/test_log.cc.o

testcases/test_log.i: testcases/test_log.cc.i
.PHONY : testcases/test_log.i

# target to preprocess a source file
testcases/test_log.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/testcases/test_log.cc.i
.PHONY : testcases/test_log.cc.i

testcases/test_log.s: testcases/test_log.cc.s
.PHONY : testcases/test_log.s

# target to generate assembly for a file
testcases/test_log.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/testcases/test_log.cc.s
.PHONY : testcases/test_log.cc.s

tinyrpc/common/config.o: tinyrpc/common/config.cc.o
.PHONY : tinyrpc/common/config.o

# target to build an object file
tinyrpc/common/config.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/config.cc.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/config.cc.o
.PHONY : tinyrpc/common/config.cc.o

tinyrpc/common/config.i: tinyrpc/common/config.cc.i
.PHONY : tinyrpc/common/config.i

# target to preprocess a source file
tinyrpc/common/config.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/config.cc.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/config.cc.i
.PHONY : tinyrpc/common/config.cc.i

tinyrpc/common/config.s: tinyrpc/common/config.cc.s
.PHONY : tinyrpc/common/config.s

# target to generate assembly for a file
tinyrpc/common/config.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/config.cc.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/config.cc.s
.PHONY : tinyrpc/common/config.cc.s

tinyrpc/common/log.o: tinyrpc/common/log.cc.o
.PHONY : tinyrpc/common/log.o

# target to build an object file
tinyrpc/common/log.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/log.cc.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/log.cc.o
.PHONY : tinyrpc/common/log.cc.o

tinyrpc/common/log.i: tinyrpc/common/log.cc.i
.PHONY : tinyrpc/common/log.i

# target to preprocess a source file
tinyrpc/common/log.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/log.cc.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/log.cc.i
.PHONY : tinyrpc/common/log.cc.i

tinyrpc/common/log.s: tinyrpc/common/log.cc.s
.PHONY : tinyrpc/common/log.s

# target to generate assembly for a file
tinyrpc/common/log.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/log.cc.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/log.cc.s
.PHONY : tinyrpc/common/log.cc.s

tinyrpc/common/tinystr.o: tinyrpc/common/tinystr.cpp.o
.PHONY : tinyrpc/common/tinystr.o

# target to build an object file
tinyrpc/common/tinystr.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinystr.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinystr.cpp.o
.PHONY : tinyrpc/common/tinystr.cpp.o

tinyrpc/common/tinystr.i: tinyrpc/common/tinystr.cpp.i
.PHONY : tinyrpc/common/tinystr.i

# target to preprocess a source file
tinyrpc/common/tinystr.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinystr.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinystr.cpp.i
.PHONY : tinyrpc/common/tinystr.cpp.i

tinyrpc/common/tinystr.s: tinyrpc/common/tinystr.cpp.s
.PHONY : tinyrpc/common/tinystr.s

# target to generate assembly for a file
tinyrpc/common/tinystr.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinystr.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinystr.cpp.s
.PHONY : tinyrpc/common/tinystr.cpp.s

tinyrpc/common/tinyxml.o: tinyrpc/common/tinyxml.cpp.o
.PHONY : tinyrpc/common/tinyxml.o

# target to build an object file
tinyrpc/common/tinyxml.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinyxml.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinyxml.cpp.o
.PHONY : tinyrpc/common/tinyxml.cpp.o

tinyrpc/common/tinyxml.i: tinyrpc/common/tinyxml.cpp.i
.PHONY : tinyrpc/common/tinyxml.i

# target to preprocess a source file
tinyrpc/common/tinyxml.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinyxml.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinyxml.cpp.i
.PHONY : tinyrpc/common/tinyxml.cpp.i

tinyrpc/common/tinyxml.s: tinyrpc/common/tinyxml.cpp.s
.PHONY : tinyrpc/common/tinyxml.s

# target to generate assembly for a file
tinyrpc/common/tinyxml.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinyxml.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinyxml.cpp.s
.PHONY : tinyrpc/common/tinyxml.cpp.s

tinyrpc/common/tinyxmlerror.o: tinyrpc/common/tinyxmlerror.cpp.o
.PHONY : tinyrpc/common/tinyxmlerror.o

# target to build an object file
tinyrpc/common/tinyxmlerror.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinyxmlerror.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinyxmlerror.cpp.o
.PHONY : tinyrpc/common/tinyxmlerror.cpp.o

tinyrpc/common/tinyxmlerror.i: tinyrpc/common/tinyxmlerror.cpp.i
.PHONY : tinyrpc/common/tinyxmlerror.i

# target to preprocess a source file
tinyrpc/common/tinyxmlerror.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinyxmlerror.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinyxmlerror.cpp.i
.PHONY : tinyrpc/common/tinyxmlerror.cpp.i

tinyrpc/common/tinyxmlerror.s: tinyrpc/common/tinyxmlerror.cpp.s
.PHONY : tinyrpc/common/tinyxmlerror.s

# target to generate assembly for a file
tinyrpc/common/tinyxmlerror.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinyxmlerror.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinyxmlerror.cpp.s
.PHONY : tinyrpc/common/tinyxmlerror.cpp.s

tinyrpc/common/tinyxmlparser.o: tinyrpc/common/tinyxmlparser.cpp.o
.PHONY : tinyrpc/common/tinyxmlparser.o

# target to build an object file
tinyrpc/common/tinyxmlparser.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinyxmlparser.cpp.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinyxmlparser.cpp.o
.PHONY : tinyrpc/common/tinyxmlparser.cpp.o

tinyrpc/common/tinyxmlparser.i: tinyrpc/common/tinyxmlparser.cpp.i
.PHONY : tinyrpc/common/tinyxmlparser.i

# target to preprocess a source file
tinyrpc/common/tinyxmlparser.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinyxmlparser.cpp.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinyxmlparser.cpp.i
.PHONY : tinyrpc/common/tinyxmlparser.cpp.i

tinyrpc/common/tinyxmlparser.s: tinyrpc/common/tinyxmlparser.cpp.s
.PHONY : tinyrpc/common/tinyxmlparser.s

# target to generate assembly for a file
tinyrpc/common/tinyxmlparser.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/tinyxmlparser.cpp.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/tinyxmlparser.cpp.s
.PHONY : tinyrpc/common/tinyxmlparser.cpp.s

tinyrpc/common/util.o: tinyrpc/common/util.cc.o
.PHONY : tinyrpc/common/util.o

# target to build an object file
tinyrpc/common/util.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/util.cc.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/util.cc.o
.PHONY : tinyrpc/common/util.cc.o

tinyrpc/common/util.i: tinyrpc/common/util.cc.i
.PHONY : tinyrpc/common/util.i

# target to preprocess a source file
tinyrpc/common/util.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/util.cc.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/util.cc.i
.PHONY : tinyrpc/common/util.cc.i

tinyrpc/common/util.s: tinyrpc/common/util.cc.s
.PHONY : tinyrpc/common/util.s

# target to generate assembly for a file
tinyrpc/common/util.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/common/util.cc.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/common/util.cc.s
.PHONY : tinyrpc/common/util.cc.s

tinyrpc/net/eventloop.o: tinyrpc/net/eventloop.cc.o
.PHONY : tinyrpc/net/eventloop.o

# target to build an object file
tinyrpc/net/eventloop.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/eventloop.cc.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/eventloop.cc.o
.PHONY : tinyrpc/net/eventloop.cc.o

tinyrpc/net/eventloop.i: tinyrpc/net/eventloop.cc.i
.PHONY : tinyrpc/net/eventloop.i

# target to preprocess a source file
tinyrpc/net/eventloop.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/eventloop.cc.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/eventloop.cc.i
.PHONY : tinyrpc/net/eventloop.cc.i

tinyrpc/net/eventloop.s: tinyrpc/net/eventloop.cc.s
.PHONY : tinyrpc/net/eventloop.s

# target to generate assembly for a file
tinyrpc/net/eventloop.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/eventloop.cc.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/eventloop.cc.s
.PHONY : tinyrpc/net/eventloop.cc.s

tinyrpc/net/fd_event.o: tinyrpc/net/fd_event.cc.o
.PHONY : tinyrpc/net/fd_event.o

# target to build an object file
tinyrpc/net/fd_event.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/fd_event.cc.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/fd_event.cc.o
.PHONY : tinyrpc/net/fd_event.cc.o

tinyrpc/net/fd_event.i: tinyrpc/net/fd_event.cc.i
.PHONY : tinyrpc/net/fd_event.i

# target to preprocess a source file
tinyrpc/net/fd_event.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/fd_event.cc.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/fd_event.cc.i
.PHONY : tinyrpc/net/fd_event.cc.i

tinyrpc/net/fd_event.s: tinyrpc/net/fd_event.cc.s
.PHONY : tinyrpc/net/fd_event.s

# target to generate assembly for a file
tinyrpc/net/fd_event.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/fd_event.cc.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/fd_event.cc.s
.PHONY : tinyrpc/net/fd_event.cc.s

tinyrpc/net/io_thread.o: tinyrpc/net/io_thread.cc.o
.PHONY : tinyrpc/net/io_thread.o

# target to build an object file
tinyrpc/net/io_thread.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/io_thread.cc.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/io_thread.cc.o
.PHONY : tinyrpc/net/io_thread.cc.o

tinyrpc/net/io_thread.i: tinyrpc/net/io_thread.cc.i
.PHONY : tinyrpc/net/io_thread.i

# target to preprocess a source file
tinyrpc/net/io_thread.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/io_thread.cc.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/io_thread.cc.i
.PHONY : tinyrpc/net/io_thread.cc.i

tinyrpc/net/io_thread.s: tinyrpc/net/io_thread.cc.s
.PHONY : tinyrpc/net/io_thread.s

# target to generate assembly for a file
tinyrpc/net/io_thread.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/io_thread.cc.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/io_thread.cc.s
.PHONY : tinyrpc/net/io_thread.cc.s

tinyrpc/net/timer.o: tinyrpc/net/timer.cc.o
.PHONY : tinyrpc/net/timer.o

# target to build an object file
tinyrpc/net/timer.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/timer.cc.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/timer.cc.o
.PHONY : tinyrpc/net/timer.cc.o

tinyrpc/net/timer.i: tinyrpc/net/timer.cc.i
.PHONY : tinyrpc/net/timer.i

# target to preprocess a source file
tinyrpc/net/timer.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/timer.cc.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/timer.cc.i
.PHONY : tinyrpc/net/timer.cc.i

tinyrpc/net/timer.s: tinyrpc/net/timer.cc.s
.PHONY : tinyrpc/net/timer.s

# target to generate assembly for a file
tinyrpc/net/timer.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/timer.cc.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/timer.cc.s
.PHONY : tinyrpc/net/timer.cc.s

tinyrpc/net/timer_event.o: tinyrpc/net/timer_event.cc.o
.PHONY : tinyrpc/net/timer_event.o

# target to build an object file
tinyrpc/net/timer_event.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/timer_event.cc.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/timer_event.cc.o
.PHONY : tinyrpc/net/timer_event.cc.o

tinyrpc/net/timer_event.i: tinyrpc/net/timer_event.cc.i
.PHONY : tinyrpc/net/timer_event.i

# target to preprocess a source file
tinyrpc/net/timer_event.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/timer_event.cc.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/timer_event.cc.i
.PHONY : tinyrpc/net/timer_event.cc.i

tinyrpc/net/timer_event.s: tinyrpc/net/timer_event.cc.s
.PHONY : tinyrpc/net/timer_event.s

# target to generate assembly for a file
tinyrpc/net/timer_event.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/timer_event.cc.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/timer_event.cc.s
.PHONY : tinyrpc/net/timer_event.cc.s

tinyrpc/net/wakeup_fd_event.o: tinyrpc/net/wakeup_fd_event.cc.o
.PHONY : tinyrpc/net/wakeup_fd_event.o

# target to build an object file
tinyrpc/net/wakeup_fd_event.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/wakeup_fd_event.cc.o
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/wakeup_fd_event.cc.o
.PHONY : tinyrpc/net/wakeup_fd_event.cc.o

tinyrpc/net/wakeup_fd_event.i: tinyrpc/net/wakeup_fd_event.cc.i
.PHONY : tinyrpc/net/wakeup_fd_event.i

# target to preprocess a source file
tinyrpc/net/wakeup_fd_event.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/wakeup_fd_event.cc.i
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/wakeup_fd_event.cc.i
.PHONY : tinyrpc/net/wakeup_fd_event.cc.i

tinyrpc/net/wakeup_fd_event.s: tinyrpc/net/wakeup_fd_event.cc.s
.PHONY : tinyrpc/net/wakeup_fd_event.s

# target to generate assembly for a file
tinyrpc/net/wakeup_fd_event.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_log.dir/build.make CMakeFiles/test_log.dir/tinyrpc/net/wakeup_fd_event.cc.s
	$(MAKE) $(MAKESILENT) -f CMakeFiles/test_eventloop.dir/build.make CMakeFiles/test_eventloop.dir/tinyrpc/net/wakeup_fd_event.cc.s
.PHONY : tinyrpc/net/wakeup_fd_event.cc.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... test_eventloop"
	@echo "... test_log"
	@echo "... testcases/test_eventloop.o"
	@echo "... testcases/test_eventloop.i"
	@echo "... testcases/test_eventloop.s"
	@echo "... testcases/test_log.o"
	@echo "... testcases/test_log.i"
	@echo "... testcases/test_log.s"
	@echo "... tinyrpc/common/config.o"
	@echo "... tinyrpc/common/config.i"
	@echo "... tinyrpc/common/config.s"
	@echo "... tinyrpc/common/log.o"
	@echo "... tinyrpc/common/log.i"
	@echo "... tinyrpc/common/log.s"
	@echo "... tinyrpc/common/tinystr.o"
	@echo "... tinyrpc/common/tinystr.i"
	@echo "... tinyrpc/common/tinystr.s"
	@echo "... tinyrpc/common/tinyxml.o"
	@echo "... tinyrpc/common/tinyxml.i"
	@echo "... tinyrpc/common/tinyxml.s"
	@echo "... tinyrpc/common/tinyxmlerror.o"
	@echo "... tinyrpc/common/tinyxmlerror.i"
	@echo "... tinyrpc/common/tinyxmlerror.s"
	@echo "... tinyrpc/common/tinyxmlparser.o"
	@echo "... tinyrpc/common/tinyxmlparser.i"
	@echo "... tinyrpc/common/tinyxmlparser.s"
	@echo "... tinyrpc/common/util.o"
	@echo "... tinyrpc/common/util.i"
	@echo "... tinyrpc/common/util.s"
	@echo "... tinyrpc/net/eventloop.o"
	@echo "... tinyrpc/net/eventloop.i"
	@echo "... tinyrpc/net/eventloop.s"
	@echo "... tinyrpc/net/fd_event.o"
	@echo "... tinyrpc/net/fd_event.i"
	@echo "... tinyrpc/net/fd_event.s"
	@echo "... tinyrpc/net/io_thread.o"
	@echo "... tinyrpc/net/io_thread.i"
	@echo "... tinyrpc/net/io_thread.s"
	@echo "... tinyrpc/net/timer.o"
	@echo "... tinyrpc/net/timer.i"
	@echo "... tinyrpc/net/timer.s"
	@echo "... tinyrpc/net/timer_event.o"
	@echo "... tinyrpc/net/timer_event.i"
	@echo "... tinyrpc/net/timer_event.s"
	@echo "... tinyrpc/net/wakeup_fd_event.o"
	@echo "... tinyrpc/net/wakeup_fd_event.i"
	@echo "... tinyrpc/net/wakeup_fd_event.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
