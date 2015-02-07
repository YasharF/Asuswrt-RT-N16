/*
 * WPS IE
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wps_ie.h,v 1.5 2009/10/05 02:11:29 Exp $
 */

#ifndef __WPS_IE_H__
#define __WPS_IE_H__

int wps_ie_default_ssr_info(CTlvSsrIE *ssrmsg);
void wps_ie_set(char *wps_ifname, CTlvSsrIE *ssrmsg);
void wps_ie_clear();


#endif	/* __WPS_IE_H__ */
