# lincity
A copy of Debian unstable version of lincity, maybe with some fixes

This is my copy of Debian Sid's version of lincity (with all the patches applied) and several more fixes:

* Fixed a segfault on 64-bit machines in the beginning
* Removed the help screen invoked on left-clicking items for the first time
* Added the ability to scroll the map screen by left-dragging on the map
* Switched (not perfectly) the build system from automake to cmake
* Replaced SVGAlib dependency with libSDL. Works, but has a few bugs and performance issues. Enabled by default. Can be removed by adding `-DUSE_SDL=0` to the cmake parameters.
