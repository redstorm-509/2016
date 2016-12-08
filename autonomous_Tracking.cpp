#include "WPILib.h"
#include "ADXRS450_Gyro.h"
#include <math.h>


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




	ADXRS450_Gyro gyro; // A gyro. Duh.
	Talon rfm; // Front right motor
	Talon rrm; // Rear right motor
	Talon lfm; // Front left motor
	Talon lrm; // Re  ar left motor
	//Compressor comp; // Compressor
	//DoubleSolenoid loadsol; // Solenoid that drops the zubringer
	Talon loadmotor; // Loading motor
	Talon lshoot; // Left shooting motor
	Talon rshoot; // Right shooting motor




	std::shared_ptr<NetworkTable> table;




	IMAQdxSession session; //session
	Image *frame; //camera frame
	IMAQdxError imaqError; //imaq
	std::unique_ptr<AxisCamera> camera; //camera




	// update every 0.005 seconds/5 milliseconds.
	double kUpdatePeriod = 0.005;
	double cx = 0;
	double cy = 0;
	double speedconstant = 4; // how many feet the robot moves in 1 second at .7


public:
	Robot() :
					gyro(SPI::Port::kOnboardCS0),
					rfm(1),
					rrm(3),
					lfm(7),
					lrm(5),
					//comp(1),
					//loadsol(0, 1),
					loadmotor(4),
					lshoot(2),
					rshoot(6),
					table(NetworkTable::GetTable("GRIP/TestCameraReport"))
	{
	}


	void RobotInit() override {
		{	CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0"); }
		// create an image
		frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
		// open the camera at the IP address assigned. This is the IP address that the camera
		// can be accessed through the web interface.
		camera.reset(new AxisCamera("axis-camera.local"));
	}
	float ABSNUM(float input) { // An abs() function that returns floats
		if (input<0) {
			input=input*-1;
		}
		return input;
	}
	void CAM() {
		SmartDashboard::PutBoolean("Running",true);
		std::vector<double> arr = table->GetNumberArray("centerX", llvm::ArrayRef<double>());
		SmartDashboard::PutNumber("ArrSize",arr.size());
		for (unsigned int i=0; i < arr.size(); i++) {
			SmartDashboard::PutBoolean("Running2",true);
			if (i == 0) {
				SmartDashboard::PutNumber("CenterX0",arr[i]);
			}
				else {
				if (i == 1) {
					SmartDashboard::PutNumber("CenterX1",arr[i]);
				}
				else {
					if (i == 2) {
						SmartDashboard::PutNumber("CenterX2",arr[i]);
					}
					else {
						if (i == 3) {
							SmartDashboard::PutNumber("CenterX3",arr[i]);
						}
						else {
							if (i == 4) {
								SmartDashboard::PutNumber("CenterX4",arr[i]);
							}
							else {
								SmartDashboard::PutNumber("CenterXOTHER",arr[i]);
		}}}}}}}
	//---------------------------GET ROBOT ANGLE------------------------------
	float getrobotangle() {
		float robotangle = 0;


		robotangle = ((360/4.8)*(-(gyro.GetAngle())));
		//robotangle = gyro.GetAngle();


		while ((robotangle >= 360) or (robotangle < 0)){


			if (robotangle >= 360){
				robotangle = robotangle - 360;
			}




			else if (robotangle < 0){
				robotangle = robotangle + 360;
			}
		}
		return robotangle;
	}


	void SETMOTORS(float l,float r) {


		if ((l+r) != 0) {
			double velocity = speedconstant * kUpdatePeriod;


			cx -= (sin(getrobotangle()) * velocity);
			cy += (sin(getrobotangle()) * velocity);
		}


		lfm.Set(l);
		lrm.Set(l);
		rfm.Set(r);
		rrm.Set(r);
	}


	void GOTOPOINT(float tx, float ty) {
		while ((sqrt(((cx-tx)*(cx-tx))+((cy-ty)*(cy-ty)))) > .5) { // while distance > .5 ft
			//float D = sqrt(((cx-tx)*(cx-tx))+((cy-ty)*(cy-ty)));
			float tO = atan((cy-ty)/(cx-tx)); // target angle
			float cO = getrobotangle(); // current angle
			float rmotors = 0;
			float lmotors = 0;


			if (ABSNUM(tO-cO) > 4) { // only point turn if the angle is off by > 4 degrees
				while (ABSNUM(tO-cO) > 1) { // if point turning, then do it until it's good
					if (((tO-cO) >= 0) && (tO-cO) < 180) {
						rmotors = -.7; // turn right
						lmotors = .7;
					}
					else {
						if (((tO-cO) >= 180) && (tO-cO) < 360) {
							rmotors = .7; // turn left
							lmotors = -.7;
						}
						else {
							if (((tO-cO) < 0) && (tO-cO) >= -180) {
								rmotors = .7; // turn left
								lmotors = -.7;
							}
							else {
								if (((tO-cO) < -180) && (tO-cO) >= -360) {
									rmotors = -.7; // turn right
									lmotors = .7;
					}}}}
					SETMOTORS(lmotors,rmotors); // keep updating stuff in this loop
					CAM();
					Wait(kUpdatePeriod);
				}
			}
			else { // if less than 4 degrees off
				if ((tO-cO) < 1) { // if less than 1 degrees off
					lmotors = .7;
					rmotors = .7;
				}
				else { // if between 1 and 4 (inclusive) degrees
					if ((tO-cO) < 0) { // negative angle, swerve left
						lmotors = .6;
						rmotors = .8;
					}
					else { // positive angle, swerve right
						lmotors = .8;
						rmotors = .6;
					}
				}
				SETMOTORS(lmotors,rmotors); // update until it reaches the point
				CAM();
				Wait(kUpdatePeriod);
			}
		}
	}


	void OperatorControl() {
		while (IsOperatorControl() && IsEnabled()) {




			Wait(kUpdatePeriod); // Wait 5ms for the next update.
		}
	}


	void Autonomous() {
		gyro.Reset();
		while (IsAutonomous() && IsEnabled()) {
			/*camera->GetImage(frame);
			imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(frame);*/


			Wait(kUpdatePeriod);
		}


	}
};


START_ROBOT_CLASS(Robot)


