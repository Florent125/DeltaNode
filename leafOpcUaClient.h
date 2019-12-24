//#pragma once

#ifndef LEAFOPCUACLIENT_H
#define LEAFOPCUACLIENT_H

#include "uabase.h"
#include "uaclientsdk.h"

using namespace UaClientSdk;
using namespace std;

class leafOpcUaClient :  public UaSessionCallback
{
	leafOpcUaClient();
	leafOpcUaClient(char *pAddress);

};

#endif //LEAFOPCUACLIENT_H

