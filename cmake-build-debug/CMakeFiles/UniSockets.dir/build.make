# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.13

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\elade\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\183.5429.37\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\elade\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\183.5429.37\bin\cmake\win\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Programming\VisualStudioProjects\CPP\UniSockets

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Programming\VisualStudioProjects\CPP\UniSockets\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/UniSockets.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/UniSockets.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/UniSockets.dir/flags.make

CMakeFiles/UniSockets.dir/main.cpp.obj: CMakeFiles/UniSockets.dir/flags.make
CMakeFiles/UniSockets.dir/main.cpp.obj: CMakeFiles/UniSockets.dir/includes_CXX.rsp
CMakeFiles/UniSockets.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Programming\VisualStudioProjects\CPP\UniSockets\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/UniSockets.dir/main.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\UniSockets.dir\main.cpp.obj -c D:\Programming\VisualStudioProjects\CPP\UniSockets\main.cpp

CMakeFiles/UniSockets.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/UniSockets.dir/main.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Programming\VisualStudioProjects\CPP\UniSockets\main.cpp > CMakeFiles\UniSockets.dir\main.cpp.i

CMakeFiles/UniSockets.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/UniSockets.dir/main.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Programming\VisualStudioProjects\CPP\UniSockets\main.cpp -o CMakeFiles\UniSockets.dir\main.cpp.s

# Object files for target UniSockets
UniSockets_OBJECTS = \
"CMakeFiles/UniSockets.dir/main.cpp.obj"

# External object files for target UniSockets
UniSockets_EXTERNAL_OBJECTS =

UniSockets.exe: CMakeFiles/UniSockets.dir/main.cpp.obj
UniSockets.exe: CMakeFiles/UniSockets.dir/build.make
UniSockets.exe: CMakeFiles/UniSockets.dir/linklibs.rsp
UniSockets.exe: CMakeFiles/UniSockets.dir/objects1.rsp
UniSockets.exe: CMakeFiles/UniSockets.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Programming\VisualStudioProjects\CPP\UniSockets\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable UniSockets.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\UniSockets.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/UniSockets.dir/build: UniSockets.exe

.PHONY : CMakeFiles/UniSockets.dir/build

CMakeFiles/UniSockets.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\UniSockets.dir\cmake_clean.cmake
.PHONY : CMakeFiles/UniSockets.dir/clean

CMakeFiles/UniSockets.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Programming\VisualStudioProjects\CPP\UniSockets D:\Programming\VisualStudioProjects\CPP\UniSockets D:\Programming\VisualStudioProjects\CPP\UniSockets\cmake-build-debug D:\Programming\VisualStudioProjects\CPP\UniSockets\cmake-build-debug D:\Programming\VisualStudioProjects\CPP\UniSockets\cmake-build-debug\CMakeFiles\UniSockets.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/UniSockets.dir/depend

