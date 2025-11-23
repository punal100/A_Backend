cmake_minimum_required(VERSION 3.15)
project(A_Backend_Server VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Crow CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.hpp")

add_executable(A_Backend_Server ${SOURCES})

# Include directories
target_include_directories(A_Backend_Server PRIVATE src)

# Link libraries
target_link_libraries(A_Backend_Server PRIVATE Crow::Crow libpqxx::pqxx)
