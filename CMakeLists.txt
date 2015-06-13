cmake_minimum_required(VERSION 2.8)
Project(lincity)

set(
	GUI_SOURCES 
	ldsvgui.c
	splash.c
	lchelp.c
	main.c
	mouse.c
	mps.c
	screen.c
	pixmap.c
	cliglobs.c
	geometry.c
	pbar.c
	module_buttons.c
	dialbox.c
   )

set (
	ENGINE_SOURCES
	lclib.c
	power.c
	ldsvguts.c
	engine.c
	transport.c
	market.c

	simulate.c
	shrtypes.c
	lintypes.c
	readpng.c
	timer.c
	fileutil.c
	shrglobs.c
	stats.c
    )

add_subdirectory(modules)

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

SET(CMAKE_C_FLAGS "--std=gnu99 -Wall -g")
SET(CMAKE_CXX_FLAGS "--std=gnu99 -Wall -g")

Find_Package(X11)
Find_Package(SDL)

#if (SDL_FOUND) 
#
#include_directories(${SDL_INCLUDE_DIR})
#
#add_definitions(-DLC_SDL=1)
#link_libraries(${SDL_LIBRARY} png z m)
#
#elseif (X11_FOUND)
#
#set (GUI_SOURCES
#	${GUI_SOURCES}
#	lcx11.c
#)
#add_definitions(-DLC_X11=1)
#link_libraries(Xext X11 png z m)
#
#else()
#	message (FATAL_ERROR "Neither SDL nor libX11 were found. Unable to compile.")
#
#endif()

if (X11_FOUND)

set (GUI_SOURCES
	${GUI_SOURCES}
	lcx11.c
)
add_definitions(-DLC_X11=1)
link_libraries(Xext X11 png z m)

else()
	message (FATAL_ERROR "libX11 not found. Unable to compile.")

endif()

link_libraries(modules)

add_executable (
		xlincity
		WIN32
		MACOSX_BUNDLE
		${GUI_SOURCES} ${ENGINE_SOURCES} ${MODULE_SOURCES}
)

INSTALL(PROGRAMS xlincity DESTINATION games)

INSTALL(FILES colour.pal DESTINATION share/lincity)
INSTALL(DIRECTORY help/ DESTINATION share/lincity/help)
INSTALL(DIRECTORY icons/ DESTINATION share/lincity/icons)
INSTALL(DIRECTORY messages/ DESTINATION share/lincity/messages)
INSTALL(DIRECTORY opening/ DESTINATION share/lincity/opening)

add_definitions(-DDEFAULT_LIBDIR="${CMAKE_INSTALL_PREFIX}/share/lincity")
