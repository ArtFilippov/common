set(header_path ${COMMON_SOURCE_DIR}/include/common)
set(header ${header_path}/common.h)
set(src common.cpp)

add_library(common ${header} ${src})

target_include_directories(common PUBLIC  $<BUILD_INTERFACE:${COMMON_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${include_dest}> $<INSTALL_INTERFACE:${main_lib_dest}>)

install(TARGETS common EXPORT common DESTINATION "${main_lib_dest}")
install(FILES ${header} DESTINATION "${include_dest}")