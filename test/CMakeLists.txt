add_executable(steppertest steppertest.cpp)

target_link_libraries(steppertest PRIVATE Stepper pthread)
