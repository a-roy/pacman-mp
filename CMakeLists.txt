cmake_minimum_required(VERSION 2.8.4)
project(PacMan)

set(SFML_STATIC_LIBRARIES TRUE)

add_subdirectory(SFML-2.3.2)
include_directories(SFML-2.3.2/include)
link_directories(SFML-2.3.2/lib)

file(GLOB PAC_SRC_FILES
	"*.h"
	"*.cpp"
	)

add_definitions(-std=c++11)

add_executable(PacMan ${PAC_SRC_FILES})

# sfml-system sfml-window sfml-graphics sfml-network sfml-audio
target_link_libraries(PacMan sfml-graphics)
