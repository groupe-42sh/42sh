# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/k-ou/amar.slimane-42sh-apping

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/k-ou/amar.slimane-42sh-apping/build

# Include any dependencies generated for this target.
include CMakeFiles/42sh.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/42sh.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/42sh.dir/flags.make

CMakeFiles/42sh.dir/src/ast.c.o: CMakeFiles/42sh.dir/flags.make
CMakeFiles/42sh.dir/src/ast.c.o: ../src/ast.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/k-ou/amar.slimane-42sh-apping/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/42sh.dir/src/ast.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/42sh.dir/src/ast.c.o   -c /home/k-ou/amar.slimane-42sh-apping/src/ast.c

CMakeFiles/42sh.dir/src/ast.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/42sh.dir/src/ast.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/k-ou/amar.slimane-42sh-apping/src/ast.c > CMakeFiles/42sh.dir/src/ast.c.i

CMakeFiles/42sh.dir/src/ast.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/42sh.dir/src/ast.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/k-ou/amar.slimane-42sh-apping/src/ast.c -o CMakeFiles/42sh.dir/src/ast.c.s

CMakeFiles/42sh.dir/src/ast.c.o.requires:

.PHONY : CMakeFiles/42sh.dir/src/ast.c.o.requires

CMakeFiles/42sh.dir/src/ast.c.o.provides: CMakeFiles/42sh.dir/src/ast.c.o.requires
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/ast.c.o.provides.build
.PHONY : CMakeFiles/42sh.dir/src/ast.c.o.provides

CMakeFiles/42sh.dir/src/ast.c.o.provides.build: CMakeFiles/42sh.dir/src/ast.c.o


CMakeFiles/42sh.dir/src/extract.c.o: CMakeFiles/42sh.dir/flags.make
CMakeFiles/42sh.dir/src/extract.c.o: ../src/extract.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/k-ou/amar.slimane-42sh-apping/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/42sh.dir/src/extract.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/42sh.dir/src/extract.c.o   -c /home/k-ou/amar.slimane-42sh-apping/src/extract.c

CMakeFiles/42sh.dir/src/extract.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/42sh.dir/src/extract.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/k-ou/amar.slimane-42sh-apping/src/extract.c > CMakeFiles/42sh.dir/src/extract.c.i

CMakeFiles/42sh.dir/src/extract.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/42sh.dir/src/extract.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/k-ou/amar.slimane-42sh-apping/src/extract.c -o CMakeFiles/42sh.dir/src/extract.c.s

CMakeFiles/42sh.dir/src/extract.c.o.requires:

.PHONY : CMakeFiles/42sh.dir/src/extract.c.o.requires

CMakeFiles/42sh.dir/src/extract.c.o.provides: CMakeFiles/42sh.dir/src/extract.c.o.requires
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/extract.c.o.provides.build
.PHONY : CMakeFiles/42sh.dir/src/extract.c.o.provides

CMakeFiles/42sh.dir/src/extract.c.o.provides.build: CMakeFiles/42sh.dir/src/extract.c.o


CMakeFiles/42sh.dir/src/parser.c.o: CMakeFiles/42sh.dir/flags.make
CMakeFiles/42sh.dir/src/parser.c.o: ../src/parser.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/k-ou/amar.slimane-42sh-apping/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/42sh.dir/src/parser.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/42sh.dir/src/parser.c.o   -c /home/k-ou/amar.slimane-42sh-apping/src/parser.c

CMakeFiles/42sh.dir/src/parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/42sh.dir/src/parser.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/k-ou/amar.slimane-42sh-apping/src/parser.c > CMakeFiles/42sh.dir/src/parser.c.i

CMakeFiles/42sh.dir/src/parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/42sh.dir/src/parser.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/k-ou/amar.slimane-42sh-apping/src/parser.c -o CMakeFiles/42sh.dir/src/parser.c.s

CMakeFiles/42sh.dir/src/parser.c.o.requires:

.PHONY : CMakeFiles/42sh.dir/src/parser.c.o.requires

