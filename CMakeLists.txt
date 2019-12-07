cmake_minimum_required(VERSION 3.5.1)
project(GPSDO_TO_NEMA CXX)

### Configure Compiler ########################################################
set(CMAKE_CXX_STANDARD 11)

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_EXE_LINKER_FLAGS "-lthr ${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

find_package(UHD 3.12.0 REQUIRED)

set(BOOST_REQUIRED_COMPONENTS
    program_options
    system
    thread
)

set(BOOST_MIN_VERSION 1.58)

find_package(Boost "1.67" REQUIRED ${BOOST_REQUIRED_COMPONENTS})

# need these include and link directories for the build
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${UHD_INCLUDE_DIRS}
)

link_directories(${Boost_LIBRARY_DIRS})

### Make the executable #######################################################
add_executable(gpsdo_to_nmea main.cpp)

set(CMAKE_BUILD_TYPE "Release")
target_link_libraries(gpsdo_to_nmea ${UHD_LIBRARIES} ${Boost_LIBRARIES})
