# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build

# Include any dependencies generated for this target.
include tests/CMakeFiles/test_float.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/test_float.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_float.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_float.dir/flags.make

tests/CMakeFiles/test_float.dir/test_float.c.o: tests/CMakeFiles/test_float.dir/flags.make
tests/CMakeFiles/test_float.dir/test_float.c.o: /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c/tests/test_float.c
tests/CMakeFiles/test_float.dir/test_float.c.o: tests/CMakeFiles/test_float.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/test_float.dir/test_float.c.o"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/test_float.dir/test_float.c.o -MF CMakeFiles/test_float.dir/test_float.c.o.d -o CMakeFiles/test_float.dir/test_float.c.o -c /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c/tests/test_float.c

tests/CMakeFiles/test_float.dir/test_float.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_float.dir/test_float.c.i"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c/tests/test_float.c > CMakeFiles/test_float.dir/test_float.c.i

tests/CMakeFiles/test_float.dir/test_float.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_float.dir/test_float.c.s"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c/tests/test_float.c -o CMakeFiles/test_float.dir/test_float.c.s

# Object files for target test_float
test_float_OBJECTS = \
"CMakeFiles/test_float.dir/test_float.c.o"

# External object files for target test_float
test_float_EXTERNAL_OBJECTS =

tests/test_float: tests/CMakeFiles/test_float.dir/test_float.c.o
tests/test_float: tests/CMakeFiles/test_float.dir/build.make
tests/test_float: libjson-c.5.2.0.dylib
tests/test_float: tests/CMakeFiles/test_float.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_float"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_float.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_float.dir/build: tests/test_float
.PHONY : tests/CMakeFiles/test_float.dir/build

tests/CMakeFiles/test_float.dir/clean:
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_float.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_float.dir/clean

tests/CMakeFiles/test_float.dir/depend:
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c/tests /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build/tests /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/library/json-c-build/tests/CMakeFiles/test_float.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test_float.dir/depend