CMakeFiles/42sh.dir/src/parser.c.o.provides: CMakeFiles/42sh.dir/src/parser.c.o.requires
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/parser.c.o.provides.build
.PHONY : CMakeFiles/42sh.dir/src/parser.c.o.provides

CMakeFiles/42sh.dir/src/parser.c.o.provides.build: CMakeFiles/42sh.dir/src/parser.c.o


CMakeFiles/42sh.dir/src/ini.c.o: CMakeFiles/42sh.dir/flags.make
CMakeFiles/42sh.dir/src/ini.c.o: ../src/ini.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/k-ou/amar.slimane-42sh-apping/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/42sh.dir/src/ini.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/42sh.dir/src/ini.c.o   -c /home/k-ou/amar.slimane-42sh-apping/src/ini.c

CMakeFiles/42sh.dir/src/ini.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/42sh.dir/src/ini.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/k-ou/amar.slimane-42sh-apping/src/ini.c > CMakeFiles/42sh.dir/src/ini.c.i

CMakeFiles/42sh.dir/src/ini.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/42sh.dir/src/ini.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/k-ou/amar.slimane-42sh-apping/src/ini.c -o CMakeFiles/42sh.dir/src/ini.c.s

CMakeFiles/42sh.dir/src/ini.c.o.requires:

.PHONY : CMakeFiles/42sh.dir/src/ini.c.o.requires

CMakeFiles/42sh.dir/src/ini.c.o.provides: CMakeFiles/42sh.dir/src/ini.c.o.requires
	$(MAKE) -f CMakeFiles/42sh.dir/build.make CMakeFiles/42sh.dir/src/ini.c.o.provides.build
.PHONY : CMakeFiles/42sh.dir/src/ini.c.o.provides

CMakeFiles/42sh.dir/src/ini.c.o.provides.build: CMakeFiles/42sh.dir/src/ini.c.o


# Object files for target 42sh
42sh_OBJECTS = \
"CMakeFiles/42sh.dir/src/ast.c.o" \
"CMakeFiles/42sh.dir/src/extract.c.o" \
"CMakeFiles/42sh.dir/src/parser.c.o" \
"CMakeFiles/42sh.dir/src/ini.c.o"

# External object files for target 42sh
42sh_EXTERNAL_OBJECTS =

42sh: CMakeFiles/42sh.dir/src/ast.c.o
42sh: CMakeFiles/42sh.dir/src/extract.c.o
42sh: CMakeFiles/42sh.dir/src/parser.c.o
42sh: CMakeFiles/42sh.dir/src/ini.c.o
42sh: CMakeFiles/42sh.dir/build.make
42sh: CMakeFiles/42sh.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/k-ou/amar.slimane-42sh-apping/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable 42sh"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/42sh.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/42sh.dir/build: 42sh

.PHONY : CMakeFiles/42sh.dir/build

CMakeFiles/42sh.dir/requires: CMakeFiles/42sh.dir/src/ast.c.o.requires
CMakeFiles/42sh.dir/requires: CMakeFiles/42sh.dir/src/extract.c.o.requires
CMakeFiles/42sh.dir/requires: CMakeFiles/42sh.dir/src/parser.c.o.requires
CMakeFiles/42sh.dir/requires: CMakeFiles/42sh.dir/src/ini.c.o.requires

.PHONY : CMakeFiles/42sh.dir/requires

CMakeFiles/42sh.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/42sh.dir/cmake_clean.cmake
.PHONY : CMakeFiles/42sh.dir/clean

CMakeFiles/42sh.dir/depend:
	cd /home/k-ou/amar.slimane-42sh-apping/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/k-ou/amar.slimane-42sh-apping /home/k-ou/amar.slimane-42sh-apping /home/k-ou/amar.slimane-42sh-apping/build /home/k-ou/amar.slimane-42sh-apping/build /home/k-ou/amar.slimane-42sh-apping/build/CMakeFiles/42sh.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/42sh.dir/depend

