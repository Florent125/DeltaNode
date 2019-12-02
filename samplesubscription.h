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
#ifndef SAMPLESUBSCRIPTION_H
#define SAMPLESUBSCRIPTION_H

#include "uabase.h"
#include "uaclientsdk.h"

using namespace UaClientSdk;
using namespace std;

typedef enum OpcUaDataType_enum { //The datatypes of the variables from OpcUa server
	TYPE_BOOLEAN = 1,
	TYPE_SBYTE, TYPE_BYTE,
	TYPE_INT16, TYPE_UINT16,
	TYPE_INT32, TYPE_UINT32,
	TYPE_INT64, TYPE_UINT64,
	TYPE_FLOAT,
	TYPE_DOUBLE,
	TYPE_STRING,
	TYPE_DATETIME,
	TYPE_GUID,
	TYPE_BYTE_STRING,
	TYPE_XML_ELEMENT,
	TYPE_NODEID,
	TYPE_EXPANDED_NODEDID,
	TYPE_STATUS_CODE,
	TYPE_QUALIFIED_NAME,
	TYPE_LOCALIZED_TEXT,
	TYPE_STRUCTURE,
	TYPE_DATA_VALUE,
	TYPE_BASE_DATATYPE,
	TYPE_DIAGNOSTIC_INFO,
	TYPE_NUMBER,
	TYPE_INTEGER,
	TYPE_UINTEGER,
	TYPE_ENUMERATION,
	TYPE_IMAGE
};

class Configuration;

class SampleSubscription : public UaSubscriptionCallback
{
	UA_DISABLE_COPY(SampleSubscription);

public:
	OpcUaDataType_enum OpcUaDataType;
	vector<float> opcUaFloat;
	vector<bool> opcUaBool;
	bool suctionTool;

public:
	SampleSubscription(Configuration* pConfiguration);
	virtual ~SampleSubscription();

	// UaSubscriptionCallback implementation ----------------------------------------------------
	virtual void subscriptionStatusChanged(
		OpcUa_UInt32      clientSubscriptionHandle,
		const UaStatus& status);
	virtual void dataChange(
		OpcUa_UInt32               clientSubscriptionHandle,
		const UaDataNotifications& dataNotifications,
		const UaDiagnosticInfos& diagnosticInfos);
	virtual void newEvents(
		OpcUa_UInt32                clientSubscriptionHandle,
		UaEventFieldLists& eventFieldList);
	// UaSubscriptionCallback implementation ------------------------------------------------------

	// Create / delete a subscription on the server
	UaStatus createSubscription(UaSession* pSession);
	UaStatus deleteSubscription();

	// Create monitored items in the subscription
	UaStatus createMonitoredItems();

	// set the configuration where we get the list of NodeIds to monitored from.
	void setConfiguration(Configuration* pConfiguration);

private:
	UaStatus recoverSubscription();

private:
	UaSession* m_pSession;
	UaSubscription* m_pSubscription;
	Configuration* m_pConfiguration;

};

#endif // SAMPLESUBSCRIPTION_H
