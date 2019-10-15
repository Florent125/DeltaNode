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
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "uabase.h"
#include "uaclientsdk.h"

#define COMPANY_NAME "UnifiedAutomation"
#define PRODUCT_NAME "GettingStartedClient"

using namespace UaClientSdk;

class Configuration
{
    UA_DISABLE_COPY(Configuration);
public:
    Configuration();
    virtual ~Configuration();

    // get connection and session parameters
    UaString getServerUrl() const;
    UaString getDiscoveryUrl() const;
    UaString getApplicationName() const;
    UaString getProductUri() const;
    OpcUa_Boolean getAutomaticReconnect() const;
    OpcUa_Boolean getRetryInitialConnect() const;

    // get lists of NodeIds and values
    UaNodeIdArray getNodesToRead() const;
    UaNodeIdArray getNodesToWrite() const;
    UaNodeIdArray getNodesToMonitor() const;
    UaVariantArray getWriteValues() const;

    // load configuration from file to fill connection parameters, NamespaceArray and NodeIds
    UaStatus loadConfiguration(const UaString& sConfigurationFile);

    // create the folder structure to handle certificates and load or create a client certificate.
    UaStatus setupSecurity(SessionSecurityInfo& sessionSecurityInfo);

    // update the namespace indexes for all nodeIds and update the internal namespaceArray
    UaStatus updateNamespaceIndexes(const UaStringArray& namespaceArray);

private:
    // connection and session configuration
    UaString        m_applicationName;
    UaString        m_serverUrl;
    UaString        m_discoveryUrl;
    OpcUa_Boolean   m_bAutomaticReconnect;
    OpcUa_Boolean   m_bRetryInitialConnect;

    // NamespaceArray and NodeIds
    UaStringArray   m_namespaceArray;
    UaNodeIdArray   m_nodesToRead;
    UaNodeIdArray   m_nodesToWrite;
    UaNodeIdArray   m_nodesToMonitor;
    UaVariantArray  m_writeValues;

    // Certificate handling
    UaString        m_certificateTrustListLocation;
    UaString        m_certificateRevocationListLocation;
    UaString        m_issuersCertificatesLocation;
    UaString        m_issuersRevocationListLocation;
    UaString        m_clientCertificateFile;
    UaString        m_clientPrivateKeyFile;
};

#endif // CONFIGURATION_H
