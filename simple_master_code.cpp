#include "WPILib.h"
#include "ADXRS450_Gyro.h"
// por favor revisa la programma para asegurar su funcion
class Robot : public SampleRobot {
	Joystick rstick; // Right joystick
	Joystick lstick; // Left joystick
	Joystick opstick; // Xbox controller
	ADXRS450_Gyro gyro; // A gyro. Duh.
	Talon rfm; // Front right motor
	Talon rrm; // Rear right motor
	Talon lfm; // Front left motor
	Talon lrm; // Re  ar left motor
	Compressor comp; // Compressor
	DoubleSolenoid loadsol; // Solenoid that drops the zubringer
	Talon loadmotor; // Loading motor
	Talon lshoot; // Left shooting motor
	Talon rshoot; // Right shooting motor
	DigitalInput DIO0;
	DigitalInput DIO1;
	DigitalInput DIO2;
	DigitalInput DIO3;
	DigitalInput DIO4;
	DigitalInput DIO5;
	DigitalInput DIO6;
	DigitalInput DIO7;
	DigitalInput DIO8;
	DigitalInput DIO9;
	DoubleSolenoid climblock;
	Talon climbmotor;
	DoubleSolenoid grabsol;


	IMAQdxSession session; //session
	Image *frame; //camera frame
	IMAQdxError imaqError; //imaq
	std::unique_ptr<AxisCamera> camera; //camera






	double kUpdatePeriod = 0.005; // update every 0.005 seconds/5 milliseconds
	double PI = 3.14159265; // I want pie right now
	bool tankdrive = true; // toggle whether robot starts in tank or snake
	bool gyroenabled = true; // toggle whether gyro is used or it is simulated
	bool cameraenabled = true; // If cameras aren't attached and this is true, it'll break
	bool forward = true; // swap if you want robot to start with other side forward
	float ltrtval = .5;
	float shootspeed = .6;
	//rfm = -1
	//rrm = -1
	//lfm = 1
	//lrm = 1
	int lfminversesafe = 1;
	int lrminversesafe = 1;
	int lfminverse = lfminversesafe;
	int lrminverse = lrminversesafe;
	int lshootinverse = 1;
	int rshootinverse = -1;
	int loadinverse = 1;
	int climbinverse = 1;


public:
	Robot() :
			rstick(0),
			lstick(1),
			opstick(2),
			gyro(SPI::Port::kOnboardCS0),
			rfm(1),
			rrm(3),
			lfm(7),
			lrm(5),
			comp(1),
			loadsol(0, 1),
			loadmotor(4),
			lshoot(2),
			rshoot(6),
			DIO0(0),
			DIO1(1),
			DIO2(2),
			DIO3(3),
			DIO4(4),
			DIO5(5),
			DIO6(6),
			DIO7(7),
			DIO8(8),
			DIO9(9),
			climblock(2,3),
			climbmotor(8),
			grabsol(4,5)
	{
	}
	void RobotInit() override {
			 if (cameraenabled) {
				// create an image
				{	CameraServer::GetInstance()->SetQuality(50);
				CameraServer::GetInstance()->StartAutomaticCapture("cam0"); }
			frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
					// open the camera at the IP address assigned. This is the IP address that the camera
					// can be accessed through the web interface.
					camera.reset(new AxisCamera("axis-camera.local"));


			 }
			}
	//----------------------------Magnitude INT--------------------//
	float ABSNUM(float input) { // An abs() function that returns floats
			if (input<0) {
				input=input*-1;
			}
			return input;
		}
	//------------------------------Jitterbot------------------------//
	void jitterbot(bool left){
			int backwardmultiplier = 1;
			if (not(forward)) {
				backwardmultiplier = -1;
			}


			if ((left)) {
				rfm.Set((.85) * backwardmultiplier * (-1)); // left
				rrm.Set((.85) * backwardmultiplier * (-1));
				lfm.Set(0);
				lrm.Set(0);
			Wait(0.25);
				lfm.Set((.85/2) * backwardmultiplier);
				lrm.Set((.85/2) * backwardmultiplier);
				rfm.Set(-(.85/2) * backwardmultiplier * (-1));
				rrm.Set(-(.85/2) * backwardmultiplier * (-1));
			Wait(0.2);
				rfm.Set(-(.85) * backwardmultiplier * (-1));
				rrm.Set(-(.85) * backwardmultiplier * (-1));
				lfm.Set(0);
				lrm.Set(0);
			Wait(0.25);
				lfm.Set(-(.85/2) * backwardmultiplier);
				lrm.Set(-(.85/2) * backwardmultiplier);
				rfm.Set((.85/2) * backwardmultiplier * (-1));
				rrm.Set((.85/2) * backwardmultiplier * (-1));
			Wait(0.2);
			}
			else {
				lfm.Set((.85) * backwardmultiplier); // right
				lrm.Set((.85) * backwardmultiplier);
				rfm.Set(0);
				rrm.Set(0);
			Wait(0.25);
				rfm.Set((.85/2) * backwardmultiplier * (-1));
				rrm.Set((.85/2) * backwardmultiplier * (-1));
				lfm.Set(-(.85/2) * backwardmultiplier);
				lrm.Set(-(.85/2) * backwardmultiplier);
			Wait(0.2);
				lfm.Set(-(.85) * backwardmultiplier);
				lrm.Set(-(.85) * backwardmultiplier);
				rfm.Set(0);
				rrm.Set(0);
			Wait(0.25);
				rfm.Set(-(.85/2) * backwardmultiplier * (-1));
				rrm.Set(-(.85/2) * backwardmultiplier * (-1));
				lfm.Set((.85/2) * backwardmultiplier);
				lrm.Set((.85/2) * backwardmultiplier);
			Wait(0.2);
			}
		}


