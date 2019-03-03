/**
	Stepper.hpp
	Purpose: Library to be used with the TB6600 Stepper Driver and a Raspberry Pi

	@author Kaushik Prakash
	@version 1.0 03/01/19
*/

class Stepper {

public:
    Stepper(const int DIRECTION_PIN, const int PULSE_PIN, const int MICRO_STEP_SIZE);	// Constructor
    void relStep(const int STEPS); // Relative motor stepping; No software tracking of shaft position
    void absStep(const int DESIRED_POSITION); // Absolute motor steppering with software tracking
private:
    int _microStepSize;
    int _directionPin;
    int _pulsePin;
    int _currPosition;
    void moveForward(const int STEPS);
    void moveBackward(const int STEPS);
};
