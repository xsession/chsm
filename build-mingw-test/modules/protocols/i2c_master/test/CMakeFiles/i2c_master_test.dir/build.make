# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.25

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\cmake\bin\cmake.exe

# The command to remove a file.
RM = C:\cmake\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test

# Include any dependencies generated for this target.
include modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/compiler_depend.make

# Include the progress variables for this target.
include modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/progress.make

# Include the compile flags for this target's objects.
include modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/flags.make

modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/main.c.obj: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/flags.make
modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/main.c.obj: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/includes_C.rsp
modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/main.c.obj: C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/modules/protocols/i2c_master/test/tsrc/main.c
modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/main.c.obj: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/main.c.obj"
	cd /d C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\modules\protocols\i2c_master\test && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/main.c.obj -MF CMakeFiles\i2c_master_test.dir\tsrc\main.c.obj.d -o CMakeFiles\i2c_master_test.dir\tsrc\main.c.obj -c C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\modules\protocols\i2c_master\test\tsrc\main.c

modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/i2c_master_test.dir/tsrc/main.c.i"
	cd /d C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\modules\protocols\i2c_master\test && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\modules\protocols\i2c_master\test\tsrc\main.c > CMakeFiles\i2c_master_test.dir\tsrc\main.c.i

modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/i2c_master_test.dir/tsrc/main.c.s"
	cd /d C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\modules\protocols\i2c_master\test && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\modules\protocols\i2c_master\test\tsrc\main.c -o CMakeFiles\i2c_master_test.dir\tsrc\main.c.s

modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.obj: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/flags.make
modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.obj: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/includes_C.rsp
modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.obj: C:/GIT/WORK/Gabor/paper_dispenser/fw/c/externals/chsm/modules/protocols/i2c_master/test/tsrc/ut_i2c_master_test.c
modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.obj: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.obj"
	cd /d C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\modules\protocols\i2c_master\test && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.obj -MF CMakeFiles\i2c_master_test.dir\tsrc\ut_i2c_master_test.c.obj.d -o CMakeFiles\i2c_master_test.dir\tsrc\ut_i2c_master_test.c.obj -c C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\modules\protocols\i2c_master\test\tsrc\ut_i2c_master_test.c

modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.i"
	cd /d C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\modules\protocols\i2c_master\test && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\modules\protocols\i2c_master\test\tsrc\ut_i2c_master_test.c > CMakeFiles\i2c_master_test.dir\tsrc\ut_i2c_master_test.c.i

modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.s"
	cd /d C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\modules\protocols\i2c_master\test && C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\modules\protocols\i2c_master\test\tsrc\ut_i2c_master_test.c -o CMakeFiles\i2c_master_test.dir\tsrc\ut_i2c_master_test.c.s

# Object files for target i2c_master_test
i2c_master_test_OBJECTS = \
"CMakeFiles/i2c_master_test.dir/tsrc/main.c.obj" \
"CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.obj"

# External object files for target i2c_master_test
i2c_master_test_EXTERNAL_OBJECTS =

bin/i2c_master_test.exe: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/main.c.obj
bin/i2c_master_test.exe: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/tsrc/ut_i2c_master_test.c.obj
bin/i2c_master_test.exe: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/build.make
bin/i2c_master_test.exe: lib/libunity.a
bin/i2c_master_test.exe: lib/libi2c_drv_mock.a
bin/i2c_master_test.exe: lib/libi2c_master.a
bin/i2c_master_test.exe: lib/libcrf.a
bin/i2c_master_test.exe: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/linkLibs.rsp
bin/i2c_master_test.exe: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/objects1
bin/i2c_master_test.exe: modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ..\..\..\..\bin\i2c_master_test.exe"
	cd /d C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\modules\protocols\i2c_master\test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\i2c_master_test.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/build: bin/i2c_master_test.exe
.PHONY : modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/build

modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/clean:
	cd /d C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\modules\protocols\i2c_master\test && $(CMAKE_COMMAND) -P CMakeFiles\i2c_master_test.dir\cmake_clean.cmake
.PHONY : modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/clean

modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\modules\protocols\i2c_master\test C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\modules\protocols\i2c_master\test C:\GIT\WORK\Gabor\paper_dispenser\fw\c\externals\chsm\build-mingw-test\modules\protocols\i2c_master\test\CMakeFiles\i2c_master_test.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : modules/protocols/i2c_master/test/CMakeFiles/i2c_master_test.dir/depend