	//---------------------------DriveControls------------------------//


	void drivercontrols() { // Convert joystick inputs to robot movement
		//-------------------------GET RIGHT STICK VALUES-----------------
		float x = rstick.GetX();
		float y = -rstick.GetY(); // Get right stick X and Y (an X and a Y... It's a boy!)
		float lval;
		float rval;
		//SmartDashboard::PutNumber("Right y",y);
		//SmartDashboard::PutNumber("Right x",x);


		//---------------------------GET BUTTONS--------------------------


		bool l1 = lstick.GetRawButton(1);
		bool l2 = lstick.GetRawButton(2);
		bool l3 = lstick.GetRawButton(3);
		bool l4 = lstick.GetRawButton(4);
		bool l5 = lstick.GetRawButton(5);
	//		bool l6 = lstick.GetRawButton(6);
	//		bool l7 = lstick.GetRawButton(7);
	//		bool l8 = lstick.GetRawButton(8);
	//		bool l9 = lstick.GetRawButton(9);
	//		bool l10 = lstick.GetRawButton(10);
	//		bool l11 = lstick.GetRawButton(11);




		bool r1 = rstick.GetRawButton(1);
		bool r2 = rstick.GetRawButton(2);
		bool r3 = rstick.GetRawButton(3);
		bool r4 = rstick.GetRawButton(4);
		bool r5 = rstick.GetRawButton(5);
	//		bool r6 = rstick.GetRawButton(6);
	//		bool r7 = rstick.GetRawButton(7);
	//		bool r10 = rstick.GetRawButton(10);
	//		bool r11 = rstick.GetRawButton(11);


		//---------------------------Toggle Direction------------------------//
		if (r2) {
				lfm.Set(0);
				lrm.Set(0);
				rfm.Set(0);
				rrm.Set(0);
				if (forward==true) {
					forward = false;
				}
				else {
					forward = true;
				}
				Wait(.5);
				}
		//--------------------------Gyro-------------------------------//
	float robotangle = 0;


				if (r3){
					gyro.Reset();
				}


				if (gyroenabled) { // Get gyro values only if gyro is enabled
					//robotangle = ((360/4.8)*(-(gyro.GetAngle())));
					robotangle = gyro.GetAngle();
				}


		//------------------------------Magnitude------------------------//
		float magnitude = 0; // borra este mas tarde: no es necesario
					if ((ABSNUM(x)>.1) or (ABSNUM(y)>.1)) {
					 magnitude = sqrt((x*x)+(y*y));
					}
					//float magnitude = .95;
				if (magnitude > .97) {
					magnitude = .97;
				}
		//----------------------------Pivot Turning------------------------//
			if (not(l1)) { // If not var turning. Also can run if trankdrive toggled
					if (l4) { // toggle 90 deg left
						lfm.Set(-.85);
						lrm.Set(-.85);
						rfm.Set(.85 * (-1));
						rrm.Set(.85 * (-1));
						Wait(.35);
					}
					if (l5) { // toggle 90 deg right
						lfm.Set(.85);
						lrm.Set(.85);
						rfm.Set(-.85 * (-1));
						rrm.Set(-.85 * (-1));
						Wait(.35);
					}
					if (l2) { // toggle 180 deg (right)
						lfm.Set(.8); // era asignado a su propio void
						lrm.Set(.8);
						rfm.Set(-.8 * (-1));
						rrm.Set(-.8 * (-1));
						Wait(.7);
					}
				}
		if (not((l1)or (l3))) { // num 2, 4, and 5 stop the program to run
			if (not(tankdrive)) {
			if (r4) { // jitter left
				jitterbot(true); // true means left
			}
			if (r5) { // jitter right
				jitterbot(false); // false means right
				}
			}


	if (tankdrive) { // override values if it's tank drive
		float lval;
		float rval;
				lval = -lstick.GetY();
				rval = -rstick.GetY();
			}
			if (r1) {
				rval = rval / 2;
				lval = lval / 2;
			}
			if (l1) { // toggle variable turning
				/*float leftx = lstick.GetX();
				lval = leftx/1.5;
				rval = -leftx/1.5;*/


				float  lx = lstick.GetX();
				float ly = rstick.GetY();


				double leftjoyangle=0;
				if ((lx>0) and (ly>0)) {
					leftjoyangle = ((180/PI)*ABSNUM((atan(lx/ly)))); // TOP RIGHT
				}
				if ((lx>0) and (ly<0)) {
					leftjoyangle = (((180/PI)*ABSNUM((atan(ly/lx))) + 90)); // BOTTOM RIGHT
				}
				if ((lx<0) and (ly<0)) {
					leftjoyangle = (((180/PI)*ABSNUM((atan(lx/ly))) + 180)); // BOTTOM LEFT
				}
				if ((lx<0) and (ly>0)) {
					leftjoyangle = (((180/PI)*ABSNUM((atan(ly/lx))) + 270)); // TOP LEFT
				}
				if ((lx==0) and (ly>0)) {
					leftjoyangle = 0; // STRAIGHT UP
				}
				if ((ly==0) and (lx>0)) {
					leftjoyangle = 90; // STRAIGHT RIGHT
				}
				if ((lx==0) and (ly<0)) {
					leftjoyangle = 180; // STRAIGHT DOWN
				}
				if ((ly==0) and (lx<0)) {
					leftjoyangle = 270; // STRAIGHT LEFT
				}
				double leftdif = robotangle - leftjoyangle;


				float leftmagnitude = 0;
				if ((ABSNUM(lx)>.1) or (ABSNUM(ly)>.1)) {
					leftmagnitude = sqrt((lx*lx)+(ly*ly));
				}
				//float magnitude = .95;
				if (leftmagnitude > .97) {
					leftmagnitude = .97;
				}
				leftmagnitude = leftmagnitude/1.5;
				if (leftdif > 5) {
					if (leftdif >= 180) {
						lval = leftmagnitude;
						rval = -leftmagnitude;
					}
					if ((leftdif < 180) and (leftdif >= 0)) {
						lval = -leftmagnitude;
						rval = leftmagnitude;
					}
					if (leftdif > (-180) and (leftdif < 0)) {
						lval = leftmagnitude;
						rval = -leftmagnitude;
					}
					if (leftdif <= (-180)) {
						lval = -leftmagnitude;
						rval = leftmagnitude;
					}
				}
			}
			if (not(forward)) { // if we are toggled backward
				rfm.Set(-lval * (-1));
				rrm.Set(-lval * (-1));
				lfm.Set(-rval);
				lrm.Set(-rval);
				//SmartDashboard::PutNumber("Left motors",-rval);
				//SmartDashboard::PutNumber("Right motors",-lval);
			}
			if (forward) {
				lfm.Set(lval);
				lrm.Set(lval);
				rfm.Set(rval * (-1));
				rrm.Set(rval * (-1));
				//SmartDashboard::PutNumber("Left motors",lval);
				//SmartDashboard::PutNumber("Right motors",rval);
			}
		}
			//anadir mas cosas aqui
	}


