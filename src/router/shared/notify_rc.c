/*
 * This is the implementation of a routine to notify the rc driver that it
 * should take some action.
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "notify_rc.h"
#include "typedefs.h"


static void notify_rc_internal(const char *event_name, bool do_wait);


extern void notify_rc(const char *event_name)
{
	notify_rc_internal(event_name, FALSE);
}

extern void notify_rc_and_wait(const char *event_name)
{
	notify_rc_internal(event_name, TRUE);
}

static void notify_rc_internal(const char *event_name, bool do_wait)
{
	char *full_name;
	FILE *fp;
	int close_result;

	full_name = (char *)(malloc(strlen(event_name) + 100));
	if (full_name == NULL)
	{
		fprintf(stderr,
				"Error: Failed trying to allocate %lu bytes of memory for the "
				"full path of an rc notification marker file, while trying to "
				"notify rc of a `%s' event.\n",
				(unsigned long)(strlen(event_name) + 100), event_name);
		return;
	}

	sprintf(full_name, "/tmp/rc_notification/%s", event_name);
	fp = fopen(full_name, "w");
	if (fp == NULL)
	{
		fprintf(stderr,
				"Error: Failed trying to open file %s while trying to notify "
				"rc of an event: %s.\n", full_name, strerror(errno));
		free(full_name);
		return;
	}

	close_result = fclose(fp);
	if (close_result != 0)
	{
		fprintf(stderr,
				"Error: Failed trying to close file %s while trying to notify "
				"rc of an event: %s.\n", full_name, strerror(errno));
	}

	sprintf(full_name, "/tmp/rc_action_incomplete/%s", event_name);
	fp = fopen(full_name, "w");
	if (fp == NULL)
	{
		fprintf(stderr,
				"Error: Failed trying to open file %s while trying to notify "
				"rc of an event: %s.\n", full_name, strerror(errno));
		free(full_name);
		return;
	}

	close_result = fclose(fp);
	if (close_result != 0)
	{
		fprintf(stderr,
				"Error: Failed trying to close file %s while trying to notify "
				"rc of an event: %s.\n", full_name, strerror(errno));
	}

	sprintf(full_name, "/tmp/rc_notification/%s", event_name);
	fp = fopen(full_name, "w");
	if (fp == NULL)
	{
		fprintf(stderr,
				"Error: Failed trying to open file %s while trying to notify "
				"rc of an event: %s.\n", full_name, strerror(errno));
		free(full_name);
		return;
	}

	close_result = fclose(fp);
	if (close_result != 0)
	{
		fprintf(stderr,
				"Error: Failed trying to close file %s while trying to notify "
				"rc of an event: %s.\n", full_name, strerror(errno));
	}
        
	//wendebug
	int killproc = 1;
	//printf("notify_rc_internal : kill %d\n", killproc);
	kill(killproc, SIGUSR1);

	if (do_wait)
	{
		sprintf(full_name, "/tmp/rc_action_incomplete/%s", event_name);

		while (TRUE)
		{
			fp = fopen(full_name, "r");
			if (fp == NULL)
				break;
			fclose(fp);
			sleep(1);
		}
	}

	free(full_name);
}
