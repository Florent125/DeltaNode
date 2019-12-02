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
	sConfigFile += "/mySampleconfig.ini";

	//Create configuration object and load configuration
	Configuration* pMyConfiguration = new Configuration();

	//Variables to write to the server
	UaVariantArray opcUaArrayVariableToWrite;
	UaVariant variableToWrite;
	opcUaArrayVariableToWrite.create(1); //Pr le moment on r�cup�re que la valeur du laser

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

	int lbrJointRedundant1 = 0;

	vector<float> opcUaFloat;
	vector<bool> opcUaBool;
	bool suctionTool = FALSE;
	int diDetectedObject = 0;
	int counter = 0;

	stateMachineStep_enum step = STATE_INIT;
	connectionType_enum connectionType = TYPE_OPCUA;
	bool exit = false;
	bool WORK = true;
	bool testingOnce = FALSE;
	simxInt executeChildScript;
	simxInt executeMainScript;

	while (!exit) {

		switch (step)
		{
			case STATE_INIT:
				cout << "Init step" << endl;
				step = STATE_CONNECT_VREP;

				//Testing OpcUa
				//step = STATE_CONNECT_OPCUA;
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
					simxInt syncho = simxSynchronous(clientID, 0);
					int start = simxStartSimulation(clientID, simx_opmode_oneshot);
					

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
					//step = STATE_TESTING_PURPOSE;

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
					// Create subscription
					status = pMyClient->subscribe();

					//OpcUa connection is done, go to STATE_RUN_CONNECTION
					step = STATE_RUN_CONNECTION;
				}

				//step = STATE_TESTING_PURPOSE;

			break;


			case STATE_CONNECT_BR_PVI:
				cout << "Connect PVI step" << endl;

				break;

			case STATE_RUN_CONNECTION:
				if (status.isGood())
				{
					opcUaFloat = pMyClient->getOpcUaFloat();
					opcUaBool = pMyClient->getOpcUaBool();
					
					/*simxSetJointPosition(clientID, lbrJoint1, opcUaFloat[0] * (PI / 180), simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint2, opcUaFloat[1] * (PI / 180), simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint3, opcUaFloat[2] * (PI / 180), simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint4, opcUaFloat[3] * (PI / 180), simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint5, opcUaFloat[4] * (PI / 180), simx_opmode_oneshot_wait);
					simxSetJointPosition(clientID, lbrJoint6, opcUaFloat[5] * (PI / 180), simx_opmode_oneshot_wait);*/
					
					simxSetJointPosition(clientID, lbrJoint1, opcUaFloat[0] * (PI / 180), simx_opmode_streaming + 5);
					simxSetJointPosition(clientID, lbrJoint2, opcUaFloat[1] * (PI / 180), simx_opmode_streaming + 5);
					simxSetJointPosition(clientID, lbrJoint3, opcUaFloat[2] * (PI / 180), simx_opmode_streaming + 5);
					simxSetJointPosition(clientID, lbrJoint4, opcUaFloat[3] * (PI / 180), simx_opmode_streaming + 5);
					simxSetJointPosition(clientID, lbrJoint5, opcUaFloat[4] * (PI / 180), simx_opmode_streaming + 5);
					simxSetJointPosition(clientID, lbrJoint6, opcUaFloat[5] * (PI / 180), simx_opmode_streaming + 5);

					simxSetIntegerSignal(clientID, "BaxterVacuumCup_active", opcUaBool[6], simx_opmode_oneshot_wait); //Vacuum value

					//R�cup�rer la valeur du laser
					simxGetIntegerSignal(clientID, "conveyorObjectDetected", &diDetectedObject, simx_opmode_oneshot_wait);

					//Ecrire la valeur du laser par com opcUa
					variableToWrite.setBool((bool)diDetectedObject);
					variableToWrite.copyTo(&opcUaArrayVariableToWrite[0]);
					pMyClient->writeCyclicValues(opcUaArrayVariableToWrite);

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

			case STATE_TESTING_PURPOSE:
				
				// Wait for user command.
				printf("\nPress Enter to do a simple browse\n");
				getchar();
				// Simple Browse
				status = pMyClient->browseSimple();

				printf("\nPress Enter to browse with continuation point\n");
				getchar();
				// Browse with continuation point
				status = pMyClient->browseContinuationPoint();
				
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
