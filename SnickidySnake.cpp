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

	Joystick rstick; // only joystick
	Joystick lstick;
	AnalogGyro gyro;
	Talon rfm;
	Talon rrm;
	Talon lfm;
	Talon lrm;

	// update every 0.005 seconds/5 milliseconds.
	double kUpdatePeriod = 0.005;
	double PI = 3.14159265;

public:
	Robot() :
			rstick(0),
			lstick(1),
			gyro(0),
			rfm(4),
			rrm(3),
			lfm(1),
			lrm(2)
	{
	}


	float ABSNUM(float input) {
		if (input<0) {
			input=input*-1;
		}
		return input;
	}
	/**
	 * Runs the motor from the output of a Joystick.
	 */
	void OperatorControl() {
		while (IsOperatorControl() && IsEnabled()) {
			// Set the motor controller's output.
			// This takes a number from -1 (100% speed in reverse) to +1 (100% speed forwards).
			float x = rstick.GetX();
			float y = -rstick.GetY();
			SmartDashboard::PutNumber("rstick x",x);
			SmartDashboard::PutNumber("rstick y",y);

			//---------------------------GET ROBOT ANGLE------------------------------
			bool button3 = false;
			if (rstick.GetRawButton(3)){
				button3 = true;
				gyro.Reset();
			}
			bool button2 = false;
			if (rstick.GetRawButton(2)) {
				button2 = true;
				gyro.Calibrate();
			}

			float robotangle = ((360/4.8)*(-(gyro.GetAngle())));
			SmartDashboard::PutNumber("raw gyro",robotangle);
			double gyrorate = gyro.GetRate();
			SmartDashboard::PutNumber("Gryo Rate",gyrorate);
			float gyrooffset = gyro.GetOffset();
			SmartDashboard::PutNumber("Gyro Offset",gyrooffset);

			while (abs(robotangle) > 360){

				if (robotangle > 360){
					robotangle = robotangle - 360;
				}

				else if (robotangle < -360){
					robotangle = robotangle + 360;
				}
			}

			SmartDashboard::PutNumber("gyro",robotangle);
			SmartDashboard::PutBoolean("button 2",button2);
			SmartDashboard::PutBoolean("button 3",button3);
			//--------------------------GET JOYSTICK ANGLE----------------------------
			double joyangle=0;
			if ((x>0) and (y>0)) {
				joyangle = (ABSNUM((180/PI)*(atan(x/y)))); // TOP RIGHT
			}
			if ((x>0) and (y<0)) {
				joyangle = (ABSNUM(((180/PI)*(atan(y/x)) + 90))); // BOTTOM RIGHT
			}
			if ((x<0) and (y<0)) {
				joyangle = (ABSNUM(((180/PI)*(atan(x/y)) + 180))); // BOTTOM LEFT
			}
			if ((x<0) and (y>0)) {
				joyangle = (ABSNUM(((180/PI)*(atan(y/x)) + 270))); // TOP LEFT
			}
			if ((x==0) and (y>0)) {
				joyangle = (0); // STRAIGHT UP
			}
			if ((y==0) and (x>0)) {
				joyangle = 90; // STRAIGHT RIGHT
			}
			if ((x==0) and (y<0)) {
				joyangle = 180; // STRAIGHT DOWN
			}
			if ((y==0) and (x<0)) {
				joyangle = 270; // STRAIGHT LEFT
			}
			SmartDashboard::PutNumber("joyangle",joyangle);

			//----------------------------GET MAGNITUDE-----------------------
			float magnitude = 0;
			bool passingmag=false;
			if ((ABSNUM(x)>.1) or (ABSNUM(y)>.1)) {
				passingmag=true;
			 magnitude = sqrt((x*x)+(y*y));
			}
			//float magnitude = .95;
			bool overmaxmag=false;
			if (magnitude > .95) {
				overmaxmag=true;
				magnitude = .95;
			}

			SmartDashboard::PutBoolean("Passing mag check",passingmag);
			SmartDashboard::PutBoolean("Over max mag",overmaxmag);
			SmartDashboard::PutNumber("magnitude",magnitude);

			//----------------------------SET MOTORS--------------------------
			double dif = robotangle - joyangle;
			SmartDashboard::PutNumber("dif",dif);

			float lval = 0;
			float rval = 0;

			if (magnitude >= .1) {
				if (dif > 180) {
					lval = magnitude;
					rval = 0;
				}
				if (dif < 180) {
					lval = 0;
					rval = magnitude;
				}
				if (dif > (-180)) {
					lval = magnitude;
					rval = 0;
				}
				if (dif < (-180)) {
					lval = 0;
					rval = magnitude;
				}
			}

			//lfm.Set(lval);
			//lrm.Set(lval);
			//rfm.Set(-rval);
			//rrm.Set(-rval);

			SmartDashboard::PutNumber("left side",lval);
			SmartDashboard::PutNumber("right side",rval);


			Wait(kUpdatePeriod); // Wait 5ms for the next update.
		}
	}
};

START_ROBOT_CLASS(Robot)
















