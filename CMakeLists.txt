cmake_minimum_required (VERSION 2.8)
project (joysticks_test_app)
set(CMAKE_CXX_FLAGS "-std=c++11")

# Add Dependence on Boost
find_package(Boost COMPONENTS date_time system REQUIRED)
find_package(Threads REQUIRED)

# Demo of Read Data from Joysticks
add_executable( joystick_demo joystick_demo.cc joystick.cc )

# Demo of Serial Communication
add_executable(serial_communication_demo serial_communication_demo.cc serialstream.cpp )


# Linking
target_link_libraries( joystick_demo )

target_link_libraries(serial_communication_demo ${Boost_LIBRARIES}  ${CMAKE_THREAD_LIBS_INIT})
