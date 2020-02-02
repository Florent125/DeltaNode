#pragma once


typedef enum stateMachineStep_enum /*Machine State step for connecting Vrep and Automation software*/
{
	STATE_INIT, /*Initialisation state*/
	STATE_CONNECT_COPPELIASIM, /*Connect to Vrep and Run the simulation, when connection is done, choose which type of communication*/
	STATE_CONNECT_OPCUA, /*Connect to the OpcUa server*/
	//STATE_CONNECT_OPCUATSN
	STATE_CONNECT_BR_PVI, /*Connect to the B&R PVI server*/
	//STATE_CONNECT_TCPIP, /*Connect to the TcpIp server*/
	//STATE_BROWSE_COPPELIASIM, /*Browse available joints from coppeliasim scene*/
	STATE_BROWSE_OPCUA, /*Browse available variables from the opcua server*/
	//STATE_BROWSE_OPCUATSN, /*Browse available variables from the opcua over TSN server*/
	//STATE_CONFIGURE_CONNECTION, /*After browsing the variables (CPU and CoppeliaSim) we can configure the connection*/
	STATE_RUN_CONNECTION, /*Connection between Automation software and Vrep is properly done and running*/
	STATE_STOP_CONNECTION, /*Stop the connection between Automation software and Vrep*/
	//STATE_SAVE_SOFTWARE, /*Save the project*/
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