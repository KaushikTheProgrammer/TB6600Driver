/**
	Stepper.hpp
	Purpose: Library to be used with the TB6600 Stepper Driver and a Raspberry Pi

	@author Kaushik Prakash
	@version 1.0 03/01/19
*/

class Stepper {

public:
    Stepper(const int DIRECTION_PIN, const int PULSE_PIN, const int MICRO_STEP_SIZE);
    void relStep(const int STEPS); 
    void absStep(const int DESIRED_POSITION);
    void setAcceleration(const float ACCELERATION);
    void setMaxVelocity(const float VELOCITY);
    int getCurrentPosition();
private:
	const int _frqcy = 100;
    int _microStepSize;
    int _directionPin;
    int _pulsePin;
    int _currPosition;
    int _targetPosition;
    const float _acceleration;
    const float _initVel;
    const float _targetVel;
    const float _multiplier;
    float _currDelay;
    
    void moveForward(const int STEPS);
    void moveBackward(const int STEPS);
};
