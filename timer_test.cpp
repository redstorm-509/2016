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
	Joystick m_stick;
	Timer tim;


	// The motor to control with the Joystick.
	// This uses a Talon speed controller; use the Victor or Jaguar classes for
	//   other speed controllers.


	// update every 0.005 seconds/5 milliseconds.
	double kUpdatePeriod = 0.005;


public:
	Robot() :
			m_stick(0) // Initialize Joystick on port 0.
	{
	}


	/**
	 * Runs the motor from the output of a Joystick.
	 */
	void OperatorControl() {
		while (IsOperatorControl() && IsEnabled()) {
			// Set the motor controller's output.
			// This takes a number from -1 (100% speed in reverse) to +1 (100% speed forwards).
			double currenttime = tim.Get();
			if (m_stick.GetRawButton(1)) {
				tim.Reset();
			}
			if (m_stick.GetRawButton(2)) {
				tim.Stop();
			}
			if (m_stick.GetRawButton(3)) {
				tim.Start();
			}
			SmartDashboard::PutNumber("timer",currenttime);


			if (currenttime >= 149.5) {
				SmartDashboard::PutBoolean("Locked",true);
			}
			if (currenttime < 149.5) {
				SmartDashboard::PutBoolean("Locked",false);
			}


			Wait(kUpdatePeriod); // Wait 5ms for the next update.
		}
	}
	void Autonomous() {
		tim.Start();
		tim.Reset();
	}
};


START_ROBOT_CLASS(Robot)


