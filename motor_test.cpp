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
	Joystick r_stick;
	Joystick l_stick;
	Joystick op_stick;
	ADXRS450_Gyro gyro;
	Talon rfm;
	Talon rrm;
	Talon lfm;
	Talon lrm;
	Talon loadmotor;
	DoubleSolenoid loadsol;
	DoubleSolenoid grabsol;
	//DoubleSolenoid climblock;
	//DoubleSolenoid grabsol;
	Talon lshoot;
	Talon rshoot;
	//Talon climbmotor;
	Compressor comp;


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
			comp(1),
			loadsol(0, 1),
			loadmotor(4),
			lshoot(2),
			rshoot(6)
			//climblock(2,3)
			//climbmotor(8),
			//grabsol(4,5)
	{
	}


	/**
	 * Runs the motor from the output of a Joystick.
	 */
	void RobotInit() override {
				// create an image
				{	CameraServer::GetInstance()->SetQuality(50);
				CameraServer::GetInstance()->StartAutomaticCapture("cam0"); }
			frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
					// open the camera at the IP address assigned. This is the IP address that the camera
					// can be accessed through the web interface.
					camera.reset(new AxisCamera("axis-camera.local"));
			 }


	void OperatorControl() {
		while (IsOperatorControl() && IsEnabled()) {
			// Set the motor controller's output.
			// This takes a number from -1 (100% speed in reverse) to +1 (100% speed forwards).
			{
			camera->GetImage(frame);
			imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(frame);
															}
			if (op_stick.GetRawButton(1)) {
				rfm.Set(.5);
				Wait(.5);
			}
			op_stick.GetRawButton(x)
			op_stick.GetRawButton(x)
			op_stick.GetRawButton(x)
			op_stick.GetRawButton(x)


			Wait(kUpdatePeriod); // Wait 5ms for the next update.
		}
	}
};


START_ROBOT_CLASS(Robot)


