#pragma once


typedef enum stateMachineStep_enum /*Machine State step for connecting Vrep and Automation software*/
{
	STATE_INIT, /*Initialisation state*/
	STATE_CONNECT_VREP, /*Connect to Vrep and Run the simulation, when connection is done, choose which type of communication*/
	STATE_CONNECT_OPCUA, /*Connect to the OpcUa server*/
	//STATE_CONNECT_OPCUATSN
	STATE_CONNECT_BR_PVI, /*Connect to the B&R PVI server*/
	//STATE_BROWSE_VREP <- this step could be used for Vrep services
	//STATE_BROWSE_OPCUA <- this step will be used for browsing opcua available var
	STATE_RUN_CONNECTION, /*Connection between Automation software and Vrep is properly done and running*/
	STATE_STOP_CONNECTION, /*Stop the connection between Automation software and Vrep*/
	STATE_EXIT, /*Exit the node software*/
	STATE_ERROR,	/*A error occured during the connection process*/
	STATE_TESTING_PURPOSE /*Testing State*/
};

typedef enum connectionType_enum /*Connection type between Vrep and the programming software*/
{
	TYPE_OPCUA, /*OpcUa connection*/
	TYPE_OPCUATSN, /*OpcUa over TSN connection*/
	TYPE_BR_PVI, /*PVI connection*/
	TYPE_TCPIP /*TCP/IP connection*/
};