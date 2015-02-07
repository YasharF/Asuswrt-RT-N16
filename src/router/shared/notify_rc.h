/*
 * This is the interface to a routine to notify the rc driver that it should
 * take some action.
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

#ifndef NOTIFY_RC_H
#define NOTIFY_RC_H

#include <typedefs.h>


extern void notify_rc(const char *event_name);
extern void notify_rc_and_wait(const char *event_name);


#endif /* NOTIFY_RC_H */
