cmake_minimum_required(VERSION 2.8.12)

project(libogg)

set(SRC_FILES
  include/ogg/ogg.h
  include/ogg/os_types.h
  src/bitwise.c
  src/framing.c  
)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Dependencies)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Library/${ENGINE_BUILD_PLATFORM_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Library/${ENGINE_BUILD_PLATFORM_DIR})

target_include_directories(${PROJECT_NAME} 
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)