cmake_minimum_required(VERSION 2.8)
Project(lincity)

set(CMAKE_C_FLAGS_GPROF "-pg")
set(CMAKE_EXE_LINKER_FLAGS_GPROF "-pg")

set(
	GUI_SOURCES 
	src/ldsvgui.c
	src/splash.c
	src/lchelp.c
	src/main.c
	src/mouse.c
	src/mps.c
	src/screen.c
	src/pixmap.c
	src/cliglobs.c
	src/geometry.c
	src/pbar.c
	src/module_buttons.c
	src/dialbox.c
	src/lcsdl.c
   )

set (
	ENGINE_SOURCES
	src/lclib.c
	src/power.c
	src/ldsvguts.c
	src/engine.c
	src/transport.c
	src/market.c

	src/simulate.c
	src/shrtypes.c
	src/lintypes.c
	src/readpng.c
	src/timer.c
	src/fileutil.c
	src/shrglobs.c
	src/stats.c
    )

add_subdirectory(modules)

include_directories(${CMAKE_SOURCE_DIR}/modules)

add_definitions(-DVERSION="1.14.0")

INCLUDE (CheckIncludeFiles)

CHECK_INCLUDE_FILES (dirent.h HAVE_DIRENT_H)
if (HAVE_DIRENT_H)
	add_definitions(-DHAVE_DIRENT_H=1)
endif()
CHECK_INCLUDE_FILES (string.h HAVE_STRING_H)
if (HAVE_DIRENT_H)
	add_definitions(-DHAVE_STRING_H=1)
endif()
CHECK_INCLUDE_FILES (strings.h HAVE_STRINGS_H)
if (HAVE_DIRENT_H)
	add_definitions(-DHAVE_STRINGS_H=1)
endif()
CHECK_INCLUDE_FILES (unistd.h HAVE_UNISTD_H)
if (HAVE_DIRENT_H)
	add_definitions(-DHAVE_UNISTD_H=1)
endif()

find_program(HAVE_GZIP gzip)
if (HAVE_GZIP)
	add_definitions(-DHAVE_GZIP=1)
endif()

include(CheckFunctionExists)
check_function_exists(popen HAVE_POPEN)
if (HAVE_POPEN)
	add_definitions(-DHAVE_POPEN=1)
endif()

SET(CMAKE_C_FLAGS "--std=gnu99 -Wall -g")
SET(CMAKE_CXX_FLAGS "--std=gnu99 -Wall -g")

Find_Package(SDL REQUIRED)
Find_Package(SDL_ttf REQUIRED)

include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDLTTF_INCLUDE_DIR})

link_libraries(${SDL_LIBRARY} ${SDLTTF_LIBRARY} png z m modules)

add_executable (
		lincity
		WIN32
		MACOSX_BUNDLE
		${GUI_SOURCES} ${ENGINE_SOURCES} ${MODULE_SOURCES}
)

INSTALL(PROGRAMS lincity DESTINATION games)

INSTALL(FILES colour.pal DESTINATION share/lincity)
INSTALL(DIRECTORY help/ DESTINATION share/lincity/help)
INSTALL(DIRECTORY icons/ DESTINATION share/lincity/icons)
INSTALL(DIRECTORY messages/ DESTINATION share/lincity/messages)
INSTALL(DIRECTORY opening/ DESTINATION share/lincity/opening)

add_definitions(-DDEFAULT_LIBDIR="${CMAKE_INSTALL_PREFIX}/share/lincity")
