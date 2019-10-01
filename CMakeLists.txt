cmake_minimum_required(VERSION 3.5)
project (superCuteGrab)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

add_executable(cuteGrab 
	src/superCuteGrab.cpp
	src/fileMd5.cpp
	src/filetypes.cpp
	src/image.cpp
	src/hashDB.cpp
	src/thumbDB.cpp
	src/tag.cpp
	src/tagSet.cpp
	src/tagDB.cpp
	src/booru.cpp
	src/scoreDB.cpp
	src/cskill_instance.cpp
	src/compMan.cpp
	src/collectionMan.cpp
	src/graphics/scalingImage.cpp
	src/graphics/imageIcons.cpp
	src/graphics/window.cpp
	src/graphics/infoPopup.cpp
)

target_include_directories(cuteGrab
	PRIVATE
		src
		src/graphics
		${GTKMM_INCLUDE_DIRS}
)
add_subdirectory(lib/cskill)
target_link_libraries(cuteGrab 
	PRIVATE 
		Threads::Threads 
		cskill 
		${GTKMM_LIBRARIES}
		curl
		jsoncpp
		ssl
		crypto
		stdc++fs
)

target_compile_features(cuteGrab PUBLIC cxx_std_17)

