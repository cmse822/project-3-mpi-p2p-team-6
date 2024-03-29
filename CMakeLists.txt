cmake_minimum_required(VERSION 3.0)

project(project3_team6)

find_package(MPI REQUIRED)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Activate compiler optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

### Blocking ping pong section ###
add_executable(ping_pong src/ping_pong/ping_pong.cpp)

target_link_libraries(ping_pong
  PRIVATE
  MPI::MPI_CXX)

target_include_directories(
    ping_pong
    PRIVATE
    include
    ${MPI_C_INCLUDE_PATH})

### Non-blocking ping pong section ###
add_executable(ping_pong_non_blocking src/ping_pong/ping_pong_non_blocking.cpp)

target_link_libraries(ping_pong_non_blocking
  PRIVATE
  MPI::MPI_CXX)

target_include_directories(
  ping_pong_non_blocking
  PRIVATE
  include
  ${MPI_C_INCLUDE_PATH})

add_executable(ring_shift src/ring_shift/ring_shift.cpp)

target_link_libraries(ring_shift
  PRIVATE
  MPI::MPI_CXX)

target_include_directories(
    ring_shift
    PRIVATE
    include
    ${MPI_C_INCLUDE_PATH}) 
    
add_executable(ring_shift_non_blocking src/ring_shift/ring_shift_non_blocking.cpp)

target_link_libraries(ring_shift_non_blocking
  PRIVATE
  MPI::MPI_CXX)

target_include_directories(
    ring_shift_non_blocking
    PRIVATE
    include
    ${MPI_C_INCLUDE_PATH})          

# Install the executable in the current bin directory, not usr bin
install(TARGETS 
        ping_pong
        ping_pong_non_blocking 
        ring_shift
        ring_shift_non_blocking 
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)