cmake_minimum_required(VERSION 3.29.0)

set(CMAKE_SYSTEM_NAME Generic)

get_filename_component(CMAKE_PREFIX_PATH
  "${CMAKE_PREFIX_PATH}"
  REALPATH
)

include(${CMAKE_PREFIX_PATH}/toolchain.cmake)
add_compile_options(
    -Werror

    -Wall
    -Wextra
    -Wpedantic

    -Wcast-align
    -Wcast-qual
    # -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
    -Wno-missing-field-initializers
)

include_directories(${CMAKE_PREFIX_PATH})

project(COMMON CXX)

set(include_dest "common")
set(main_lib_dest "common/lib/common")


install(FILES common-config.cmake DESTINATION ${main_lib_dest})
install(EXPORT common DESTINATION "${main_lib_dest}")

add_subdirectory(src)

