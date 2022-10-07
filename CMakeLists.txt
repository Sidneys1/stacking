CMAKE_MINIMUM_REQUIRED(VERSION 3.24)

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
elseif(NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")
endif()
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

PROJECT(Stacking)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0")

# include(CheckIPOSupported)
# check_ipo_supported(RESULT supported OUTPUT error)
# if( supported )
#     message(STATUS "IPO / LTO enabled")
#     set(INTERPROCEDURAL_OPTIMIZATION TRUE)
# else()
#     message(STATUS "IPO / LTO not supported: <${error}>")
# endif()

set(INTERPROCEDURAL_OPTIMIZATION FALSE)

INCLUDE_DIRECTORIES(include)

find_package(gcem CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp util.cpp viewer.cpp kernel.cpp stb_image.cpp stb_image_resize.cpp olcPixelGameEngine.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE gcem fmt::fmt)
