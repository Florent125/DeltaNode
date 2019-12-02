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
#include "samplesubscription.h"
#include "uasubscription.h"
#include "uasession.h"
#include "configuration.h"
#include <iostream>
#include <string>

using namespace std;

SampleSubscription::SampleSubscription(Configuration* pConfiguration)
	: m_pSession(NULL),
	m_pSubscription(NULL),
	m_pConfiguration(pConfiguration)
{
}

SampleSubscription::~SampleSubscription()
{
	if (m_pSubscription)
	{
		deleteSubscription();
	}
}

void SampleSubscription::subscriptionStatusChanged(
	OpcUa_UInt32      clientSubscriptionHandle, //!< [in] Client defined handle of the affected subscription
	const UaStatus& status)                   //!< [in] Changed status for the subscription
{
	OpcUa_ReferenceParameter(clientSubscriptionHandle); // We use the callback only for this subscription

	if (status.isBad())
	{
		printf("Subscription not longer valid - failed with status %s\n", status.toString().toUtf8());

		// recover subscription on the server
		recoverSubscription();
	}
}

void SampleSubscription::dataChange(
	OpcUa_UInt32               clientSubscriptionHandle, //!< [in] Client defined handle of the affected subscription
	const UaDataNotifications& dataNotifications,        //!< [in] List of data notifications sent by the server
	const UaDiagnosticInfos& diagnosticInfos)          //!< [in] List of diagnostic info related to the data notifications. This list can be empty.
{
	OpcUa_ReferenceParameter(clientSubscriptionHandle); // We use the callback only for this subscription
	OpcUa_ReferenceParameter(diagnosticInfos);
	OpcUa_UInt32 size, i;
	// Configure items to add to the subscription
	UaNodeIdArray lstNodeIds = m_pConfiguration->getNodesToMonitor();
	size = lstNodeIds.length();

	OpcUa_Float tmpFloat;
	OpcUa_Boolean tmpBool;
	OpcUa_Int datatype;
	int normalDatatype;

	printf("-- DataChange Notification ---------------------------------\n");
	for (i = 0; i < dataNotifications.length(); i++)
	{
		if (OpcUa_IsGood(dataNotifications[i].Value.StatusCode))
		{
			UaVariant tempValue = dataNotifications[i].Value.Value;
			UaVariant tempValue2 = UaVariant(dataNotifications[i].Value.ServerTimestamp);
			printf("  Variable = %d value = %s\n", dataNotifications[i].ClientHandle, tempValue.toString().toUtf8());
			printf("  DataType = %d value = %s\n", dataNotifications[i].ClientHandle, tempValue.dataType().toString().toUtf8());
			printf("  Timestamp = %d value = %s\n", dataNotifications[i].ClientHandle, tempValue2.toString().toUtf8());
			

			if (tempValue.dataType() == 1) //Boolean (has to be changed with the enum)
			{
				tempValue.toBool(tmpBool);
				opcUaBool[dataNotifications[i].ClientHandle] = tmpBool;
				//Test d'un outil avec entrée en booleen
				suctionTool = tmpBool;
			}

			if (tempValue.dataType() == 10) //Float (has to be changed with the enum)
			{
				tempValue.toFloat(tmpFloat);
				opcUaFloat[dataNotifications[i].ClientHandle] = tmpFloat;
			}
			
		
		}
		else
		{
			UaStatus itemError(dataNotifications[i].Value.StatusCode);
			printf("  Variable = %d failed with status %s\n", dataNotifications[i].ClientHandle, itemError.toString().toUtf8());
		}
	}
	printf("------------------------------------------------------------\n");
}

void SampleSubscription::newEvents(
	OpcUa_UInt32                clientSubscriptionHandle, //!< [in] Client defined handle of the affected subscription
	UaEventFieldLists& eventFieldList)           //!< [in] List of event notifications sent by the server
{
	OpcUa_ReferenceParameter(clientSubscriptionHandle);
	OpcUa_ReferenceParameter(eventFieldList);
}

