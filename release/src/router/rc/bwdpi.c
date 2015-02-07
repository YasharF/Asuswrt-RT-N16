/*
	bwdpi.c for TrendMicro DPI engine / iQoS / WRS / APP partol

	DPI engine 	: applications and devices identify engine
	iQoS 		: tc control rule and qosd.conf
	WRS 		: TrendMicro parental control and wrs.conf
	APP partol	: app parental control
*/

#include "bwdpi.h"

static void show_help(char *base)
{
	printf("%s Usage :\n", base);
	printf("  bwdpi stat -t [time] -o [action]\n");
	printf("  time	: time interval to get information\n");
	printf("  action: app_get / udb_get / app_patrol_set\n");
}

int bwdpi_main(int argc, char **argv)
{
	dbg("[bwdpi] argc=%d, argv[0]=%s, argv[1]=%s, argv[2]=%s\n", argc, argv[0], argv[1], argv[2]);
	int c, timeval = 0;
	char *action = NULL;

	if (argc == 1){
		printf("Usage :\n");
		printf("  bwdpi [iqos/qosd/wrs] [start/stop/restart]\n");
		printf("  bwdpi stat -t [time] -o [app_get/udb_get]\n");
		printf("  bwdpi dpi [0/1]\n");
		printf("  bwdpi qosconf\n");
		return 0;
	}

	if (!strcmp(argv[1], "iqos")){
		if(argc != 3)
		{
			printf("  bwdpi iqos [start/stop/restart]\n");
			return 0;
		}
		else
		{
			return tm_qos_main(argv[2]);
		}
	}
	else if (!strcmp(argv[1], "qosd")){
		if(argc != 3)
		{
			printf("  bwdpi qosd [start/stop/restart]\n");
			return 0;
		}
		else
		{
			return qosd_main(argv[2]);
		}
	}
	else if (!strcmp(argv[1], "wrs")){
		if(argc != 3)
		{
			printf("  bwdpi wrs [start/stop/restart]\n");
			return 0;
		}
		else
		{
			return wrs_main(argv[2]);
		}

	}
	else if (!strcmp(argv[1], "stat")){
		while ((c = getopt(argc, argv, "t:o:h")) != -1)
		{
			switch(c)
			{
				case 't':
					timeval = atoi(optarg);
					break;
				case 'o':
					action = optarg;
					break;
				case 'h':
					show_help(argv[1]);
					break;
				default:
					printf("ERROR: unknown option %c\n", c);
					break;
			}
		}
		dbg("[bwdpi] timeval=%d, action=%s\n", timeval, action);
		return stat_main(timeval, action);
	}
	else if (!strcmp(argv[1], "dpi")){
		if(argc != 3)
		{
			printf("  bwdpi dpi [0/1]\n");
			return 0;
		}
		else
		{
			return dpi_main(argv[2]);
		}
	}
	else if (!strcmp(argv[1], "qosconf")){
		if(argc != 2)
		{
			printf("  bwdpi qosconf\n");
			return 0;
		}
		else
		{
			setup_qos_conf();
			return 1;
		}
	}

	return 1;
}
