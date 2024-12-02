cmake_minimum_required(VERSION 3.29.0)

set(CMAKE_SYSTEM_NAME Generic)

if (OS MATCHES "arm")
    set(CMAKE_CXX_COMPILER  C:/wintoolchain/gcc-arm-10.2-2020.11-mingw-w64-i686-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-g++.exe)

    include_directories(c:/wintoolchain/gcc-arm-10.2-2020.11-mingw-w64-i686-arm-none-linux-gnueabihf/bin/../lib/gcc/arm-none-linux-gnueabihf/10.2.1/include)
    include_directories(c:/wintoolchain/gcc-arm-10.2-2020.11-mingw-w64-i686-arm-none-linux-gnueabihf/bin/../lib/gcc/arm-none-linux-gnueabihf/10.2.1/include-fixed)
    include_directories(c:/wintoolchain/gcc-arm-10.2-2020.11-mingw-w64-i686-arm-none-linux-gnueabihf/bin/../lib/gcc/arm-none-linux-gnueabihf/10.2.1/../../../../arm-none-linux-gnueabihf/include)
    include_directories(c:/wintoolchain/gcc-arm-10.2-2020.11-mingw-w64-i686-arm-none-linux-gnueabihf/bin/../arm-none-linux-gnueabihf/libc/usr/include)
elseif(OS MATCHES "linux")
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
endif()

project(COMMON CXX)

set(tool_dest "bin")
set(include_dest "include/common")
set(main_lib_dest "lib/common")

install(FILES common-config.cmake DESTINATION ${main_lib_dest})
install(EXPORT common DESTINATION "${main_lib_dest}")

add_subdirectory(src)