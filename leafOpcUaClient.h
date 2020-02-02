//#pragma once

#ifndef LEAFOPCUACLIENT_H
#define LEAFOPCUACLIENT_H

#include "uabase.h"
#include "uaclientsdk.h"

using namespace UaClientSdk;
using namespace std;

class leafOpcUaClient :  public UaSessionCallback
{
	//Classic constructor
	leafOpcUaClient();
	//Constructor using the address of the OpcUa server
	leafOpcUaClient(char *pAddress); 
	//Destructor
	~leafOpcUaClient();

public: //Variables

public: //Functions

private: //Variables

private: //Functions

};

#endif //LEAFOPCUACLIENT_H

