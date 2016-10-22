#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <math.h>
#include <random>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "CPositionSensor.h"
#include "CCrazyflie.h"
#include "CPositionControllerSimple.h"

#include "clockgettime.h"
#include "CLogs.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define DEFAULT_PORT "26517"  // GamesOnTrack TCP connection 

typedef std::numeric_limits< double > dbl;

int main(int argc, char **argv) {

	double sensor_XYZ[3] = { 0,0,0 };   /* meters, meters, meters, meters */
	double ctrl_ref_XYZ[4] = { 0,0,0,0 };  /* meters, meters, meters, meters */
	double ctrl_cmd_RPYT[4] = { 0,0,0,0 }; /* degrees, degrees, degress, ? */
	double dt;
	bool MissingGoT = FALSE;

	CLogs *log = new CLogs();

	double scale_ref = 0.1;

	// Initialize the quadrotor
	CCrazyRadio *crRadio = new CCrazyRadio("radio://0/80/250K"); // Address does not matter for now 
	if (!crRadio->startRadio())
	{
		std::cerr << "Could not connect to dongle. Did you plug it in?" << std::endl;
		exit(-1);
	}
	CCrazyflie *cflieCopter = new CCrazyflie(crRadio);
    std::cout << "Initializing connection. ";
	cflieCopter->setSendSetpoints(true);
	cflieCopter->setSendExtPosition(true); // if, for example, we want to make position control on the quad, instead of in this program

	// First command
	cflieCopter->setRoll(0);
	cflieCopter->setPitch(0);
	cflieCopter->setYaw(0);
	cflieCopter->setThrust(0);

	// Make sure to check the Crazyflie library for initialization settings
	// such as logging and parameters:
	// CCrazyflie::startLogging()
	// CCrazyflie::sendFlightModeParameters()
	while (!cflieCopter->isInitialized())
	{
		cflieCopter->cycle();
	}


// Quick loop to test external position data communication 
//---------------------------------------------------------
	//float fX, fY, fZ;

	//bool exit_loop_x = false;
	//while ((cflieCopter->cycle()) && (!exit_loop_x))
	//{
	//	
	//	fX = rand()/1000.0;
	//	fY = rand()/1000.0;
	//	fZ = rand()/1000.0;

	//	cflieCopter->setExtPosition(fX, fY, fZ);
	//	std::cout << fX << " " << fY << " " << fZ << std::endl;

	//	log->CF(cflieCopter);
	//	if (_kbhit())
	//	{ 
	//		exit_loop_x = true;
	//	}
	//	
	//}

	//delete log;
	//exit(0);
// -----------------------------------------------------------


	// Initialize the GoT connection 
	CPositionSensor *GoT = new CPositionSensor("localhost", DEFAULT_PORT, log);
	GoT->init();
	GoT->giveP(ctrl_ref_XYZ);  

	// Initialize the position controller 
	ctrl_ref_XYZ[2] = ctrl_ref_XYZ[2] - 0.25; // take off to x meters
	CPositionController *posController = new CPositionController(ctrl_ref_XYZ, log);

	double mylogtime = currentTime();
	bool exit_loop = false;
	int ch;
	std::cout.precision(dbl::max_digits10);
	std::cout << " \n Ok. Started! (Pressing any other keys than arrows and q/a and k/l will stop.) \n";

    // The main control loop
 	while ((cflieCopter->cycle()) && (!exit_loop))
		{
			if (GoT->readNewData() && (GoT->giveTimestamp() - GoT->giveTimestampOld() > 0.0001))
			{
				dt = GoT->giveP(sensor_XYZ);

				// Example of how to send external position data to the Crazyflie
				// cflieCopter->setExtPosition(ctrl_ref_XYZ[0]-sensor_XYZ[0], ctrl_ref_XYZ[1]-sensor_XYZ[1], ctrl_ref_XYZ[2] - sensor_XYZ[2]);

				posController->update(sensor_XYZ, dt);
				posController->giveCmd(ctrl_cmd_RPYT);

				cflieCopter->setRoll(ctrl_cmd_RPYT[0]);
				cflieCopter->setPitch(-ctrl_cmd_RPYT[1]);
				cflieCopter->setYaw(ctrl_cmd_RPYT[2]);
				cflieCopter->setThrust(ctrl_cmd_RPYT[3]);

                if (sensor_XYZ[2] < -2.0)
                {
					cflieCopter->setThrust(_PID_THRUST_OFFSET*0.95);
					std::cout << "---- HEIGHT SAFETY: " << sensor_XYZ[2] << " \n";
				}
			}

			if (currentTime() - GoT->giveTimestampOld() > 1)
			{
				cflieCopter->setRoll(0);
				cflieCopter->setPitch(0);
				cflieCopter->setYaw(0);
				cflieCopter->setThrust(_PID_THRUST_OFFSET*0.95);
				std::cout << "-------------Safety----------------" << std::endl;
			}

			// Change the control reference  from the keyboard
			if (_kbhit())
			{
				ch = _getch();
				if (ch == 224)
				{
					ch = _getch();
					switch (ch)
					{
					case KEY_UP:
						ctrl_ref_XYZ[0] = ctrl_ref_XYZ[0] + scale_ref; // x position meters
						break;
					case KEY_DOWN:
						ctrl_ref_XYZ[0] = ctrl_ref_XYZ[0] - scale_ref;
						break;
					case KEY_LEFT:
						ctrl_ref_XYZ[1] = ctrl_ref_XYZ[1] - scale_ref; // y position meters
						break;
					case KEY_RIGHT:
						ctrl_ref_XYZ[1] = ctrl_ref_XYZ[1] + scale_ref;
						break;
					default:
						exit_loop = true;
					}
				}
				else
				{
					switch (ch)
					{
					case 113: // q
						ctrl_ref_XYZ[2] = ctrl_ref_XYZ[2] - scale_ref; // z position meters
						break;
					case 97: // a 
						ctrl_ref_XYZ[2] = ctrl_ref_XYZ[2] + scale_ref;
						break;
					case 107: // k
						ctrl_ref_XYZ[3] = ctrl_ref_XYZ[3] - 5; // the yaw angle 
						break;
					case 108: // l
						ctrl_ref_XYZ[3] = ctrl_ref_XYZ[3] + 5; // the yaw angle 
						break;
					default:
						exit_loop = true;
					}
				}

				posController->setRef(ctrl_ref_XYZ);
			}

			// Logging 
			log->CF(cflieCopter);

		}
    // ----------- END MAIN CONTROL LOOP ---------------------

	// Send quadrotor stop 
	cflieCopter->setRoll(0);
	cflieCopter->setPitch(0);
	cflieCopter->setYaw(0);
	cflieCopter->setThrust(0);

	cflieCopter->cycle(); // to actually send it

	mylogtime = currentTime() - mylogtime;
	std::cout << "Run time was:" << mylogtime << " [sec].";

	delete GoT;
	delete posController;
	delete cflieCopter;
	delete crRadio;

	delete log;

	return 0;
}