UaStatus SampleSubscription::createSubscription(UaSession* pSession)
{
	if (m_pSubscription)
	{
		printf("\nError: Subscription already created\n");
		return OpcUa_BadInvalidState;
	}

	m_pSession = pSession;

	UaStatus result;

	ServiceSettings serviceSettings;
	SubscriptionSettings subscriptionSettings;
	subscriptionSettings.publishingInterval = 10;

	printf("\nCreating subscription ...\n");
	result = pSession->createSubscription(
		serviceSettings,
		this,
		1,
		subscriptionSettings,
		OpcUa_True,
		&m_pSubscription);

	if (result.isGood())
	{
		printf("CreateSubscription succeeded\n");
		OpcUa_UInt32 size, i;
		// Configure items to add to the subscription
		UaNodeIdArray lstNodeIds = m_pConfiguration->getNodesToMonitor();
		size = lstNodeIds.length();

		//Resize des vecteurs à la taille totales des variables bindées
		opcUaBool.resize(size);
		opcUaFloat.resize(size);
		printf("Creation de opcUaFloat: %i \n", opcUaFloat.size());
		printf("Creation de opcUaBool: %i \n", opcUaBool.size());
		
	}
	else
	{
		m_pSubscription = NULL;
		printf("CreateSubscription failed with status %s\n", result.toString().toUtf8());
	}

	return result;
}

UaStatus SampleSubscription::deleteSubscription()
{
	if (m_pSubscription == NULL)
	{
		printf("\nError: No Subscription created\n");
		return OpcUa_BadInvalidState;
	}

	UaStatus result;
	ServiceSettings serviceSettings;

	// let the SDK cleanup the resources for the existing subscription
	printf("\nDeleting subscription ...\n");
	result = m_pSession->deleteSubscription(
		serviceSettings,
		&m_pSubscription);

	if (result.isGood())
	{
		printf("DeleteSubscription succeeded\n");
	}
	else
	{
		printf("DeleteSubscription failed with status %s\n", result.toString().toUtf8());
	}
	m_pSubscription = NULL;

	return result;
}

UaStatus SampleSubscription::createMonitoredItems()
{
	if (m_pSubscription == NULL)
	{
		printf("\nError: No Subscription created\n");
		return OpcUa_BadInvalidState;
	}

	UaStatus result;
	OpcUa_UInt32 size, i;
	ServiceSettings serviceSettings;
	UaMonitoredItemCreateRequests itemsToCreate;
	UaMonitoredItemCreateResults createResults;

	// Configure items to add to the subscription
	UaNodeIdArray lstNodeIds = m_pConfiguration->getNodesToMonitor();
	size = lstNodeIds.length();
	itemsToCreate.create(size);
	for (i = 0; i < size; i++)
	{
		itemsToCreate[i].ItemToMonitor.AttributeId = OpcUa_Attributes_Value;
		OpcUa_NodeId_CopyTo(&lstNodeIds[i], &itemsToCreate[i].ItemToMonitor.NodeId);
		itemsToCreate[i].RequestedParameters.ClientHandle = i;
		itemsToCreate[i].RequestedParameters.SamplingInterval = 10;
		itemsToCreate[i].RequestedParameters.QueueSize = 1;
		itemsToCreate[i].RequestedParameters.DiscardOldest = OpcUa_True;
		itemsToCreate[i].MonitoringMode = OpcUa_MonitoringMode_Reporting;
	}

	printf("\nAdding monitored items to subscription ...\n");
	result = m_pSubscription->createMonitoredItems(
		serviceSettings,
		OpcUa_TimestampsToReturn_Both,
		itemsToCreate,
		createResults);

	if (result.isGood())
	{
		// check individual results
		for (i = 0; i < createResults.length(); i++)
		{
			if (OpcUa_IsGood(createResults[i].StatusCode))
			{
				printf("CreateMonitoredItems succeeded for item: %s\n",
					UaNodeId(itemsToCreate[i].ItemToMonitor.NodeId).toXmlString().toUtf8());
			}
			else
			{
				printf("CreateMonitoredItems failed for item: %s - Status %s\n",
					UaNodeId(itemsToCreate[i].ItemToMonitor.NodeId).toXmlString().toUtf8(),
					UaStatus(createResults[i].StatusCode).toString().toUtf8());
			}
		}
	}
	// service call failed
	else
	{
		printf("CreateMonitoredItems failed with status %s\n", result.toString().toUtf8());
	}

	return result;
}

void SampleSubscription::setConfiguration(Configuration* pConfiguration)
{
	m_pConfiguration = pConfiguration;
}

UaStatus SampleSubscription::recoverSubscription()
{
	UaStatus result;

	// delete existing subscription
	if (m_pSubscription)
	{
		deleteSubscription();
	}

	// create a new subscription
	result = createSubscription(m_pSession);

	// create monitored items
	if (result.isGood())
	{
		result = createMonitoredItems();
	}

	printf("-------------------------------------------------------------\n");
	if (result.isGood())
	{
		printf("RecoverSubscription succeeded.\n");
	}
	else
	{
		printf("RecoverSubscription failed with status %s\n", result.toString().toUtf8());
	}
	printf("-------------------------------------------------------------\n");

	return result;
}
