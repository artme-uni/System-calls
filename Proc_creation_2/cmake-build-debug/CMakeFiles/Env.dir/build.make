# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/Users/twiggy/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/193.6911.21/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/twiggy/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/193.6911.21/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/twiggy/Doc_Git/OS/Proc_creation_2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/twiggy/Doc_Git/OS/Proc_creation_2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/env.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/env.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/env.dir/flags.make

CMakeFiles/env.dir/print_env.c.o: CMakeFiles/env.dir/flags.make
CMakeFiles/env.dir/print_env.c.o: ../print_env.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/twiggy/Doc_Git/OS/Proc_creation_2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/env.dir/print_env.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/env.dir/print_env.c.o   -c /Users/twiggy/Doc_Git/OS/Proc_creation_2/print_env.c

CMakeFiles/env.dir/print_env.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/env.dir/print_env.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/twiggy/Doc_Git/OS/Proc_creation_2/print_env.c > CMakeFiles/env.dir/print_env.c.i

CMakeFiles/env.dir/print_env.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/env.dir/print_env.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/twiggy/Doc_Git/OS/Proc_creation_2/print_env.c -o CMakeFiles/env.dir/print_env.c.s

# Object files for target env
env_OBJECTS = \
"CMakeFiles/env.dir/print_env.c.o"

# External object files for target env
env_EXTERNAL_OBJECTS =

env: CMakeFiles/env.dir/print_env.c.o
env: CMakeFiles/env.dir/build.make
env: CMakeFiles/env.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/twiggy/Doc_Git/OS/Proc_creation_2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable env"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/env.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/env.dir/build: env

.PHONY : CMakeFiles/env.dir/build

CMakeFiles/env.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/env.dir/cmake_clean.cmake
.PHONY : CMakeFiles/env.dir/clean

CMakeFiles/env.dir/depend:
	cd /Users/twiggy/Doc_Git/OS/Proc_creation_2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/twiggy/Doc_Git/OS/Proc_creation_2 /Users/twiggy/Doc_Git/OS/Proc_creation_2 /Users/twiggy/Doc_Git/OS/Proc_creation_2/cmake-build-debug /Users/twiggy/Doc_Git/OS/Proc_creation_2/cmake-build-debug /Users/twiggy/Doc_Git/OS/Proc_creation_2/cmake-build-debug/CMakeFiles/env.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/env.dir/depend

