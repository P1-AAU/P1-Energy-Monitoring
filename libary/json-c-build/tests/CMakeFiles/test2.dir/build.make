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
CMAKE_SOURCE_DIR = /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build

# Include any dependencies generated for this target.
include tests/CMakeFiles/test2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/test2.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test2.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test2.dir/flags.make

tests/CMakeFiles/test2.dir/test2.c.o: tests/CMakeFiles/test2.dir/flags.make
tests/CMakeFiles/test2.dir/test2.c.o: /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/test2.c
tests/CMakeFiles/test2.dir/test2.c.o: tests/CMakeFiles/test2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/test2.dir/test2.c.o"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/test2.dir/test2.c.o -MF CMakeFiles/test2.dir/test2.c.o.d -o CMakeFiles/test2.dir/test2.c.o -c /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/test2.c

tests/CMakeFiles/test2.dir/test2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test2.dir/test2.c.i"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/test2.c > CMakeFiles/test2.dir/test2.c.i

tests/CMakeFiles/test2.dir/test2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test2.dir/test2.c.s"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/test2.c -o CMakeFiles/test2.dir/test2.c.s

# Object files for target test2
test2_OBJECTS = \
"CMakeFiles/test2.dir/test2.c.o"

# External object files for target test2
test2_EXTERNAL_OBJECTS =

tests/test2: tests/CMakeFiles/test2.dir/test2.c.o
tests/test2: tests/CMakeFiles/test2.dir/build.make
tests/test2: libjson-c.5.2.0.dylib
tests/test2: tests/CMakeFiles/test2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test2"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test2.dir/build: tests/test2
.PHONY : tests/CMakeFiles/test2.dir/build

tests/CMakeFiles/test2.dir/clean:
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && $(CMAKE_COMMAND) -P CMakeFiles/test2.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test2.dir/clean

tests/CMakeFiles/test2.dir/depend:
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests/CMakeFiles/test2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test2.dir/depend

