cmake_minimum_required(VERSION 3.5)
project(zed_simple_capture)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

add_definitions(-DVIDEO_MOD_AVAILABLE)
find_package(OpenCV 3 REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -O3 -w)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

execute_process(COMMAND uname -m OUTPUT_VARIABLE HOST_ARCH)
string(STRIP ${HOST_ARCH} HOST_ARCH)

set(COMPILE_ARCH "x86_64")
message("host arch: ${HOST_ARCH}; compile arch: ${COMPILE_ARCH}")

include_directories(
  /home/tanhaozhang/zed/3rd/zed_open_capture/include/zed-open-capture
  include
  ${OpenCV_INCLUDE_DIRS}
)


link_directories(
  /home/tanhaozhang/zed_bow/3rd/lib
)

add_executable(zed_capture_image
./simple_capture.cpp)

target_link_libraries(zed_capture_image
zed_open_capture 
${OpenCV_LIBS}
)