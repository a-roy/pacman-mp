cmake_minimum_required(VERSION 2.8.4)
project(pacman-mp)

set(SFML_STATIC_LIBRARIES TRUE)

add_subdirectory(SFML)
include_directories(SFML/include)
link_directories(SFML/lib)

file(GLOB PAC_SRC_FILES
	"*.h"
	"*.cpp"
	)

add_definitions(-std=c++11)

add_executable(pacman-mp ${PAC_SRC_FILES})

# sfml-system sfml-window sfml-graphics sfml-network sfml-audio
target_link_libraries(pacman-mp sfml-graphics)
