cmake_minimum_required(VERSION 3.0.2)
set(PROJECT_NAME pacman-mp)
project(${PROJECT_NAME})

include_directories(SFML/include)
set(SFML_LIBS )
if (WIN32)
	link_directories(SFML/lib_win32d)
	file(GLOB SFML_LIBS
		"SFML/lib_win32d/*.dll"
		)
endif()

file(GLOB PAC_SRC_FILES
	"*.h"
	"*.cpp"
	)

add_executable(${PROJECT_NAME} ${PAC_SRC_FILES})

target_link_libraries(${PROJECT_NAME}
	sfml-system-d
	sfml-window-d
	sfml-graphics-d
	sfml-network-d
	sfml-audio-d)

foreach(lib ${SFML_LIBS})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${lib}"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endforeach(lib)
