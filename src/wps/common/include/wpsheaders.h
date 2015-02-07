/*
 * WPS header
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wpsheaders.h,v 1.7 2009/10/16 09:18:00 Exp $
 */

#ifndef _WPS_HEADERS_
#define _WPS_HEADERS_

#include <wpstypes.h>

#include <slist.h>

#include <wpstlvbase.h>

/* Include the following until we figure out where to put the beacons */
#include <reg_prototlv.h>


#define WPS_VERSION                0x10

/* Beacon Info */
typedef struct {
	CTlvVersion version;
	CTlvScState scState;
	CTlvAPSetupLocked apSetupLocked;
	CTlvSelRegistrar selRegistrar;
	CTlvDevicePwdId pwdId;
	CTlvSelRegCfgMethods selRegConfigMethods;
	CTlvUuid uuid;
	CTlvRfBand rfBand;
} WpsBeaconIE;

/* Probe Request Info */
typedef struct {
	CTlvVersion version;
	CTlvReqType reqType;
	CTlvConfigMethods confMethods;
	CTlvUuid uuid;
	CTlvPrimDeviceType primDevType;
	CTlvRfBand rfBand;
	CTlvAssocState assocState;
	CTlvConfigError confErr;
	CTlvDevicePwdId pwdId;
	CTlvPortableDevice portableDevice;
	CTlvVendorExt vendExt;
} WpsProbreqIE;

/* Probe Response Info */
typedef struct {
	CTlvVersion version;
	CTlvScState scState;
	CTlvAPSetupLocked apSetupLocked;
	CTlvSelRegistrar selRegistrar;
	CTlvDevicePwdId pwdId;
	CTlvSelRegCfgMethods selRegConfigMethods;
	CTlvRespType respType;
	CTlvUuid uuid;
	CTlvManufacturer manuf;
	CTlvModelName modelName;
	CTlvModelNumber modelNumber;
	CTlvSerialNum serialNumber;
	CTlvPrimDeviceType primDevType;
	CTlvDeviceName devName;
	CTlvConfigMethods confMethods;
	CTlvRfBand rfBand;
	CTlvVendorExt vendExt;
} WpsProbrspIE;


#endif /* _WPS_HEADERS_ */
