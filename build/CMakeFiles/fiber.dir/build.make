# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/zhangxuan/apps/XCK

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zhangxuan/apps/XCK/build

# Include any dependencies generated for this target.
include CMakeFiles/fiber.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fiber.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fiber.dir/flags.make

CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.o: CMakeFiles/fiber.dir/flags.make
CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.o: ../code/Fiber/Fiber_CC/fiber.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhangxuan/apps/XCK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.o -c /home/zhangxuan/apps/XCK/code/Fiber/Fiber_CC/fiber.cc

CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zhangxuan/apps/XCK/code/Fiber/Fiber_CC/fiber.cc > CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.i

CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zhangxuan/apps/XCK/code/Fiber/Fiber_CC/fiber.cc -o CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.s

CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.o: CMakeFiles/fiber.dir/flags.make
CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.o: ../code/Fiber/Fiber_CC/iomanager.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhangxuan/apps/XCK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.o -c /home/zhangxuan/apps/XCK/code/Fiber/Fiber_CC/iomanager.cc

CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zhangxuan/apps/XCK/code/Fiber/Fiber_CC/iomanager.cc > CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.i

CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zhangxuan/apps/XCK/code/Fiber/Fiber_CC/iomanager.cc -o CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.s

CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.o: CMakeFiles/fiber.dir/flags.make
CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.o: ../code/Fiber/Fiber_CC/scheduler.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhangxuan/apps/XCK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.o -c /home/zhangxuan/apps/XCK/code/Fiber/Fiber_CC/scheduler.cc

CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zhangxuan/apps/XCK/code/Fiber/Fiber_CC/scheduler.cc > CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.i

CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zhangxuan/apps/XCK/code/Fiber/Fiber_CC/scheduler.cc -o CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.s

# Object files for target fiber
fiber_OBJECTS = \
"CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.o" \
"CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.o" \
"CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.o"

# External object files for target fiber
fiber_EXTERNAL_OBJECTS =

../lib/libfiber.so: CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/fiber.cc.o
../lib/libfiber.so: CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/iomanager.cc.o
../lib/libfiber.so: CMakeFiles/fiber.dir/code/Fiber/Fiber_CC/scheduler.cc.o
../lib/libfiber.so: CMakeFiles/fiber.dir/build.make
../lib/libfiber.so: CMakeFiles/fiber.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zhangxuan/apps/XCK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ../lib/libfiber.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fiber.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fiber.dir/build: ../lib/libfiber.so

.PHONY : CMakeFiles/fiber.dir/build

CMakeFiles/fiber.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fiber.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fiber.dir/clean

CMakeFiles/fiber.dir/depend:
	cd /home/zhangxuan/apps/XCK/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhangxuan/apps/XCK /home/zhangxuan/apps/XCK /home/zhangxuan/apps/XCK/build /home/zhangxuan/apps/XCK/build /home/zhangxuan/apps/XCK/build/CMakeFiles/fiber.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fiber.dir/depend

