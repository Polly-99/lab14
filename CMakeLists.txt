cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
include(cmake/HunterGate.cmake)

HunterGate(
    URL "https://github.com/Polly-99/hunter/archive/v0.19.137.3.tar.gz"
    SHA1 17257ab891b12b911b54b67e4f8592b0d51485bd
)
project(builder)

hunter_add_package(Boost COMPONENTS program_options system filesystem)
find_package(Boost CONFIG REQUIRED program_options system filesystem)

add_executable(builder ${CMAKE_CURRENT_SOURCE_DIR}/sources/builder.cpp)
target_link_libraries(builder Boost::program_options Boost::system Boost::filesystem)

install(TARGETS builder RUNTIME DESTINATION bin)
