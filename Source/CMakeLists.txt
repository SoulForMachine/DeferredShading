
include_directories(../Libs/ ../Libs/GLSlayer/Interface)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++17 -Wall -Wno-unused-parameter -Wno-missing-field-initializers -Wno-unused-result -Wno-switch")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions(-DUNICODE -D_UNICODE)
endif()

if(UNIX)
	file(GLOB EXCLUDED_SOURCES *windows* *Windows*)
elseif(WIN32)
	file(GLOB EXCLUDED_SOURCES *linux* *Linux*)
endif()

file(GLOB DeferredShading_SOURCES *.cpp)
list(REMOVE_ITEM DeferredShading_SOURCES ${EXCLUDED_SOURCES})

add_executable(DeferredShading ${DeferredShading_SOURCES})

if(UNIX)
	target_link_libraries(DeferredShading GLSlayer ImGui X11 GL rt stdc++fs)
elseif(WIN32)
	target_link_libraries(DeferredShading GLSlayer ImGui)
endif()
