cmake_minimum_required(VERSION 2.8)
Project(lincity_modules)

include_directories(${CMAKE_SOURCE_DIR})

set (
	MODULE_SOURCES
	organic_farm.c
	residence.c
	light_industry.c
	heavy_industry.c
	coalmine.c
	oremine.c
	commune.c
	port.c
	parkland.c
	university.c
	recycle.c
	health_centre.c
	rocket_pad.c
	monument.c
	school.c
	blacksmith.c
	mill.c
	pottery.c
	firestation.c
	cricket.c
	fire.c
	shanty.c
	tip.c
	substation.c
	windmill.c
	solar_power.c
	coal_power.c
	power_line.c
	track.c
	rail.c
	road.c
	water.c
)

add_library (
		modules STATIC
		${MODULE_SOURCES}
)
