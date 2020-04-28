/******************************************************************************
** Copyright (c) 2006-2018 Unified Automation GmbH. All rights reserved.
**
** Software License Agreement ("SLA") Version 2.7
**
** Unless explicitly acquired and licensed from Licensor under another
** license, the contents of this file are subject to the Software License
** Agreement ("SLA") Version 2.7, or subsequent versions
** as allowed by the SLA, and You may not copy or use this file in either
** source code or executable form, except in compliance with the terms and
** conditions of the SLA.
**
** All software distributed under the SLA is provided strictly on an
** "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
** AND LICENSOR HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
** LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
** PURPOSE, QUIET ENJOYMENT, OR NON-INFRINGEMENT. See the SLA for specific
** language governing rights and limitations under the SLA.
**
** The complete license agreement can be found here:
** http://unifiedautomation.com/License/SLA/2.7/
**
** Project: C++ OPC Client SDK sample code
**
******************************************************************************/
#ifndef SAMPLECLIENT_H
#define SAMPLECLIENT_H

#include "uabase.h"
#include "uaclientsdk.h"
#include <iostream>
#include <fstream>

class SampleSubscription;
class Configuration;

using namespace UaClientSdk;
using namespace std;

class SampleClient : public UaSessionCallback
{
	UA_DISABLE_COPY(SampleClient);
	
public:
	SampleClient();
	virtual ~SampleClient();

	// UaSessionCallback implementation ----------------------------------------------------
	virtual void connectionStatusChanged(OpcUa_UInt32 clientConnectionId, UaClient::ServerStatus serverStatus);
	// UaSessionCallback implementation ------------------------------------------------------

	// set a configuration object we use to get connection parameters and NodeIds
	void setConfiguration(Configuration* pConfiguration);

	// OPC UA service calls
	UaStatus discover();
	UaStatus connect();
	UaStatus connectSecure();
	UaStatus disconnect();
	UaStatus browseSimple();
	UaStatus browseContinuationPoint();
	UaStatus read();
	UaStatus write();
	UaStatus writeRegistered();
	UaStatus subscribe();
	UaStatus unsubscribe();
	UaStatus registerNodes();
	UaStatus unregisterNodes();
	//My service call
	 /*Write values cyclically to the configured nodes*/
	UaStatus writeCyclicValues(const UaVariantArray& valuesToWrite);
	UaStatus writeInternalCyclicValues(const UaNodeIdArray& nodesToWrite, const UaVariantArray& valuesToWrite);
	/*Browse the nodes and return the references (for now it is only displayed)*/
	UaStatus browseAndReturnReferences(const UaNodeId& nodeToBrowse, OpcUa_UInt32 maxReferencesToReturn, UaReferenceDescriptions *initArrayReference);
	UaStatus browseFromRoot(); //Test
	UaStatus browseInternal(const UaNodeId& nodeToBrowse, OpcUa_UInt32 maxReferencesToReturn);

	//Get the float values
	vector<float> getOpcUaFloat();

	//Get the boolean values
	vector<bool> getOpcUaBool();

	//UaNodeAray testing
	UaNodeIdArray testingArrayOpcUa;
	int numberOfNode = 0;

private:
	// helper methods
	UaStatus connectInternal(const UaString& serverUrl, SessionSecurityInfo& sessionSecurityInfo);
	UaStatus writeInternal(const UaNodeIdArray& nodesToWrite, const UaVariantArray& valuesToWrite);
	UaStatus findSecureEndpoint(SessionSecurityInfo& sessionSecurityInfo);
	UaStatus checkServerCertificateTrust(SessionSecurityInfo& sessionSecurityInfo);
	void printBrowseResults(const UaReferenceDescriptions& referenceDescriptions);
	/*write a txt file with the different nodes*/
	void writeTxtFileFromBrowseResult(const UaNodeId& nodeToBrowse, int depthLevel, string browseName);
	/*write a JSON file with the different nodes*/
	void writeJSONFileFromBrowseResult(const UaNodeId& nodeToBrowse, int depthLevel, string browseName);
	void printCertificateData(const UaByteString& serverCertificate);
	int userAcceptCertificate();

private:
	UaSession* m_pSession;
	Configuration* m_pConfiguration;
	UaClient::ServerStatus  m_serverStatus;
	UaNodeIdArray           m_registeredNodes;
	SampleSubscription* m_pSampleSubscription;
	ofstream m_resultOpcUabrowsingTxt;
	ofstream m_resultOpcUabrowsingJSON;
};


#endif // SAMPLECLIENT_H

