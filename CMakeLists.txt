cmake_minimum_required(VERSION 3.29.0)

set(CMAKE_SYSTEM_NAME Generic)

include(${CMAKE_PREFIX_PATH}/toolchain.cmake)

project(COMMON CXX)

set(include_dest "common")
set(main_lib_dest "common/lib/common")


install(FILES common-config.cmake DESTINATION ${main_lib_dest})
install(EXPORT common DESTINATION "${main_lib_dest}")

add_subdirectory(src)