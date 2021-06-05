cmake_minimum_required(VERSION 3.5)

configure_file(uvd.pc.in uvd.pc @ONLY)

project(uvd VERSION 1.0.0 DESCRIPTION "A simple library to detect version information of unix operating systems." LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_library(uvd SHARED src/libuvd.h src/libuvd.c)

set_target_properties(uvd PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(uvd PROPERTIES SOVERSION 1)
set_target_properties(uvd PROPERTIES PUBLIC_HEADER src/libuvd.h)


include(GNUInstallDirs)

install(TARGETS uvd LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/uvd.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
