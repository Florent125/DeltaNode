#include "uaplatformlayer.h"
#include "sampleclient.h"
#include "shutdown.h"
#include "configuration.h"
#include "DeltaNode.h"
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>



extern "C" {
#include "extApi.h"
}

using namespace std;
#define PI 3.14

#ifdef _WIN32_WCE
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
#else
int main(int, char* [])
#endif
{
	//************************//
	//**Init OpcUa variables**//
	//************************//
	UaStatus status;

	//Initialize the UA Stack platform layer
	UaPlatformLayer::init();

	//Create instance of the client
	SampleClient* pMyClient = new SampleClient();

	//Get current path to build configuration file name
	UaString sConfigFile(getAppPath());
	sConfigFile += "/sampleconfig.ini";

	//Create configuration object and load configuration
	Configuration* pMyConfiguration = new Configuration();

	//***********************//
	//**Init VREP variables**//
	//***********************//
	int clientID = 0;
	bool VERBOSE = true;
	int lbrJoint1 = 0;
	int lbrJoint2 = 0;
	int lbrJoint3 = 0;
	int lbrJoint4 = 0;
	int lbrJoint5 = 0;
	int lbrJoint6 = 0;

	float* BR_joints;
	int counter = 0;

	//**********************//
	//**Init PVI variables**//
	//**********************//

	stateMachineStep_enum step = STATE_INIT;
	connectionType_enum connectionType = TYPE_OPCUA;
	bool exit = false;
	bool WORK = true;

	while (!exit) {
		switch (step)
		{
			case STATE_INIT:
				cout << "Init step" << endl;
				step = STATE_CONNECT_VREP;
			break;

			case STATE_CONNECT_VREP:
				cout << "Connect VREP step" << endl;
				//! Todo Naresh: check to run this in parallel with real robot driver. May need to integrate my planner
				simxFinish(-1);                                                     //! Close any previously unfinished business
				clientID = simxStart((simxChar*)"127.0.0.1", 19997, true, true, 5000, 5);  //!< Main connection to V-REP
				Sleep(1000);
				if (clientID != -1)
				{
					cout << "Connection status to VREP: SUCCESS" << endl;
					simxInt syncho = simxSynchronous(clientID, 1);
					int start = simxStartSimulation(clientID, simx_opmode_oneshot_wait);

					simxGetObjectHandle(clientID, "IRB140_joint1", &lbrJoint1, simx_opmode_oneshot_wait);
					simxGetObjectHandle(clientID, "IRB140_joint2", &lbrJoint2, simx_opmode_oneshot_wait);
					simxGetObjectHandle(clientID, "IRB140_joint3", &lbrJoint3, simx_opmode_oneshot_wait);
					simxGetObjectHandle(clientID, "IRB140_joint4", &lbrJoint4, simx_opmode_oneshot_wait);
					simxGetObjectHandle(clientID, "IRB140_joint5", &lbrJoint5, simx_opmode_oneshot_wait);
					simxGetObjectHandle(clientID, "IRB140_joint6", &lbrJoint6, simx_opmode_oneshot_wait);


					//simxPauseCommunication(clientID,true);
					simxSetJointPosition(clientID, lbrJoint1, 0.0, simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint2, 0.0, simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint3, 0.0, simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint4, 0.0, simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint5, 0.0, simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint6, 0.0, simx_opmode_oneshot_wait);

					switch (connectionType)
					{
					case TYPE_OPCUA:
						cout << "OpcUa connection chosen" << endl;
						step = STATE_CONNECT_OPCUA;
						break;
					}

				}


			break;

			case STATE_CONNECT_OPCUA:
				cout << "Connect OpcUa step" << endl;
				status = pMyConfiguration->loadConfiguration(sConfigFile);

				//Loading Configuration succeeded
				if (status.isGood())
				{
					//Set configuration
					pMyClient->setConfiguration(pMyConfiguration);
					//Connect to OpcUa Server
					status = pMyClient->connect();

				}
				else {
					delete pMyConfiguration;
					pMyConfiguration = NULL;
				}

				if (status.isGood())
				{
					// register nodes and do a write with the registered nodeIds
					pMyClient->registerNodes();
					pMyClient->writeRegistered();

					// Create subscription
					status = pMyClient->subscribe();

					//OpcUa connection is done, go to STATE_RUN_CONNECTION
					step = STATE_RUN_CONNECTION;
				}

			break;


			case STATE_CONNECT_BR_PVI:
				cout << "Connect PVI step" << endl;

				break;

			case STATE_RUN_CONNECTION:
				if (status.isGood())
				{
					BR_joints = pMyClient->getJoints();
					simxSetJointPosition(clientID, lbrJoint1, *BR_joints* (PI / 180), simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint2, *(BR_joints + 1)* (PI / 180), simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint3, *(BR_joints + 2)* (PI / 180), simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint4, *(BR_joints + 3)* (PI / 180), simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint5, *(BR_joints + 4)* (PI / 180), simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint6, *(BR_joints + 5)* (PI / 180), simx_opmode_oneshot_wait);
				}
				else {
					step = STATE_ERROR;
					
				}

				break;


			case STATE_STOP_CONNECTION:
				cout << "Connection stopped" << endl;

				break;

			case STATE_EXIT:
				cout << "Exit step" << endl;
				exit = true;
				break;

			case STATE_ERROR:
				cout << "An error occured" << endl;
				break;

		}
	}

	/*****************************************************/
	/****************CLOSE OPCUA AND V-REP****************/
	/*****************************************************/

	// Delete subscription
	status = pMyClient->unsubscribe();

	// Close application
	printf("\nPress Enter to close\n");
	// Wait for user command.
	getchar();

	delete pMyClient;
	pMyClient = NULL;

	// Cleanup the UA Stack platform layer
	UaPlatformLayer::cleanup();

	simxFinish(clientID);
	//return clientID;
}
