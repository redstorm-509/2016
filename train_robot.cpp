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
	Joystick lstick;
	Joystick rstick;
	Joystick opstick;
	Talon rfm;
	Talon rrm;
	Talon lfm;
	Talon lrm;
	Talon slm;
	Talon srm;
	Talon loadm;
	Compressor comp;
	DoubleSolenoid loadsol;
	DoubleSolenoid grabsol;


	// update every 0.005 seconds/5 milliseconds.
	double kUpdatePeriod = 0.005;


public:
	Robot() :
			 // Initialize Joystick on port 0.
			 // Initialize the Talon on channel 0.
			lstick(1),
			rstick(0),
			opstick(2),
			rfm(3),
			rrm(1),
			lfm(7),
			lrm(5),
			slm(2),
			srm(6),
			loadm(4),
			comp(1),
			loadsol(0,1),
			grabsol(4,5)
	{
	}






	void pnematics(){


		if (opstick.GetRawButton(4)){
			loadsol.Set(DoubleSolenoid::kForward); // up
		}else if (opstick.GetRawButton(1)){
			loadsol.Set(DoubleSolenoid::kReverse);
		}
		if (opstick.GetRawButton(8)){
			grabsol.Set(DoubleSolenoid::kForward); // up
		}else if (opstick.GetRawButton(7)){
			grabsol.Set(DoubleSolenoid::kReverse);
		}


	}


	void loadandshoot(){
	//if (opstick.GetRawButton(1) & opstick.GetThrottle()){






//		}




	}


	/*void loader (){


		if(opstick.GetY()>=.5){
			loadm.Set(.5);
		}
		else if(opstick.GetY()<=-.5){
			loadm.Set(-.5);
		}
		loadm.Set(opstick.GetY());


	}


	void shooter(){


		if(opstick.GetRawButton(1)){
			slm.Set(.4);
			srm.Set(-.4);
		}
		else {
			if(opstick.GetRawButton(2)){
				slm.Set(-.4);
				srm.Set(.4);
			}
			else {
				slm.Set(0);
				srm.Set(0);
			}
		}
	}*/


	void RobotInit() override{
			CameraServer::GetInstance()->SetQuality(75);
			//the camera name (ex "cam0") can be found through the roborio web interface
			CameraServer::GetInstance()->StartAutomaticCapture("cam0");




	}


	/**
	 * Runs the motor from the output of a Joystick.
	 */
	void OperatorControl() {


		while (IsOperatorControl() && IsEnabled()) {
			// Set the motor controller's output.
			// This takes a number from -1 (100% speed in reverse) to +1 (100% speed forwards).


			comp.SetClosedLoopControl(true);




			if (lstick.GetY() >= .95){
				lrm.Set(.95);
				lfm.Set(.95);
			}
			if (lstick.GetY() <= -.95){
				lrm.Set(-.95);
				lfm.Set(-.95);
			}
			if (rstick.GetY() >= .95){
				rrm.Set(.95);
				rrm.Set(.95);
			}
			if (rstick.GetY() <= -.95){
				rrm.Set(-.95);
				rfm.Set(-.95);
			}


			rfm.Set(rstick.GetY());
			rrm.Set(rstick.GetY());
			lfm.Set(lstick.GetY()*-1);
			lrm.Set(lstick.GetY()*-1);
			pnematics();
			//loader();
			//shooter();
			Wait(kUpdatePeriod); // Wait 5ms for the next update.
		}
	}
};


START_ROBOT_CLASS(Robot)




