add_library (
	Stepper
	src/Stepper/Stepper.cpp
	src/Stepper/Stepper.hpp
)

target_include_directories(Stepper PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
