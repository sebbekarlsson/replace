cmake_minimum_required(VERSION 3.20)

project(replace)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB sourcefiles ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(replace_e ${sourcefiles})

include(FetchContent)


if (NOT TARGET text_static)
  FetchContent_Declare(text_static
    URL /home/ianertson/workspace/text
    )

  FetchContent_MakeAvailable(text_static)
endif()


set(replace_deps m text_static)





#add_library(replace SHARED ${sourcefiles})
#add_library(replace_static STATIC ${sourcefiles})


#target_link_libraries(replace PUBLIC ${replace_deps})
target_link_libraries(replace_e PUBLIC ${replace_deps})
#target_link_libraries(replace_static PUBLIC ${replace_deps})




target_include_directories(
  replace_e
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


# target_include_directories(
#   replace
#     PUBLIC
#       $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#       $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
#       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#       ${CMAKE_CURRENT_BINARY_DIR}/include
#       ${CMAKE_CURRENT_SOURCE_DIR}/include)
# 
# 
# 
# target_include_directories(
#   replace_static
#     PUBLIC
#       $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#       $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
#       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#       ${CMAKE_CURRENT_BINARY_DIR}/include
#       ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS replace_e text_static)
