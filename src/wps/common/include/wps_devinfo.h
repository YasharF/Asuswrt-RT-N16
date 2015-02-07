/*
 * WPS device infomation
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wps_devinfo.h,v 1.3 2009/03/06 15:52:15 Exp $
 */

#ifndef __WPS_DEVICE_INFO_H__
#define __WPS_DEVICE_INFO_H__


/* data structure to hold Enrollee and Registrar information */
typedef struct {
    uint8   version;
    uint8   uuid[SIZE_16_BYTES];
    uint8   macAddr[SIZE_6_BYTES];
    char    deviceName[SIZE_32_BYTES];
    uint16  primDeviceCategory;
    uint32  primDeviceOui;
    uint16  primDeviceSubCategory;
    uint16  authTypeFlags;
    uint16  encrTypeFlags;
    uint8   connTypeFlags;
    uint16  configMethods;
    uint8   scState;
    char    manufacturer[SIZE_64_BYTES];
    char    modelName[SIZE_32_BYTES];
    char    modelNumber[SIZE_32_BYTES];
    char    serialNumber[SIZE_32_BYTES];
    uint8   rfBand;
    uint32  osVersion;
    uint32  featureId;
    uint16  assocState;
    uint16  devPwdId;
    uint16  configError;
    bool    b_ap;
    char    ssid[SIZE_SSID_LENGTH];
    char    keyMgmt[SIZE_20_BYTES];
    uint16  auth;
    uint16  wep;
    uint16  crypto;
} DevInfo;


#endif /* __WPS_DEVICE_INFO_H__ */
