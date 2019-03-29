/**
	Stepper.hpp
	Purpose: Library to be used with the TB6600 Stepper Driver and a Raspberry Pi

	@author Kaushik Prakash
	@version 1.0 03/01/19
*/

#include <vector>

class Stepper {

public:
    Stepper(const int DIRECTION_PIN, const int PULSE_PIN, const int MICRO_STEP_SIZE);
    void pulse(int direction, int pulseDelay);              // Take 1 step
    void relStep(const int STEPS);                          // Step a certain number of times
    void absStep(const int DESIRED_POSITION);               // Step to a certain position
    void setMaxAcceleration(const float MAX_ACCELERATION);  // Set max acceleration rev/s^2
    void setMaxVelocity(const float MAX_VELOCITY);          // Set max velocity in rev/s
    int getCurrentPosition();

private:
    std::vector<float> _allDelays;  // Contains time intervals for every step in the routine
    const float _initVel;           // Starting velocity in steps/s
    const float _maxVel;            // Max velocity in steps/s
    const float _maxAccel;          // Max acceleration in steps/s^2
    const float _multiplier;        // Multiplier [19] to calculate time interval for next step [20]
    const int _frqcy;               // Frequency of timer in ticks per second (100 for RPI 3 B)
    const int _microStepSize;       // Microstepping size in microsteps/step: 1/2, 1/4 would be 2, 4 etc.
    const int _maxSteps;            // Maximum number of steps/rev after microstepping
    const int _directionPin;        // Direction Pin on the RPI using Broadcom pin numbers
    const int _pulsePin;            // Signal Pin on the RPI using Broadcom pin numbers
    const int _propAccel;           // Proportion of stepper routine to be accelerating/decelerating
    const int _stopAccel;           // Step number at which to stop acceleration
    const int _startDecel;          // Step number at which to start deceleration
    float _currDelay;               // Delay of the next step to be taken

    void calculateParameters(const int STEPS);  // Calculate time intervals for each step
};
