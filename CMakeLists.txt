cmake_minimum_required(VERSION 3.18)
project(filemapbot)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
add_definitions("-std=c++20")
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan.cmake"
    EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
    TLS_VERIFY ON)
endif ()
include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_configure(REQUIRES cpr/1.6.2 threadpool/20140926 simdjson/0.9.6 spdlog/1.8.5 GENERATORS cmake_find_package)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE . BUILD missing REMOTE conan-center SETTINGS ${settings})
find_package(cpr)
find_package(spdlog)
find_package(simdjson)
find_package(ThreadPool)
file(GLOB proj_sources src/*.cpp)
add_executable(app ${proj_sources})
target_link_libraries(app PRIVATE ThreadPool::ThreadPool spdlog::spdlog cpr::cpr simdjson::simdjson)



