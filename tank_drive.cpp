#include "WPILib.h"


/**
 * This sample program shows how to control a motor using a joystick. In the operator
 * control part of the program, the joystick is read and the value is written to the motor.
 *
 * Joystick analog values range from -1 to 1 and speed controller inputs as range from
 * -1 to 1 making it easy to work together. The program also delays a short time in the loop
 * to allow other threads to run. This is generally a good idea, especially since the joystick
 * values are only transmitted from the Driver Station once every 20ms.
 */
class Robot : public SampleRobot {
	Joystick rstick;
	Joystick lstick;


	// The motor to control with the Joystick.
	// This uses a Talon speed controller; use the Victor or Jaguar classes for
	//   other speed controllers.
	Talon rmotor;
	Talon lmotor;
	Talon rfmotor;
	Talon lfmotor;


	// update every 0.005 seconds/5 milliseconds.
	double kUpdatePeriod = 0.005;


public:
	Robot() :
			rstick(0), // Initialize Joystick on port 0.
			lstick(1), // Initialize the Talon on channel 0.
			rmotor(1),
			lmotor(5),
			rfmotor(3),
			lfmotor(7)
	{
	}


	/**
	 * Runs the motor from the output of a Joystick.
	 */
	void OperatorControl() {
		while (IsOperatorControl() && IsEnabled()) {
			// Set the motor controller's output.
			// This takes a number from -1 (100% speed in reverse) to +1 (100% speed forwards).
			float lval = -lstick.GetY();
			float rval = rstick.GetY();


			if (not(rstick.GetRawButton(1))) {
				lval = lval/2;
				rval = rval/2;
			}


			rmotor.Set(rval);
			rfmotor.Set(rval);
			lmotor.Set(lval);
			lfmotor.Set(lval);


			Wait(kUpdatePeriod); // Wait 5ms for the next update.
		}
	}
};


START_ROBOT_CLASS(Robot)


