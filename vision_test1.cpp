#include "WPILib.h"
#include <iostream>


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
	Joystick rstick; // Right joystick
		Joystick lstick; // Left joystick
		Joystick opstick; // Xbox controller
		Talon rfm; // Front right motor
		Talon rrm; // Rear right motor
		Talon lfm; // Front left motor
		Talon lrm; // Rear left motor


		std::shared_ptr<NetworkTable> table;


		IMAQdxSession session; //session
		Image *frame; //camera frame
		IMAQdxError imaqError; //imaq
		std::unique_ptr<AxisCamera> camera; //camera


	// update every 0.005 seconds/5 milliseconds.
	double kUpdatePeriod = 0.005;


public:
	Robot() :
		rstick(0),
		lstick(1),
		opstick(2),
		rfm(1),
		rrm(3),
		lfm(7),
		lrm(5),
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


	/*	while (true) {
			std::vector<double> arr = table->GetNumberArray("centerX", llvm::ArrayRef<double>());
			for (unsigned int i=0; i < arr.size(); i++) {
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
							}
						}
					}
				}
			}
			Wait(1);
		}*/
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
								}
							}
						}
					}
				}
			}
		}


	/**
	 * Runs the motor from the output of a Joystick.
	 */
	void OperatorControl() {
		while (IsOperatorControl() && IsEnabled()) {


			camera->GetImage(frame);
			imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
			CameraServer::GetInstance()->SetImage(frame);
			SmartDashboard::PutNumber("Right y",rstick.GetY());
			rfm.Set(rstick.GetY());
			lfm.Set(rstick.GetY());
			rrm.Set(rstick.GetY());
			lrm.Set(rstick.GetY());
			CAM();
			Wait(kUpdatePeriod); // Wait 5ms for the next update.
		}
	}
};


START_ROBOT_CLASS(Robot)


