/*
 * Broadcom UPnP module request message include file
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: upnp_req.h,v 1.20 2009/08/11 04:41:08 Exp $
 */

#ifndef __UPNP_REQ_H__
#define __UPNP_REQ_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* UPnP message handler */
int upnp_request_init(UPNP_CONTEXT *context);
void upnp_request_shutdown(UPNP_CONTEXT *context);
void upnp_request_handler(UPNP_CONTEXT *context);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __UPNP_REQ_H__ */
