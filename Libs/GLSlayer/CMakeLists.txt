
add_definitions(-DGLSLAYER_EXPORTS=1)

include_directories(./Interface/)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++0x -fvisibility=hidden -Wall -Wno-unused-parameter")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions(-DUNICODE -D_UNICODE)
endif()

if(UNIX)
	file(GLOB EXCLUDED_SOURCES ./Implementation/*windows* ./Implementation/*Windows* ./Implementation/extensions/*windows* ./Implementation/extensions/*Windows*)
elseif(WIN32)
	file(GLOB EXCLUDED_SOURCES ./Implementation/*linux* ./Implementation/*Linux* ./Implementation/extensions/*linux* ./Implementation/extensions/*Linux*)
endif()

file(GLOB GLSlayer_SOURCES ./Implementation/*.cpp ./Implementation/extensions/*.cpp)
list(REMOVE_ITEM GLSlayer_SOURCES ${EXCLUDED_SOURCES})

add_library(GLSlayer SHARED ${GLSlayer_SOURCES})

if(UNIX)
	target_link_libraries(GLSlayer X11 GL)
endif()

