add_library (
	Stepper
	src/Stepper/Stepper.cpp
	src/Stepper/Stepper.hpp
)

find_library(wiringPi_LIB wiringPi)
target_link_libraries(Stepper ${wiringPi_LIB})

target_include_directories(Stepper PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
