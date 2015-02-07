/*
 * Broadcom UPnP module, device_init.c
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: device_init.c,v 1.10 2009/07/04 11:58:19 Exp $
 */
#include <upnp.h>

/* << TABLE BEGIN */
/*
 * WARNNING: DON'T MODIFY THE FOLLOWING TABLES
 * AND DON'T REMOVE TAG :
 *          "<< TABLE BEGIN"
 *          ">> TABLE END"
 */

#ifdef __CONFIG_NAT__
#ifdef __BCMIGD__
extern UPNP_DEVICE	InternetGatewayDevice;
#endif /* __BCMIGD__ */
#endif /* __CONFIG_NAT__ */


UPNP_DEVICE *upnp_device_table[] =
{
#ifdef __CONFIG_NAT__
#ifdef __BCMIGD__
	&InternetGatewayDevice,
#endif /* __BCMIGD__ */
#endif /* __CONFIG_NAT__ */

	0
};
/* >> TABLE END */
