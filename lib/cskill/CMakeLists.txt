cmake_minimum_required(VERSION 3.5)
project(cskill)

add_library(cskill STATIC 
	trueskill.cpp
	matrix.cpp
	gaussian.cpp
	basemath.cpp
	factorgraph.cpp
)

target_include_directories(cskill PUBLIC ${PROJECT_SOURCE_DIR})
