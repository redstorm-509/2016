#include "WPILib.h"
#include "ADXRS450_Gyro.h"
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
	Joystick r_stick;
	Joystick l_stick;
	Joystick op_stick;
	ADXRS450_Gyro gyro;
	Talon rfm;
	Talon rrm;
	Talon lfm;
	Talon lrm;
	Talon loadmotor;
	//DoubleSolenoid loadsol;
	//DoubleSolenoid grabsol;
	//DoubleSolenoid grabsol;
	Talon lshoot;
	Talon rshoot;
	//Talon climbmotor;
	Compressor comp;
	DoubleSolenoid climblock;
	Talon climbm;


	IMAQdxSession session; //session
	Image *frame; //camera frame
	IMAQdxError imaqError; //imaq
	std::unique_ptr<AxisCamera> camera; //camera


	// update every 0.005 seconds/5 milliseconds.
	double kUpdatePeriod = 0.005;


public:
	Robot() :
			r_stick(0), // Initialize Joystick on port 0.
			l_stick(1),
			op_stick(2),
			gyro(SPI::Port::kOnboardCS0),
			rfm(1),
			rrm(3),
			lfm(7),
			lrm(5),
			loadmotor(4),
			//loadsol(0, 1),
			//grabsol(4,5),
			lshoot(2),
			rshoot(6),
			comp(1),
			climblock(2,3),
			climbm(8)
			//climbmotor(8),
			//grabsol(4,5)
	{
	}


	/**
	 * Runs the motor from the output of a Joystick.
	 */
	/*void RobotInit() override {
				// create an image
				{	CameraServer::GetInstance()->SetQuality(50);
				CameraServer::GetInstance()->StartAutomaticCapture("cam0"); }
			frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
					// open the camera at the IP address assigned. This is the IP address that the camera
					// can be accessed through the web interface.
					camera.reset(new AxisCamera("axis-camera.local"));
			 }*/


	void OperatorControl() {
		while (IsOperatorControl() && IsEnabled()) {
			comp.SetClosedLoopControl(true);
			bool climbmotor = false;
			float climbval = op_stick.GetY();


			// Set the motor controller's output.
			// This takes a number from -1 (100% speed in reverse) to +1 (100% speed forwards).
			/*{
			camera->GetImage(frame);
			imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(frame);
															}*/
			SmartDashboard::PutNumber("Gyro" , gyro.GetAngle());
			if (op_stick.GetRawButton(1)) {
				rfm.Set(.5);
				Wait(.5);
				rfm.Set(0);
			}
			if (op_stick.GetRawButton(2)) {
				rrm.Set(.5);
				Wait(.5);
				rrm.Set(0);
			}
			if 	(op_stick.GetRawButton(3)) {
				lfm.Set(.5);
				Wait(.5);
				lfm.Set(0);
			}
			if (op_stick.GetRawButton(4)) {
				lrm.Set(.5);
				Wait(.5);
				lrm.Set(0);
			}
			if (op_stick.GetRawButton(5)) {
				loadmotor.Set(.5);
				Wait(.5);
				loadmotor.Set(0);
			}
			if (op_stick.GetRawButton(7)) {
				climblock.Set(DoubleSolenoid::kForward);
				Wait(.5);
			}
			if (op_stick.GetRawButton(8)) {
				climblock.Set(DoubleSolenoid::kReverse);
				Wait(.5);
			}
			if (op_stick.GetRawButton(6)) {
				climbmotor = (true);
				}
			if (climbmotor) {
				climblock.Set(DoubleSolenoid::kReverse);
				Wait(.5);
				climbm.Set(-(climbval/2));


			}
			if (not(climbmotor)) {
				climblock.Set(DoubleSolenoid::kForward);
				Wait(.5);
			}


			Wait(kUpdatePeriod); // Wait 5ms for the next update.
		}
	}
};


START_ROBOT_CLASS(Robot)