	void highgoal(float mval) { // Uses the shooter to shoot to the high goal


			lfm.Set(0);
			lrm.Set(0);
			rfm.Set(0);
			rrm.Set(0);
			loadmotor.Set((-.8) * loadinverse);
			lshoot.Set(-.8 * lshootinverse);
			rshoot.Set(-.8 * rshootinverse);
			Wait(.15);
			loadmotor.Set(0);
			//lshoot.Set((mval * lshootinverse));
			//rshoot.Set((mval * rshootinverse));
			lshoot.Set((1 * lshootinverse));
			rshoot.Set((1 * rshootinverse));
			Wait(3);
			loadmotor.Set(1 * loadinverse);
			Wait(1.5);
			lshoot.Set(0);
			rshoot.Set(0);
			loadmotor.Set(0);
		}
		void lowgoal() { // zubringer in reverse
			loadmotor.Set(-.8 * loadinverse);
			lshoot.Set(-.8 * lshootinverse);
			rshoot.Set(-.8 * rshootinverse);
		}
		void zubringer() { // RELEASE THE ZUBRINGER
			float currentDistance = .2; // Switch now? Need to add in - lolno


			if (currentDistance <= 0.1) {
				loadmotor.Set(0);
				lshoot.Set(0);
				rshoot.Set(0);
			}
			else {
				loadmotor.Set(.8 * loadinverse);
				lshoot.Set(-.3 * lshootinverse);
				rshoot.Set(-.3 * rshootinverse);
			}
		}
		void operatorcontrols() { // Main function for xbox controller


			//---------------------------GET BUTTONS--------------------------


			bool opa = opstick.GetRawButton(1); // A
			bool opb = opstick.GetRawButton(2); // B
			bool opx = opstick.GetRawButton(3); // X
			bool opy = opstick.GetRawButton(4); // Y
			bool oplb = opstick.GetRawButton(5); // LB
			bool oprb = opstick.GetRawButton(6); // RB
			bool opback = opstick.GetRawButton(7); // back
			bool opstart = opstick.GetRawButton(8); // start


			float lt = opstick.GetZ(); // get LT
			float rt = opstick.GetThrottle(); // get RT
			float mval = opstick.GetPOV();  // get speed




			//-------------------------------SET CONTROLS------------------------------


			int numofops = 0;


			bool shoot=false;
			if ((oplb) and (oprb)) {
				numofops += 1;
				shoot=true;
			}
			bool load=false;
			if (opx) {
				numofops += 1;
				load = true;
			}
			bool release=false;
			if (opb) {
				numofops += 1;
				release = true;
			}
			bool raise=false;
			if (opy) {
				numofops += 1;
				raise = true;
			}
			bool lower=false;
			if (opa) {
				numofops += 1;
				lower = true;
			}
			if (mval != -1) {
				if (mval == 0) {
					shootspeed = 0.9;
				}
				if (mval == 90) {
					shootspeed = 0.8;
				}
				if (mval == 180) {
					shootspeed = 0.7;
				}
				if (mval == 270) {
					shootspeed = 0.6;
				}
			}
			bool climbretract=false;
			if ((lt>ltrtval) and (rt<ltrtval)) { // if using lt
				numofops += 1;
				climbretract = true;


			}
			bool climbextend=false;
			if ((rt>ltrtval) and (lt<ltrtval)) { // if using rt
				numofops += 1;
				climbextend = true;
			}




			//---------------------------SET OPERATOR MOTORS---------------------------


			if (numofops == 0) { // if no operations
				loadmotor.Set(0);
				lshoot.Set(0);
				rshoot.Set(0);
			}
			else {
				if ((shoot) and (numofops == 1)) { // if shooting
					highgoal(shootspeed);
				}
				if ((release) and (numofops == 1)) { // if releasing
					lowgoal();
				}
				if ((load) and (numofops == 1)) { // if loading
					zubringer();
				}
				if ((raise) and (numofops == 1)) { // if raising
					loadsol.Set(DoubleSolenoid::kForward);
				}
				if ((lower) and (numofops == 1)) { // if lowering
					loadsol.Set(DoubleSolenoid::kReverse);
				}
				if ((climbextend) and (numofops == 1) and (false)) { // if climbing up
					climblock.Set(DoubleSolenoid::kForward); // might need to be kReverse
					Wait(.5);
					climbmotor.Set(rt * climbinverse);
				}
				if ((climbretract) and (numofops == 1) and (false)) {
					climblock.Set(DoubleSolenoid::kForward); // might need to be kReverse
					Wait(.5);
					climbmotor.Set((-lt) * climbinverse);
				}
				if (not((climbextend) or (climbretract))) {
					climblock.Set(DoubleSolenoid::kOff);
				}
			}
			if ((opstart) and (not(opback))) {
				grabsol.Set(DoubleSolenoid::kForward);
			}
			if ((opback) and (not(opstart))) {
				grabsol.Set(DoubleSolenoid::kReverse);
			}
		}


