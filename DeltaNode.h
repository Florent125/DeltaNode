#pragma once


typedef enum stateMachineStep_enum /*Machine State step for connecting Vrep and OpcUa*/
{
	STATE_INIT, /*This is a test*/
	STATE_CONNECT_VREP,
	STATE_CONNECT_OPCUA,
	//STATE_CONNECT_OPCUATSN
	STATE_CONNECT_BR_PVI,
	//STATE_BROWSE_VREP <- this step could be used for Vrep services
	//STATE_BROWSE_OPCUA <- this step will be used for browsing opcua available var
	STATE_RUN_CONNECTION,
	STATE_STOP_CONNECTION,
	STATE_EXIT,
	STATE_ERROR	
};

typedef enum connectionType_enum /*Connection type between Vrep and the programming software*/
{
	TYPE_OPCUA, /*OpcUa connection*/
	TYPE_OPCUATSN, /*OpcUa over TSN connection*/
	TYPE_BR_PVI, /*PVI connection*/
	TYPE_TCPIP /*TCP/IP connection*/
};