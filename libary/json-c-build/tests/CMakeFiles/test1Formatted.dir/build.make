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
include tests/CMakeFiles/test1Formatted.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/test1Formatted.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test1Formatted.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test1Formatted.dir/flags.make

tests/CMakeFiles/test1Formatted.dir/test1.c.o: tests/CMakeFiles/test1Formatted.dir/flags.make
tests/CMakeFiles/test1Formatted.dir/test1.c.o: /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/test1.c
tests/CMakeFiles/test1Formatted.dir/test1.c.o: tests/CMakeFiles/test1Formatted.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/test1Formatted.dir/test1.c.o"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/test1Formatted.dir/test1.c.o -MF CMakeFiles/test1Formatted.dir/test1.c.o.d -o CMakeFiles/test1Formatted.dir/test1.c.o -c /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/test1.c

tests/CMakeFiles/test1Formatted.dir/test1.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test1Formatted.dir/test1.c.i"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/test1.c > CMakeFiles/test1Formatted.dir/test1.c.i

tests/CMakeFiles/test1Formatted.dir/test1.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test1Formatted.dir/test1.c.s"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/test1.c -o CMakeFiles/test1Formatted.dir/test1.c.s

tests/CMakeFiles/test1Formatted.dir/parse_flags.c.o: tests/CMakeFiles/test1Formatted.dir/flags.make
tests/CMakeFiles/test1Formatted.dir/parse_flags.c.o: /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/parse_flags.c
tests/CMakeFiles/test1Formatted.dir/parse_flags.c.o: tests/CMakeFiles/test1Formatted.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/CMakeFiles/test1Formatted.dir/parse_flags.c.o"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tests/CMakeFiles/test1Formatted.dir/parse_flags.c.o -MF CMakeFiles/test1Formatted.dir/parse_flags.c.o.d -o CMakeFiles/test1Formatted.dir/parse_flags.c.o -c /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/parse_flags.c

tests/CMakeFiles/test1Formatted.dir/parse_flags.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test1Formatted.dir/parse_flags.c.i"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/parse_flags.c > CMakeFiles/test1Formatted.dir/parse_flags.c.i

tests/CMakeFiles/test1Formatted.dir/parse_flags.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test1Formatted.dir/parse_flags.c.s"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests/parse_flags.c -o CMakeFiles/test1Formatted.dir/parse_flags.c.s

# Object files for target test1Formatted
test1Formatted_OBJECTS = \
"CMakeFiles/test1Formatted.dir/test1.c.o" \
"CMakeFiles/test1Formatted.dir/parse_flags.c.o"

# External object files for target test1Formatted
test1Formatted_EXTERNAL_OBJECTS =

tests/test1Formatted: tests/CMakeFiles/test1Formatted.dir/test1.c.o
tests/test1Formatted: tests/CMakeFiles/test1Formatted.dir/parse_flags.c.o
tests/test1Formatted: tests/CMakeFiles/test1Formatted.dir/build.make
tests/test1Formatted: libjson-c.5.2.0.dylib
tests/test1Formatted: tests/CMakeFiles/test1Formatted.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable test1Formatted"
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test1Formatted.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test1Formatted.dir/build: tests/test1Formatted
.PHONY : tests/CMakeFiles/test1Formatted.dir/build

tests/CMakeFiles/test1Formatted.dir/clean:
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests && $(CMAKE_COMMAND) -P CMakeFiles/test1Formatted.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test1Formatted.dir/clean

tests/CMakeFiles/test1Formatted.dir/depend:
	cd /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c/tests /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests /Users/andreasjackchristiansen/Desktop/UNI/P1/P1-Energy-Monitoring/libary/json-c-build/tests/CMakeFiles/test1Formatted.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test1Formatted.dir/depend

