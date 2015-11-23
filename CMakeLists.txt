cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
set(PROJECT_NAME pacman-mp)
project(${PROJECT_NAME})

include_directories(include)
set(EXT_LIBS )
if (CMAKE_COMPILER_IS_GNUCXX)
	link_directories(gcc64/lib)
elseif (MSVC)
	if (MSVC11)
		set(MSVC_DIR msvc11)
	elseif (MSVC12)
		set(MSVC_DIR msvc12)
	else()
		set(MSVC_DIR msvc14)
	endif()
	link_directories(${MSVC_DIR}/lib)
	file(GLOB EXT_LIBS
		"${MSVC_DIR}/bin/*.dll"
		)
endif()

file(GLOB PAC_SRC_FILES
	"*.h"
	"*.cpp"
	)

add_executable(${PROJECT_NAME} ${PAC_SRC_FILES})

if (WIN32)
target_link_libraries(${PROJECT_NAME}
	sfml-system-d
	sfml-window-d
	sfml-graphics-d
	sfml-network-d
	sfml-audio-d)
else()
target_link_libraries(${PROJECT_NAME}
	sfml-system
	sfml-window
	sfml-graphics
	sfml-network
	sfml-audio)
endif()

foreach(lib ${EXT_LIBS})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${lib}"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endforeach(lib)
