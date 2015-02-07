/*
 * Broadcom UPnP module, xml_x_wainipconnection.c
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: xml_x_wanipconnection.c,v 1.5 2007/11/23 09:51:55 Exp $
 */
#include <upnp.h>

char xml_x_wanipconnection[]=
	"<?xml version=\"1.0\"?>\r\n"
	"<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\r\n"
	"\t<specVersion>\r\n"
	"\t\t<major>1</major>\r\n"
	"\t\t<minor>0</minor>\r\n"
	"\t</specVersion>\r\n"
	"\t<actionList>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>SetConnectionType</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewConnectionType</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>ConnectionType</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetConnectionTypeInfo</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewConnectionType</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>ConnectionType</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewPossibleConnectionTypes</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PossibleConnectionTypes</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>RequestConnection</name>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>ForceTermination</name>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetStatusInfo</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewConnectionStatus</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>ConnectionStatus</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewLastConnectionError</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>LastConnectionError</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewUptime</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>Uptime</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetNATRSIPStatus</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewRSIPAvailable</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>RSIPAvailable</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewNATEnabled</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>NATEnabled</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetGenericPortMappingEntry</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewPortMappingIndex</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingNumberOfEntries</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewRemoteHost</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>RemoteHost</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewExternalPort</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>ExternalPort</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewProtocol</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingProtocol</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewInternalPort</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>InternalPort</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewInternalClient</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>InternalClient</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewEnabled</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingEnabled</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewPortMappingDescription</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingDescription</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewLeaseDuration</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingLeaseDuration</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetSpecificPortMappingEntry</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewRemoteHost</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>RemoteHost</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewExternalPort</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>ExternalPort</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewProtocol</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingProtocol</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewInternalPort</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>InternalPort</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewInternalClient</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>InternalClient</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewEnabled</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingEnabled</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewPortMappingDescription</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingDescription</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewLeaseDuration</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingLeaseDuration</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>AddPortMapping</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewRemoteHost</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>RemoteHost</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewExternalPort</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>ExternalPort</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewProtocol</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingProtocol</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewInternalPort</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>InternalPort</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewInternalClient</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>InternalClient</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewEnabled</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingEnabled</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewPortMappingDescription</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingDescription</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewLeaseDuration</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingLeaseDuration</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>DeletePortMapping</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewRemoteHost</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>RemoteHost</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewExternalPort</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>ExternalPort</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewProtocol</name>\r\n"
	"\t\t\t\t\t\t<direction>in</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>PortMappingProtocol</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t\t<action>\r\n"
	"\t\t\t<name>GetExternalIPAddress</name>\r\n"
	"\t\t\t\t<argumentList>\r\n"
	"\t\t\t\t\t<argument>\r\n"
	"\t\t\t\t\t\t<name>NewExternalIPAddress</name>\r\n"
	"\t\t\t\t\t\t<direction>out</direction>\r\n"
	"\t\t\t\t\t\t<relatedStateVariable>ExternalIPAddress</relatedStateVariable>\r\n"
	"\t\t\t\t\t</argument>\r\n"
	"\t\t\t\t</argumentList>\r\n"
	"\t\t</action>\r\n"
	"\t</actionList>\r\n"
	"\t<serviceStateTable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>ConnectionType</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"yes\">\r\n"
	"\t\t\t<name>PossibleConnectionTypes</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t\t<allowedValueList>\r\n"
	"\t\t\t\t<allowedValue>Unconfigured</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>IP_Routed</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>IP_Bridged</allowedValue>\r\n"
	"\t\t\t</allowedValueList>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"yes\">\r\n"
	"\t\t\t<name>ConnectionStatus</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t\t<defaultValue>Unconfigured</defaultValue>\r\n"
	"\t\t\t<allowedValueList>\r\n"
	"\t\t\t\t<allowedValue>Unconfigured</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>Connected</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>Disconnected</allowedValue>\r\n"
	"\t\t\t</allowedValueList>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>Uptime</name>\r\n"
	"\t\t\t<dataType>ui4</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>LastConnectionError</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t\t<allowedValueList>\r\n"
	"\t\t\t\t<allowedValue>ERROR_NONE</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>ERROR_UNKNOWN</allowedValue>\r\n"
	"\t\t\t</allowedValueList>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>RSIPAvailable</name>\r\n"
	"\t\t\t<dataType>boolean</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>NATEnabled</name>\r\n"
	"\t\t\t<dataType>boolean</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"yes\">\r\n"
	"\t\t\t<name>PortMappingNumberOfEntries</name>\r\n"
	"\t\t\t<dataType>ui2</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>PortMappingEnabled</name>\r\n"
	"\t\t\t<dataType>boolean</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>PortMappingLeaseDuration</name>\r\n"
	"\t\t\t<dataType>ui4</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>RemoteHost</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>ExternalPort</name>\r\n"
	"\t\t\t<dataType>ui2</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>InternalPort</name>\r\n"
	"\t\t\t<dataType>ui2</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>PortMappingProtocol</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t\t<allowedValueList>\r\n"
	"\t\t\t\t<allowedValue>TCP</allowedValue>\r\n"
	"\t\t\t\t<allowedValue>UDP</allowedValue>\r\n"
	"\t\t\t</allowedValueList>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>InternalClient</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"no\">\r\n"
	"\t\t\t<name>PortMappingDescription</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t\t<stateVariable sendEvents=\"yes\">\r\n"
	"\t\t\t<name>ExternalIPAddress</name>\r\n"
	"\t\t\t<dataType>string</dataType>\r\n"
	"\t\t</stateVariable>\r\n"
	"\t</serviceStateTable>\r\n"
	"</scpd>\r\n"
	"\r\n";
