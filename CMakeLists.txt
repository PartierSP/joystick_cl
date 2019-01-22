cmake_minimum_required (VERSION 2.8)
project (joysticks_test_app)
set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable( joystick_demo joystick_demo.cc joystick.cc )

target_link_libraries( joystick_demo )