	void OperatorControl() {
		while (IsOperatorControl() && IsEnabled()) {


						comp.SetClosedLoopControl(true);


						climblock.Set(DoubleSolenoid::kOff);


						drivercontrols();
				//-------------------------------------CAMERA-------------------------------------------------//
						if (cameraenabled) {
							camera->GetImage(frame);
							imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
							CameraServer::GetInstance()->SetImage(frame);
						}


						operatorcontrols();


			Wait(kUpdatePeriod); // Wait 5ms for the next update.
			}
		}


	void Autonomous() { // Called during autonomous
			loadsol.Set(DoubleSolenoid::kForward); // up
			climblock.Set(DoubleSolenoid::kOff); // locked
			grabsol.Set(DoubleSolenoid::kReverse); // whatever "in" is


			bool lowbar = not(DIO0.Get());
			bool portcullis = not(DIO1.Get());
			bool cheval = not(DIO2.Get());
			bool moat = not(DIO3.Get());
			bool ramparts = not(DIO4.Get());
			bool roughterrain = not(DIO5.Get());
			bool rockwall = not(DIO6.Get());
			bool pos1 = not(DIO6.Get());
			bool pos2 = not(DIO7.Get());
			bool pos3 = not(DIO8.Get());
			bool pos4 = not(DIO9.Get());


			bool donedefence = false;
			//------------------------------ DEFENCE ----------------------
			if (lowbar) {
				lowbarfunc();
				donedefence = true;
			} else {
			if (portcullis) {
				portcullisfunc();
				donedefence = true;
			} else {
			if (cheval) {
				chevalfunc();
				donedefence = true;
			} else {
			if (moat) {
				moatfunc();
				donedefence = true;
			} else {
			if (ramparts) {
				rampartsfunc();
				donedefence = true;
			} else {
			if (roughterrain) {
				roughterrainfunc();
				donedefence = true;
			} else {
			if (rockwall) {
				rockwallfunc();
				donedefence = true;
			}
			}}}}}}
			donedefence = false;
			if (donedefence) {
				rfm.Set(.6 * (-1));
				rrm.Set(.6 * (-1));
				lfm.Set(-.6);
				lrm.Set(-.6);


				Wait(.5);
				if (lowbar) {
					rfm.Set(.4 * (-1));
					rrm.Set(.4 * (-1));
					lfm.Set(.25 * (-1));
					lrm.Set(.25 * (-1));
					Wait(2);
					rfm.Set(.3 * (-1));
					rrm.Set(.3 * (-1));
					lfm.Set(.3 * (-1));
					lrm.Set(.3 * (-1));
					Wait(.8);
					rfm.Set(.1 * (-1));
					rrm.Set(.1 * (-1));
					lfm.Set(.1 * (-1));
					lrm.Set(.1 * (-1));
					loadmotor.Set(-.6 * loadinverse);
					lshoot.Set(-.4 * lshootinverse);
					rshoot.Set(-.4 * rshootinverse);
				} else {
				if (pos1) {
					rfm.Set(.4 * (-1));
					rrm.Set(.4 * (-1));
					lfm.Set(.3 * (-1));
					lrm.Set(.3 * (-1));
					Wait(2);
					rfm.Set(.1 * (-1));
					rrm.Set(.1 * (-1));
					lfm.Set(.1 * (-1));
					lrm.Set(.1 * (-1));
					loadmotor.Set(-.6 * loadinverse);
					lshoot.Set(-.4 * lshootinverse);
					rshoot.Set(-.4 * rshootinverse);
				} else{
				if (pos2) {
					rfm.Set(.3 * (-1));
					rrm.Set(.3 * (-1));
					lfm.Set(.3 * (-1));
					lrm.Set(.3 * (-1));
					Wait(1.5);
					rfm.Set(.1 * (-1));
					rrm.Set(.1 * (-1));
					lfm.Set(.1 * (-1));
					lrm.Set(.1 * (-1));
					loadmotor.Set(-.6 * loadinverse);
					lshoot.Set(-.4 * lshootinverse);
					rshoot.Set(-.4 * rshootinverse);
				} else{
				if (pos3) {
					rfm.Set(.3 * (-1));
					rrm.Set(.3 * (-1));
					lfm.Set(.4 * (-1));
					lrm.Set(.4 * (-1));
					Wait(2);
					rfm.Set(.1 * (-1));
					rrm.Set(.1 * (-1));
					lfm.Set(.1 * (-1));
					lrm.Set(.1 * (-1));
					loadmotor.Set(-.6 * loadinverse);
					lshoot.Set(-.4 * lshootinverse);
					rshoot.Set(-.4 * rshootinverse);
				} else{
				if (pos4) {
					rfm.Set(.25 * (-1));
					rrm.Set(.25 * (-1));
					lfm.Set(.4 * (-1));
					lrm.Set(.4 * (-1));
					Wait(2);
					rfm.Set(.1 * (-1));
					rrm.Set(.1 * (-1));
					lfm.Set(.1 * (-1));
					lrm.Set(.1 * (-1));
					loadmotor.Set(-.6 * loadinverse);
					lshoot.Set(-.4 * lshootinverse);
					rshoot.Set(-.4 * rshootinverse);
				}


				}}}}
			}
		}
		void lowbarfunc() {
			loadsol.Set(DoubleSolenoid::kReverse);
			Wait(3);
			rfm.Set(.35 * (-1));
			rrm.Set(.35 * (-1));
			lfm.Set(.35);
			lrm.Set(.35);
			Wait(4);
			rfm.Set(0);
			rrm.Set(0);
			lfm.Set(0);
			lrm.Set(0);
		}
		void portcullisfunc() {
			loadsol.Set(DoubleSolenoid::kReverse);
			Wait(1);
			// move up to the portcullis
			rfm.Set(-.25 * (-1));
			rrm.Set(-.25 * (-1));
			lfm.Set(-.25);
			lrm.Set(-.25);
			Wait(3);
			// stop and raise the portcullis
			grabsol.Set(DoubleSolenoid::kForward);
			Wait(1);
			loadsol.Set(DoubleSolenoid::kForward);
			rfm.Set(-.1 * (-1));
			rrm.Set(-.1 * (-1));
			lfm.Set(-.1);
			lrm.Set(-.1);
			Wait(1.5);
			loadsol.Set(DoubleSolenoid::kReverse);
			loadmotor.Set(-.8);
			// move forard through
			rfm.Set(-.6 * (-1));
			rrm.Set(-.6 * (-1));
			lfm.Set(-.6);
			lrm.Set(-.6);
			Wait(.8);
			grabsol.Set(DoubleSolenoid::kReverse);
			Wait(1.4);
			// lower the zubringer and stop the robot
			loadsol.Set(DoubleSolenoid::kReverse);
			rfm.Set(0);
			rrm.Set(0);
			lfm.Set(0);
			lrm.Set(0);
			loadmotor.Set(0);
		}
		void chevalfunc() {
			rfm.Set(.3 * (-1));
			rrm.Set(.3 * (-1));
			lfm.Set(.3);
			lrm.Set(.3);
			Wait(1);
			rfm.Set(0);
			rrm.Set(0);
			lfm.Set(0);
			lrm.Set(0);
			loadsol.Set(DoubleSolenoid::kReverse);
			grabsol.Set(DoubleSolenoid::kReverse);
			Wait(2);
			rfm.Set(.5 * (-1));
			rrm.Set(.5 * (-1));
			lfm.Set(.5);
			lrm.Set(.5);
			loadsol.Set(DoubleSolenoid::kForward);
			grabsol.Set(DoubleSolenoid::kForward);
			Wait(1);
			rfm.Set(0);
			rrm.Set(0);
			lfm.Set(0);
			lrm.Set(0);
		}
		void moatfunc() {
			rfm.Set(.7 * (-1));
			rrm.Set(.7 * (-1));
			lfm.Set(.7);
			lrm.Set(.7);
			Wait(2);
			rfm.Set(.35 * (-1));
			rrm.Set(.35 * (-1));
			lfm.Set(.35);
			lrm.Set(.35);
			Wait(1.2);
			rfm.Set(0);
			rrm.Set(0);
			lfm.Set(0);
			lrm.Set(0);
			Wait(3);
			loadsol.Set(DoubleSolenoid::kReverse);
		}
		void rampartsfunc() {
			rfm.Set(.5 * (-1));
			rrm.Set(.5 * (-1));
			lfm.Set(.5);
			lrm.Set(.5);
			Wait(3.5);
			rfm.Set(0);
			rrm.Set(0);
			lfm.Set(0);
			lrm.Set(0);
			Wait(3);
			loadsol.Set(DoubleSolenoid::kReverse);
		}
		void roughterrainfunc() {
			rfm.Set(.4 * (-1));
			rrm.Set(.4 * (-1));
			lfm.Set(.4);
			lrm.Set(.4);
			Wait(3);
			rfm.Set(0);
			rrm.Set(0);
			lfm.Set(0);
			lrm.Set(0);
			Wait(3);
			loadsol.Set(DoubleSolenoid::kReverse);
		}
		void rockwallfunc() {
			rfm.Set(.95 * (-1));
			rrm.Set(.95 * (-1));
			lfm.Set(.95);
			lrm.Set(.95);
			Wait(1.2);
			rfm.Set(.2 * (-1));
			rrm.Set(.2 * (-1));
			lfm.Set(.2);
			lrm.Set(.2);
			Wait(2.5);
			rfm.Set(0);
			rrm.Set(0);
			lfm.Set(0);
			lrm.Set(0);
			Wait(3);
			loadsol.Set(DoubleSolenoid::kReverse);
		}


};




START_ROBOT_CLASS(Robot)




