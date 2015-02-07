/*
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND ASUS GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: watchdog.c,v 1.6 2008/08/14 09:02:31 james26_jang Exp $
 */

 
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <shutils.h>
#include <rc.h>
#include <stdarg.h>
#include <wlioctl.h>
#include <syslog.h>
#include <bcmnvram.h>
#include <fcntl.h>
#include <linux_gpio.h>
#include <sys/stat.h>
#include <math.h>
#include <security_ipc.h>
/*
#include <shutils.h>
#include <security_ipc.h>
#include <wlif_utils.h>
*/
/* +++ Cherry Cho added in 2007/1/4. +++ */
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
/* --- Cherry Cho added in 2007/1/4. --- */

#define BCM47XX_SOFTWARE_RESET  0x40		/* GPIO 6 */
#define RESET_WAIT		3		/* seconds */
#define RESET_WAIT_COUNT	RESET_WAIT * 10 /* 10 times a second */

#define NORMAL_PERIOD		1		/* second */
#define URGENT_PERIOD		100 * 1000	/* microsecond */	
#define RUSHURGENT_PERIOD	50 * 1000	/* microsecond */	
						/* 1/10 second */
#define STACHECK_PERIOD_CONNECT 60/5		/* 30 seconds */
#define STACHECK_PERIOD_DISCONNECT 5/5		/* 5 seconds */

#ifdef WCN
#define QUICK_PERIOD		100 * 1000	/* microsecond */
int wcn_on=0;
int wcn_led=0;
int wcn_led_state=0;
int wcn_timer=2;
int reboot_WCN_count=0;
#endif

#define GPIO0 0x0001
#define GPIO1 0x0002
#define GPIO2 0x0004
#define GPIO3 0x0008
#define GPIO4 0x0010
#define GPIO5 0x0020
#define GPIO6 0x0040
#define GPIO7 0x0080
#define	GPIO8 0x0100
#define GPIO15 0x8000

#define LED_ON 	0
#define LED_OFF 1

#ifdef RTN16
#define LED_POWER	GPIO1
#define BTN_RESET	GPIO6 
#define BTN_SETUP	GPIO8
#define LED_RADIO 	GPIO7
#endif

#ifdef RTN12
#define LED_POWER	GPIO2
#define BTN_RESET	GPIO1 
#define BTN_SETUP	GPIO0
#define BTN_SW_1	GPIO6
#define BTN_SW_2	GPIO5
#define BTN_SW_3	GPIO4
#define LED_RADIO 	GPIO7
#endif

#ifdef RTN10
#define LED_POWER	GPIO1
#define BTN_RESET	GPIO3 
#define BTN_SETUP	GPIO2
#define LED_RADIO 	GPIO7
#endif

#ifdef BTN_SETUP
#define SETUP_WAIT		3		/* seconds */
#define SETUP_WAIT_COUNT	100 /* The real waiting time depends on the alarm timer. Cherry Cho modified in 2008/7/4. */
#define SETUP_TIMEOUT		60 		/* seconds */
#define SETUP_TIMEOUT_COUNT	SETUP_TIMEOUT * 10 /* 60 times a second */
#endif //BTN_SETUP

char AIR_LED_OFF[]={0x67, 0x70, 0x69, 0x6f, 0x74, 0x69, 0x6d, 0x65, 0x72, 0x76, 0x61, 0x6c, 0x0, 0x0, 0x10,  0x0, 0x0};
char AIR_LED_ON[]= {0x67, 0x70, 0x69, 0x6f, 0x74, 0x69, 0x6d, 0x65, 0x72, 0x76, 0x61, 0x6c, 0x0, 0x0, 0x00, 0x64, 0x0};

/* Global containing the file descriptor of the GPIO kernel drivers */
static int bcmgpio_fd;

/* Global containing information about each GPIO pin */

/* Static function prototypes */

/* Generic functions to read/write Chip Common core's GPIO registers on the AP */
int tgpio_drvinit ();
void tgpio_drvcleanup ();

int tgpio_ioctl(int gpioreg, unsigned int mask , unsigned int val);

/* GPIO registers */
#define BCMGPIO_REG_IN		0
#define BCMGPIO_REG_OUT		1
#define BCMGPIO_REG_OUTEN	2
#define BCMGPIO_REG_RESERVE	3
#define BCMGPIO_REG_RELEASE	4


#define LED_CONTROL(led,flag) gpio_write("/dev/gpio/out", led, flag)


struct itimerval itv;
int watchdog_period=0;
static int btn_pressed=0;
static int btn_count = 0;
long sync_interval=-1; // every 30 seconds a unit
int sync_flag=0;
long timestamp_g=0;
int stacheck_interval=-1;
#ifdef BTN_SETUP
int btn_pressed_setup=0;
int btn_pressed_flag=0;
int btn_count_setup=0;
int btn_count_timeout=0;
int btn_stage=0;
#endif
#ifdef WSC
int btn_addER = 0;
/* +++ Cherry Cho added in 2008/7/16. +++ */
int WSC_ENABLED;
int wps_success = 1;
int led_count = 0;
int led_count_timeout = 100;
int btn_led_count = 0;
int ap_configed = 0;
int wps_pbc_enable = 0;
//extern int start_wsc(void);
/* --- Cherry Cho added in 2008/7/16. --- */

//#define USLEEP(X)	usleep(X)

#endif

#ifdef CDMA
int cdma_down=0;
int cdma_connect=0;
#endif

int reboot_count=0;
static int int_nas_enable=0;
static int nas_check_interval=0;

int tgpio_fd_init ();
void tgpio_fd_cleanup ();
int tgpio_ioctl(int gpioreg, unsigned int mask , unsigned int val);
void gpio_write(char *dev, int gpio, int val);

void wl_led_ctrl(int ctrl);/* Cherry Cho added in 2008/7/16. */

/* WPS control functions */
#if 0
static int write_to_wps(int fd, char *cmd);
static int read_from_wps(int fd, char *databuf, int datalen);
int parse_wps_env(char *buf);
static int get_wps_env();
static int set_wps_env(char *uibuf);

static int wps_config_command;
static int wps_action;
static char wps_uuid[36];
static char wps_unit[32];
#endif

int
tgpio_fd_init ()
{
	bcmgpio_fd = open("/dev/gpio", O_RDWR);
	if (bcmgpio_fd == -1) {
		printf ("Failed to open /dev/gpio\n");
		return -1;
	}
	return 0;
}

void
tgpio_fd_cleanup ()
{
	if (bcmgpio_fd!= -1) {
		close (bcmgpio_fd);
		bcmgpio_fd = -1;
	}
}

int
tgpio_ioctl(int gpioreg, unsigned int mask , unsigned int val)
{
	struct gpio_ioctl gpio;
	int type;

	gpio.val = val;
	gpio.mask = mask;

	switch (gpioreg) {
		case BCMGPIO_REG_IN:
			type = GPIO_IOC_IN;
			break;
		case BCMGPIO_REG_OUT:
			type = GPIO_IOC_OUT;
			break;
		case BCMGPIO_REG_OUTEN:
			type = GPIO_IOC_OUTEN;
			break;
		case BCMGPIO_REG_RESERVE:
			type = GPIO_IOC_RESERVE;
			break;
		case BCMGPIO_REG_RELEASE:
			type = GPIO_IOC_RELEASE;
			break;
		default:
			printf ("invalid gpioreg %d\n", gpioreg);
			return -1;
	}
	if (ioctl(bcmgpio_fd, type, &gpio) < 0) {
		printf ("invalid gpioreg %d\n", gpioreg);
		return -1;
	}
	return (gpio.val);
}

void gpio_write(char *dev, int gpio, int val)
{
	unsigned int gpio_type;
	unsigned long bitmask;
	unsigned long gpio_val;

	if ( strcmp (dev, "/dev/gpio/in") == 0)
		gpio_type = BCMGPIO_REG_IN;
	else if ( strcmp (dev, "/dev/gpio/out") == 0)
		gpio_type = BCMGPIO_REG_OUT;
	else if ( strcmp (dev, "/dev/gpio/outen") == 0)
		gpio_type = BCMGPIO_REG_OUTEN;
	else
		printf("ERROR GPIO NAME %s\n", dev);

	tgpio_fd_init();
	tgpio_ioctl(BCMGPIO_REG_RESERVE, gpio, gpio);
	if (val > 0)
	tgpio_ioctl(gpio_type, gpio, gpio);
	else 
	tgpio_ioctl(gpio_type, gpio, 0);
	tgpio_fd_cleanup();
}

int gpio_read (char *dev, int gpio)
{
	unsigned int gpio_type;
	int ret;
	unsigned long bitmask;
	if ( strcmp (dev, "/dev/gpio/in") == 0)
		gpio_type = BCMGPIO_REG_IN;
	else if ( strcmp (dev, "/dev/gpio/out") == 0)
		gpio_type = BCMGPIO_REG_OUT;
	else if ( strcmp (dev, "/dev/gpio/outen") == 0)
		gpio_type = BCMGPIO_REG_OUTEN;
	else
		printf("ERROR GPIO NAME %s\n", dev);
	
	tgpio_fd_init();

	/* read the value of GPIO*/
	ret = (tgpio_ioctl(gpio_type, gpio, 0));
	tgpio_fd_cleanup();
	
	return (ret&gpio);
}

/* Functions used to control led and button */
gpio_init()
{
	gpio_write("/dev/gpio/outen", LED_POWER, 1);
	gpio_write("/dev/gpio/outen", BTN_RESET, 0);
#ifdef BTN_SETUP
	gpio_write("/dev/gpio/outen", BTN_SETUP, 0);
#endif
	gpio_write("/dev/gpio/out", LED_POWER, 0);
#ifdef RTN12
	gpio_write("/dev/gpio/outen", BTN_SW_1, 0);
	gpio_write("/dev/gpio/outen", BTN_SW_2, 0);
	gpio_write("/dev/gpio/outen", BTN_SW_3, 0);
#endif
}

static void
alarmtimer(unsigned long sec,unsigned long usec)
{
	itv.it_value.tv_sec  = sec;
	itv.it_value.tv_usec = usec;
	itv.it_interval = itv.it_value;
	setitimer(ITIMER_REAL, &itv, NULL);
}

void btn_check(void)
{
#ifdef WCN
	if (wcn_on==1)
	{
		wcn_led++;
		wcn_led%=wcn_timer;
		wcn_led_state=(wcn_led==0?1:0);

		if (wcn_led_state)
			LED_CONTROL(LED_POWER, LED_ON);
		else
			LED_CONTROL(LED_POWER, LED_OFF);

		return;
	}
#endif
	
#ifdef BTN_SETUP
#if 0/* Cherry Cho marked for stoping using EZsetup in 2007/2/12. */
	if (btn_pressed_flag)
	{
		btn_pressed_flag++;

		if (btn_pressed_flag==8)
		{
			start_ots();
		}
		else if (btn_pressed_flag==10)
		{		
			btn_pressed_flag=0;
			btn_pressed_setup=BTNSETUP_START;
			btn_count_setup=0;
			alarmtimer(0, RUSHURGENT_PERIOD);
		}
	}
#endif
	if (btn_pressed_setup==BTNSETUP_NONE)
	{
#endif
		if (!gpio_read("/dev/gpio/in", BTN_RESET)) {
			/*--------------- Add BTN_RST MFG test ------------------------*/
			if (!nvram_match("asus_mfg", "")) {
				nvram_set("btn_rst", "1");
			}
			else {
				if (!btn_pressed) {
					btn_pressed = 1;
					btn_count = 0;
					alarmtimer(0, URGENT_PERIOD); /* URGENT_PERIOD = 100*1000 microseconds */
				}
				else {	/* Whenever it is pushed steady */
					if (++btn_count > RESET_WAIT_COUNT) {
						btn_pressed = 2;
					}
					
					if (btn_pressed == 2) {
						/* 0123456789 */
						/* 0011100111 */
						if ((btn_count%10) < 1 ||
								((btn_count%10) > 4 && (btn_count%10) < 7)) {
							LED_CONTROL(LED_POWER, LED_OFF);
						}
						else {
							LED_CONTROL(LED_POWER, LED_ON);
						}
					}
				}
			} //end BTN_RST MFG test
		}
		else {
			if (btn_pressed == 1) {
				btn_count = 0;
				btn_pressed = 0;			
				LED_CONTROL(LED_POWER, LED_ON);
				alarmtimer(NORMAL_PERIOD, 0);
			}
			else if (btn_pressed == 2) {
				LED_CONTROL(LED_POWER, LED_OFF);
				alarmtimer(0, 0);
				eval("erase", "/dev/mtd3");
				kill(1, SIGTERM);
			}
		}
#ifdef BTN_SETUP
	}
	
	if (btn_pressed!=0) return;
	
	if (btn_pressed_setup < BTNSETUP_START)
	{
		if (	((!gpio_read("/dev/gpio/in", BTN_SETUP)) &&  	// BTN pressed
			(ap_configed == 0) && 	// Router config as WEP-64
			((nvram_invmatch("sw_mode_ex", "1") && nvram_match("wps_config_state", "0")) || (nvram_match("sw_mode_ex", "1"))) // Repeater/AP Mode with un-configurated status
			) || ((nvram_match("wps_enable_httpd", "1"))) // Press button on web page
			)
		{
			/* Add BTN_EZ MFG test */ 
			if (!nvram_match("asus_mfg", "")) {
				nvram_set("btn_ez", "1");
			}
			else {
				if (btn_pressed_setup==BTNSETUP_NONE)
				{
					btn_pressed_setup=BTNSETUP_DETECT;
					btn_count_setup=0;
					alarmtimer(0, RUSHURGENT_PERIOD);/* RUSHURGENT_PERIOD = 50*1000 microseconds */
					if (nvram_match("wps_enable_httpd", "1"))
						nvram_set ("wps_enable_httpd", "0");
				}
#ifdef PBCENR
				/* Do it for Repeater/AP mode if button is pressed */
				else if ( nvram_match("sw_mode_ex", "3") ||
					  (nvram_match("sw_mode_ex", "2") && nvram_match("sta_ssid", "")) )
				{
					char *argv[] = {"pbc_enr_monitor", NULL};
					pid_t pid;
					_eval(argv, NULL, 0, &pid);
					btn_pressed_setup = BTNSETUP_START;
					btn_count_setup = 0;
					alarmtimer(0, RUSHURGENT_PERIOD);

					nvram_set("wps_proc_status", "0");
				}
#endif
				else {	/* Whenever it is pushed steady more than 5 seconeds */
					if ( (++btn_count_setup > SETUP_WAIT_COUNT) ) 
					{
						if (nvram_match("wps_config_state", "1") || !WSC_ENABLED) { // Switch to WPS mode
							btn_pressed_setup=BTNSETUP_DETECT;
							ap_configed = 1;
						}
#ifdef WSC/* Cherry Cho modified in 2008/6/24. */
//						if (WSC_ENABLED) {
						else if (WSC_ENABLED) {
							if (nvram_match("wps_waiting", "1")
							   && nvram_match("wps_timer_start", "1")
							   && nvram_match("wps_method", "1")) {
								/* When PIN configuration is running, PBC can interrupt PIN.*/
								nvram_set("wps_config_command", "2"); //Timeout PIN process immediately
								stop_wps_proc();
							}
							else {	
								/* Run WSC Push Button to Get Configuration for ASUS's utility*/
								if (nvram_match("wps_waiting", "0")
								   &&nvram_match("wps_timer_start", "0")) {
									nvram_set("wps_pbc_force", "1");
								/* Set variables for WPS te generate configation when AP is in un-configured mode */
//										nvram_set("wl_wps_reg", "disabled");
//										nvram_set("wl0_wps_reg", "disabled");

									/* Use push button to connect client*/
									nvram_set("wps_method", "2");
									nvram_set("wps_proc_status", "0");
									nvram_set("wps_device_pin","00000000");
									nvram_set("wps_config_command","1");
									nvram_set("wps_unit", nvram_safe_get("wl_unit"));
									nvram_set("wps_client_role", "enrollee");

									if (nvram_match("wps_config_state", "0")) {
										nvram_set("wps_asus_mode", "1");
//										nvram_set("wps_config_state", "1");
										btn_addER = 0;
									}
									else
										btn_addER = 1;

//									stop_wps_proc();
//									sleep(6);
//									restart_wps();
									btn_pressed_setup=BTNSETUP_START;
									btn_count_setup=0;
									alarmtimer(0, RUSHURGENT_PERIOD);
									eval("killall", "wps_monitor");
									//_eval("/bin/wps_monitor");
									//start_wps();
									restart_wps_proc();
									sleep(1);
									start_wps_proc(WPS_UI_METHOD_PBC);
								}
							}
						}
#endif //WSC
					}
				}
			} //end BTN_EZ MFG test
		}
		else if (btn_pressed_setup==BTNSETUP_DETECT)/* Cherry Cho modified in 2008/6/24. */
		{
			if (ap_configed == 1) {
				ap_configed = 0;
			}
#ifdef WSC
			if (!WSC_ENABLED) {
				restart_wps();
				WSC_ENABLED = 1;
				wps_pbc_enable = 1;
			}

			if (WSC_ENABLED) { /* Cherry Cho added in 2008/7/21. */
				if (nvram_match("wps_waiting", "1") //The PIN process is running.
	   	   	   		&& nvram_match("wps_timer_start", "1")
	   	   	   		&& nvram_match("wps_method", "1"))
	   	   		{
					nvram_set("wps_config_command", "2");
					stop_wps_proc();
				}		
				else  {

				 	if ( (nvram_match("wps_waiting", "0")) &&
					    (nvram_match("wps_timer_start", "0")))
					{
#if 0
					        char nvifname[IFNAMSIZ];
 					        char osifname[IFNAMSIZ];

				                char pin[9] = {0};
				                char action_str[16];

				                char uibuf[256] = "SET ";
				                int uilen = 4;
#endif
						/* Set variables for WPS te generate configation when AP is in un-configured mode */
						if (nvram_match("wps_config_state", "0")) {
							nvram_set("wps_unconfig_addenr", "1");
							nvram_set("wl_wps_reg", "enabled");
							nvram_set("wl0_wps_reg", "enabled");
							nvram_set("wl_wps_config_state", "1");
							nvram_set("wl0_wps_config_state", "1");
							nvram_set("wps_config_state", "1");
						}

						if (nvram_match("wps_unconfig_addenr", "0")) {
							nvram_set("is_default", "0");
							nvram_set("is_modified", "1");
						}

						if (nvram_match("wl_wps_reg", "disabled")) {		
							nvram_set("wl_wps_config_state", "1");
							nvram_set("wl0_wps_config_state", "1");
							nvram_set("wl_wps_reg", "enabled");
							nvram_set("wl0_wps_reg", "enabled");
						}

						/* Use push button to connect client*/
//						uilen += sprintf(uibuf + uilen, "wps_method=%d ", WPS_UI_METHOD_PBC);
//						uilen += sprintf(uibuf + uilen, "wps_action=%d ", WPS_UI_ACT_CONFIGAP);

//						nvram_set("wps_proc_status", "0");
//						strcpy(wps_unit, nvram_safe_get("wl_unit"));
						
//						uilen += sprintf(uibuf + uilen, "wps_config_command=%d ", WPS_UI_CMD_START);
//			                        sprintf(nvifname, "wl%s", wps_unit);
//			                        nvifname_to_osifname(nvifname, osifname, sizeof(osifname));

//			                        uilen += sprintf(uibuf + uilen, "wps_pbc_method=%d ", WPS_UI_PBC_SW);
//			                        uilen += sprintf(uibuf + uilen, "wps_ifname=%s ", osifname);
//			                        set_wps_env(uibuf);


						nvram_set("wps_method", "2");
						nvram_set("wps_proc_status", "0");
//						nvram_set("wps_device_pin", nvram_get("secret_code"));
						nvram_set("wps_sta_pin","00000000");
						nvram_set("wps_config_command","1");
						nvram_set("wps_unit", nvram_safe_get("wl_unit"));
		 				nvram_set("wps_client_role", "enrollee");

//#endif

						btn_addER = 1;
						btn_pressed_setup=BTNSETUP_START;
						btn_count_setup=0;
						alarmtimer(0, RUSHURGENT_PERIOD);

						start_wps_proc(WPS_UI_METHOD_PBC);
					}
				}
			}
			else/* WSC is not enabled. Cherry Cho added in 2008/7/21. */
			{
				btn_pressed_setup = BTNSETUP_START;
				btn_count_setup = 0;
			}
#endif //WSC
		}
	}
	else /* Cherry Cho modified in 2008/1/15. */
	{		
#ifdef WSC
		if (WSC_ENABLED) {
			if ( (!btn_addER && nvram_match("wps_method","2")
//				&& nvram_match("wps_config_state","1") )
/*				&& nvram_match("wps_config_state","1")*/ )
				&& nvram_invmatch("wps_proc_status", "4"))
			{
					++btn_count_setup;
					btn_count_setup = btn_count_setup%20;
	
					/* 0123456789 */
					if ((btn_count_setup%2) == 0 && (btn_count_setup > 10))
						LED_CONTROL(LED_POWER, LED_ON);
					else
						LED_CONTROL(LED_POWER, LED_OFF);
			}
			else if ( (btn_addER && nvram_match("wps_method","2")) && nvram_invmatch("wps_proc_status", "4"))
			{
					++btn_count_setup;
					btn_count_setup = btn_count_setup%6;

					/* RUSHURGENT_PERIOD = 0.05 seconds 
						WPS in process: LED ON 0.2 seconds  LED OFF 0.1 seconds 
							LED: 0123 ON  45 OFF */	
					if (btn_count_setup <= 3)
						LED_CONTROL(LED_POWER, LED_ON);
					else
						LED_CONTROL(LED_POWER, LED_OFF);

				if (nvram_match("wps_proc_status", "0"))
					nvram_set("wps_proc_status", "4");
			}
			else
			{
				/* Cherry Cho added in 2008/7/16. */
//				if ( !wps_success ) //WPS process fails to complete.
				if ( (nvram_match("wps_proc_status","3") || nvram_match("wps_proc_status","4")))
				{
					if ( led_count < led_count_timeout )
					{
						++btn_count_setup;	
						btn_count_setup = btn_count_setup%4;
						if (btn_count_setup <= 1)
							LED_CONTROL(LED_POWER, LED_ON);
						else
							LED_CONTROL(LED_POWER, LED_OFF);
						led_count++;
					}
					else
					{
						btn_pressed_setup = BTNSETUP_NONE;
						btn_count_setup = 0;
						LED_CONTROL(LED_POWER, LED_ON);
						alarmtimer(NORMAL_PERIOD, 0);
						led_count = 0;
						wps_success = 1;

 			   			if (nvram_match("wps_unconfig_addenr", "1")) {
							nvram_set("wps_unconfig_addenr", "");
							nvram_set("wl_wps_reg", "disabled");
							nvram_set("wl0_wps_reg", "disabled");
							nvram_set("wl_wps_config_state", "0");
							nvram_set("wl0_wps_config_state", "0");
							nvram_set("wps_config_state", "0");
						}

						nvram_set("wps_proc_status", "0");
						nvram_set("wps_config_command", "2");
					
						if("wps_asus_mode", "1") {
							nvram_set("wps_device_pin", nvram_get("secret_code"));
							nvram_set("wps_asus_mode", "0");
						}

						if (wps_pbc_enable) {
							WSC_ENABLED = 0;
							stop_wps();
						}

						stop_wps_proc();
					}
				}
			}

			/* +++ Cherry Cho added for PBC overlap in 2008/2/25. +++ */

			if (nvram_match("wps_proc_status", "2")) { // WPS Success, Stop WPS process
				//nvram_set("wps_config_command", "2");
				if (wps_pbc_enable) {
					stop_wps();
					WSC_ENABLED = 0;
				}
				if("wps_asus_mode", "1") {
					nvram_set("wps_device_pin", nvram_get("secret_code"));
					nvram_set("wps_asus_mode", "0");
				}
				nvram_commit();
				stop_wps_proc();
			}

			if ((btn_pressed_setup==BTNSETUP_START) && !gpio_read("/dev/gpio/in", BTN_SETUP)
				&& nvram_match("wps_timer_start", "1")
				&& nvram_match("wps_waiting", "1")) {
				nvram_set("pbc_overlap", "1");	
			}

			/* --- Cherry Cho added for PBC overlap in 2008/2/25. --- */	
		}
#ifdef PBCENR
		else if (nvram_invmatch("sw_mode_ex", "1"))
		{
			if ( nvram_match("wps_proc_status","2") ||	/* WPS_OK */
			     nvram_match("wps_proc_status","3") ||	/* WPS_MSG_ERR */
			     nvram_match("wps_proc_status","4") ||	/* WPS_TIMEOUT */
			     nvram_match("wps_proc_status","8") )	/* WPS_PBCOVERLAP */
			{
				btn_pressed_setup = BTNSETUP_NONE;
				btn_count_setup = 0;
				LED_CONTROL(LED_POWER, LED_ON);
				alarmtimer(NORMAL_PERIOD, 0);
				led_count = 0;
			}
			else
			{
				++btn_count_setup;	
				btn_count_setup = btn_count_setup%4;

				if (btn_count_setup <= 1)
					LED_CONTROL(LED_POWER, LED_ON);
				else
					LED_CONTROL(LED_POWER, LED_OFF);
			}
		}
#endif
		else /* WSC is not enabled. Cherry Cho added in 2008/7/21. */
		{
			if ( led_count < led_count_timeout )
			{
				++btn_count_setup;	
				btn_count_setup = btn_count_setup%2;
				if (btn_count_setup ==1)
					LED_CONTROL(LED_POWER, LED_ON);
				else
					LED_CONTROL(LED_POWER, LED_OFF);
				led_count++;
			}
			else
			{
				btn_pressed_setup = BTNSETUP_NONE;
				btn_count_setup = 0;
				LED_CONTROL(LED_POWER, LED_ON);
				alarmtimer(NORMAL_PERIOD, 0);
				led_count = 0;
			}	
		}
#endif //WSC
	}
#endif
}

void refresh_ntpc(void)
{
#ifndef ASUS_EXT
	eval("killall","ntpclient");
#else
	kill_pidfile_s("/var/run/ntp.pid", SIGUSR1);
#endif
}

int ntp_timesync(void)
{
	time_t now;
	struct tm tm;	
	struct tm gm, local;
	struct timezone tz;

//	if (nvram_match("router_disable", "1")) return 0;
	
	if (sync_interval!=-1)
	{
		sync_interval--;

		if (sync_interval==0)
		{
			/* Update kernel timezone */
			setenv("TZ", nvram_safe_get("time_zone_x"), 1);
			time(&now);
			gmtime_r(&now, &gm);
			localtime_r(&now, &local);
			tz.tz_minuteswest = (mktime(&gm) - mktime(&local)) / 60;
			settimeofday(NULL, &tz);
		   	memcpy(&tm, localtime(&now), sizeof(struct tm));

		   	if (tm.tm_year > 100) // More than 2000 
		   	{	 
				sync_interval=60*60/5;
			  	logmessage("ntp client", "time is synchronized to %s", nvram_safe_get("ntp_servers"));
//				stop_upnp();
//				start_upnp();
		   	}	
		  	else sync_interval=1;
	
			refresh_ntpc();	
		}
	}	
}

enum 
{
	URLACTIVE=0,
	WEBACTIVE,
	RADIOACTIVE,
	ACTIVEITEMS
} ACTIVE;

int svcStatus[ACTIVEITEMS] = { -1, -1, -1};
int extStatus[ACTIVEITEMS] = { 0, 0, 0};
char svcDate[ACTIVEITEMS][10];
char *svcTime[ACTIVEITEMS][20];

int timecheck_item(char *activeDate, char *activeTime)
{
	#define DAYSTART (0)
	#define DAYEND (60*60*23+60*59+59) //86399
	int current, active, activeTimeStart, activeTimeEnd, i;
	time_t now;
	struct tm *tm;

	time(&now);
	tm = localtime(&now);
	current = tm->tm_hour*60 + tm->tm_min;

	active=0;

//	printf("active: %s %s\n", activeDate, activeTime);

	activeTimeStart=((activeTime[0]-'0')*10+(activeTime[1]-'0'))*60 + (activeTime[2]-'0')*10 + (activeTime[3]-'0');
		
	activeTimeEnd=((activeTime[4]-'0')*10+(activeTime[5]-'0'))*60 + (activeTime[6]-'0')*10 + (activeTime[7]-'0');
				
	if (activeDate[tm->tm_wday] == '1')
	{
		if (activeTimeEnd < activeTimeStart)
		{
			if ((current >= activeTimeStart && current <= DAYEND) ||
			   (current >= DAYSTART && current <= activeTimeEnd))
			{
				active = 1;
			}
			else
			{
				active = 0;
			}
		}
		else
		{
			if (current >= activeTimeStart && current <= activeTimeEnd)
			{	
				active = 1;
			}	
			else 
			{
				active = 0;
			}
		}			
	}
	return(active);
}

/* Check for time-dependent service 	*/
/* 1. URL filter 			*/
/* 2. WEB Camera Security filter 	*/
#if 1
int svc_timecheck(void)
{
	int activeFlag, activeNow;

	activeFlag = 0;

	/* Initialize */
	if (svcStatus[URLACTIVE]==-1 && nvram_invmatch("url_enable_x", "0"))
	{
		strcpy(svcDate[URLACTIVE], nvram_safe_get("url_date_x"));
		strcpy(svcTime[URLACTIVE], nvram_safe_get("url_time_x"));
		svcStatus[URLACTIVE] = -2;
	}	

	if (svcStatus[URLACTIVE]!=-1)
	{
		activeNow = timecheck_item(svcDate[URLACTIVE], svcTime[URLACTIVE]);	
		if (activeNow!=svcStatus[URLACTIVE])
		{
//			printf("url time change: %d\n", activeNow);
			svcStatus[URLACTIVE] = activeNow;
			stop_dns();
			start_dns();
		}
	}

	if (svcStatus[WEBACTIVE]==-1 && 
		nvram_invmatch("usb_webenable_x", "0") &&
		nvram_invmatch("usb_websecurity_x", "0"))
	{	
		strcpy(svcDate[WEBACTIVE], nvram_safe_get("usb_websecurity_date_x"));
		strcpy(svcTime[WEBACTIVE], nvram_safe_get("usb_websecurity_time_x"));
		svcStatus[WEBACTIVE] = -2;
	}

	if (svcStatus[WEBACTIVE]!=-1)
	{
		activeNow = timecheck_item(svcDate[WEBACTIVE], svcTime[WEBACTIVE]);
		if (activeNow!=svcStatus[WEBACTIVE])
		{
			svcStatus[WEBACTIVE] = activeNow;

//			if (!notice_rcamd(svcStatus[WEBACTIVE])) svcStatus[WEBACTIVE]=-1;
		}	
	}

	if (svcStatus[RADIOACTIVE]==-1 && nvram_invmatch("wl_radio_x", "0"))
	{	
		strcpy(svcDate[RADIOACTIVE], nvram_safe_get("wl_radio_date_x"));
		strcpy(svcTime[RADIOACTIVE], nvram_safe_get("wl_radio_time_x"));
		svcStatus[RADIOACTIVE] = -2;
	}


	if (svcStatus[RADIOACTIVE]!=-1)
	{
		activeNow = timecheck_item(svcDate[RADIOACTIVE], svcTime[RADIOACTIVE]);
		if (activeNow!=svcStatus[RADIOACTIVE])
		{
			svcStatus[RADIOACTIVE] = activeNow;

			if (activeNow)
			{
				eval("wl", "radio", "on");
				wl_led_ctrl(1);
			}
			else 
			{
				eval("wl", "radio", "off");
				wl_led_ctrl(0);
			}
		}
	}

	return 0;
}	
#endif
	
/* Sometimes, httpd becomes inaccessible, try to re-run it */
int http_processcheck(void)
{
//	char http_cmd[32];
	char buf[256];

//	sprintf(http_cmd, "http://127.0.0.1/");
	if (
#ifdef DLM
			!nvram_match("usb_storage_busy2", "1") && 
#endif
#ifdef ASUS_DDNS //2007.03.27 Yau add for prevent httpd die when doing hostname check
			!nvram_match("httpd_check_ddns", "1") &&
#endif
//			!http_check(http_cmd, buf, sizeof(buf), 0))
			!http_check(NULL, buf, sizeof(buf), 0))
	{
		dprintf("http rerun\n");
		kill_pidfile("/var/run/httpd.pid");
		if (nvram_match("httpd_die_reboot", "1")) {
			nvram_set("httpd_die_reboot", "");
			eval("reboot");
			
			return 0;
		}
//		stop_httpd();
		start_httpd();
	}
	
	/*if (nvram_invmatch("usb_webdriver_x", "") &&
			nvram_invmatch("usb_webdriver_x", "0"))
	{
//		sprintf(http_cmd, "http://127.0.0.1:%s/", nvram_safe_get("usb_webhttpport_x"));
//		logmessage("webcam", "webcam httpd die checking %s\n", http_cmd);
		
		if (
#ifdef DLM
				!nvram_match("usb_storage_busy2", "1") && 
#endif
#ifdef ASUS_DDNS //2007.03.27 Yau add for prevent httpd die when doing hostname check
				!nvram_match("httpd_check_ddns", "1") &&
#endif
//				!http_check(http_cmd, buf, sizeof(buf), 0))
				!http_check(NULL, buf, sizeof(buf), 0))
		{
			dprintf("http rerun\n");
			sprintf(buf, "/var/run/httpd-%s.pid", nvram_safe_get("usb_webhttpport_x"));
			kill_pidfile(buf);
//			logmessage("webcam", "webcam httpd rerun\n");
			
			chdir("/tmp/webcam");
			eval("httpd", nvram_safe_get("wan0_ifname"), nvram_safe_get("usb_webhttpport_x"));
			chdir("/");
		}
	}*/
	
	return 0;
}

/* While radius auth fails, wireless is down. We need to retry the auth */
int nas_processcheck()
{
	FILE *fp;
//	char cfgfile[64];
//	char pidfile[64];

//	snprintf(cfgfile, sizeof(cfgfile), "/tmp/nas.%s.conf", "lan");
//	snprintf(pidfile, sizeof(pidfile), "/tmp/nas.%s.pid", "lan");

	fp = fopen("/var/run/nas.pid", "r");
	if (fp != NULL) {
		fclose(fp);
		return 0;
	}

	char *argv[] = {"nas", NULL};
	pid_t pid;

	syslog(LOG_NOTICE, "Radius server authentication failed.");
	syslog(LOG_NOTICE, "Please check the settings of radius server are correct and the radius server is available.");
	syslog(LOG_NOTICE, "Router would try to authenticate again in seconds.");
	sleep(5);
	_eval(argv, NULL, 0, &pid);

	return 0;
}


#ifdef USB_SUPPORT
char usbinterrupt[128];

int rcamd_processcheck()
{
	FILE *fp;
	char buf[128];
	int flag=1;

	fp = fopen("/proc/interrupts", "r");

	if (fp!=NULL)
	{
		while(fgets(buf, sizeof(buf), fp))
		{
			if (strstr(buf, "ohci")) {}
		}
		fclose(fp);
	}
	return flag;		
}
#endif

int notice_rcamd(int flag)
{
	int rcamdpid=-1;
//	printf("Send signal : %d %d\n", rcamdpid, flag);
	if (rcamdpid==-1)
	{
		FILE *fp;

		if ((fp=fopen("/var/run/rcamd.pid","r"))!=NULL)
		{
			fscanf(fp,"%d", &rcamdpid);
			fclose(fp);
		}			
	}
	if (rcamdpid!=-1)
	{
		if (flag)
			kill(rcamdpid, SIGUSR1);
		else
			kill(rcamdpid, SIGUSR2);	

		return 1;
	}
	return 0;
}

int refresh_rcamd(void)
{
	FILE *fp;
	int rcamdpid=-1;

	if ((fp=fopen("/var/run/rcamd.pid","r"))!=NULL)
	{
		fscanf(fp,"%d", &rcamdpid);
		fclose(fp);
		unlink("/var/run/rcamd.pid");
		kill(rcamdpid, SIGUSR1);
	}			
	else 
	{	
//		eval("killall", "rcamd");
	}

	if ((fp=fopen("/var/run/rcamdmain.pid","r"))!=NULL)
	{
		fscanf(fp,"%d", &rcamdpid);
		fclose(fp);
		kill(rcamdpid, SIGUSR1);
	}		
	return 0;
}

int refresh_wave(void)
{
	FILE *fp;
	int wavepid=-1;

	eval("killall", "waveserver");

	if ((fp=fopen("/var/run/waveservermain.pid","r"))!=NULL)
	{
		fscanf(fp,"%d", &wavepid);
		fclose(fp);
		kill(wavepid, SIGUSR1);
	}			
	return 0;
}

static void catch_sig(int sig)
{
	if (sig == SIGUSR1)
	{
		dprintf("Catch Reset to Default Signal\n");
//		sys_default();
//		set_pid(getpid());	
	}
	else if (sig == SIGUSR2)
	{
		FILE *fp;
		char command[256], *cmd_ptr;

		dprintf("Get Signal: %d %d %d\n", svcStatus[WEBACTIVE], extStatus[WEBACTIVE], sig);

		if (!svcStatus[WEBACTIVE]) return;

		if (extStatus[WEBACTIVE]==0)
		{
			fp = fopen("/var/tmp/runshell", "r+");
			if (fp!=NULL)
			{
				cmd_ptr = fgets(command, 256, fp);

				if (cmd_ptr!=NULL) system(command);
			}
			fclose(fp);
			unlink("/tmp/runshell");
//			notice_rcamd(0);
			extStatus[WEBACTIVE]=1;
		}
		else if (svcStatus[WEBACTIVE]==1)
		{
//			notice_rcamd(1);
			extStatus[WEBACTIVE] = 0;
		}
	}		
#ifdef WSC	
	else if (sig == SIGTSTP)
	{
#ifdef PBCENR
		if (nvram_invmatch("sw_mode_ex", "1"))
			return;
#endif

		switch(atoi(nvram_safe_get("wps_sigtstp_case"))) {
			case 1:/* Cherry Cho added in 2008/7/2. */
				if ( btn_pressed_setup!=BTNSETUP_START && 
					nvram_match("wps_client_role", "enrollee") && 
					!strcmp(nvram_safe_get("wps_method"), "2") )
				{
					btn_pressed_setup=BTNSETUP_START;
					btn_count_setup=0;
					alarmtimer(0, RUSHURGENT_PERIOD);
					btn_addER = 1;
				}
				break;
			
			case 2:/* WPS process successfully completes. Added for stopping LED twinkle. Cherry Cho added in 2008/2/12. */
				if ( ( btn_pressed_setup==BTNSETUP_START )/* Reset variables when WSC process terminates. */ 
				&& nvram_match("wps_proc_status","2") )
				{
					btn_pressed_setup = BTNSETUP_NONE;
					btn_count_setup = 0;
					LED_CONTROL(LED_POWER, LED_ON);
					alarmtimer(NORMAL_PERIOD, 0);
					if (btn_addER)	
						btn_addER = 0;	
					wps_success = 1;	
				}
				break;
				
			case 3:/* Restart nas. Cherry Cho added in 2008/7/8. */
				syslog(LOG_NOTICE, "WSC: send signal to watchdog and restart nas.");
				eval("wlconf", "eth1", "up");
				eval("wlconf", "eth1", "start");
				start_wps();
//				start_wsc();
//				start_nas("lan");
				start_nas();
				break;	
				
			case 4:/* WPS process fails to complete. Cherry Cho added in 2008/7/16.*/	
				if (( btn_pressed_setup==BTNSETUP_START )/* Reset variables when WSC process terminates. */ 
				&& ( nvram_match("wps_proc_status","3") || nvram_match("wps_proc_status","4") ))
				{
					if (btn_addER)	
						btn_addER = 0;
					wps_success = 0;		  	
	 	   		}	   	
	 	   		break;
	 	   		
			default:
				break;
		}
	}		
#endif
}

void sta_check(void)
{
	int ret;
	char *wl_ifname=nvram_safe_get("wl0_ifname");
	char bssid[32];

	if (stacheck_interval==-1)
	{
		if (nvram_invmatch("wl0_mode", "sta") && 
			nvram_invmatch("wl0_mode", "wet")) return;

		stacheck_interval=STACHECK_PERIOD_DISCONNECT;
	}
	
	stacheck_interval--;

	if (stacheck_interval)
		return;

	// Get bssid
	ret=wl_ioctl(wl_ifname, WLC_GET_BSSID, bssid, sizeof(bssid));
	if (ret==0 && !(bssid[0]==0&&bssid[1]==0&&bssid[2]==0
		&&bssid[3]==0&&bssid[4]==0&&bssid[5]==0))
	{
		dprintf("connected\n");
		stacheck_interval=STACHECK_PERIOD_CONNECT;
	}
	else 
	{
		dprintf("disconnected\n");
		stacheck_interval=STACHECK_PERIOD_DISCONNECT;
		dprintf("connect: [%s]\n", nvram_safe_get("wl0_join"));
		system(nvram_safe_get("wl0_join"));
//		eval("wl", "join", nvram_safe_get("wl0_ssid"));
	}
	return;
}

int block_dm_count = 0;
void dm_block_chk()
{
        if(nvram_match("dm_block", "1"))
                ++block_dm_count;
        else
                block_dm_count = 0;

        if(block_dm_count > 20) // 200 seconds
        {
                block_dm_count = 0;
                nvram_set("dm_block", "0");
                printf("[wd] unblock dm execution\n");  // tmp test
        }
}

#ifdef RTN12
void
sw_gpio_init()
{
        gpio_write("/dev/gpio/outen", BTN_SW_1, 0);
        gpio_write("/dev/gpio/outen", BTN_SW_2, 0);
        gpio_write("/dev/gpio/outen", BTN_SW_3, 0);
}

int	pre_sw_mode=0, sw_mode=0;
int	flag_sw_mode=0;
int	tmp_sw_mode=0;
int	count_stable=0;

void 
sw_mode_check()
{
	pre_sw_mode = sw_mode;

	if (gpio_read("/dev/gpio/in", BTN_SW_1))
		sw_mode = 1;
	else if (gpio_read("/dev/gpio/in", BTN_SW_2))
		sw_mode = 2;
	else if (gpio_read("/dev/gpio/in", BTN_SW_3))
		sw_mode = 3;

	if(sw_mode != pre_sw_mode)
	{
		char tmp[10];
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", sw_mode);
		nvram_set("sw_mode", tmp);

		if (nvram_invmatch("sw_mode", nvram_safe_get("sw_mode_ex")) && nvram_invmatch("asus_mfg", "1"))
		{	/* different from working mode */
			flag_sw_mode=1;
			count_stable=0;
			tmp_sw_mode=sw_mode;
		}
		else
			flag_sw_mode=0;
	}
	else if (flag_sw_mode==1 && nvram_invmatch("asus_mfg", "1"))
	{
		if (tmp_sw_mode==sw_mode)
		{
			if (++count_stable > 4)	// stable for more than 5 seconds
			{
				fprintf(stderr, "Reboot to switch sw mdoe...\n");
				flag_sw_mode=0;
				kill(1, SIGTERM);
			}
		}
		else
			flag_sw_mode=0;
	}
}
#endif

/* wathchdog is runned in NORMAL_PERIOD, 1 seconds
 * check in each NORMAL_PERIOD
 *	1. button
 *
 * check in each NORAML_PERIOD*5
 *
 *	1. ntptime, 
 *	2. time-dependent service
 *	3. http-process
 *	4. usb hotplug status
 */
void watchdog(void)
{
	time_t now;

	/* handle button */
	btn_check();

#ifdef RTN12
	sw_mode_check();
#endif

#ifdef WCN
	if (nvram_match("reboot_WCN", "1"))
	{
		reboot_WCN_count++;

		if (reboot_WCN_count >= 50)
			kill(1, SIGTERM);
		else
			return;
	}
	else if (nvram_match("reboot_WCN", "2"))
	{
		nvram_commit();
		alarmtimer(0, QUICK_PERIOD);
		wcn_on=1;
		nvram_set("reboot_WCN", "1");
		return;
	}
#endif

	/* if timer is set to less than 1 sec, then bypass the following */
	if (itv.it_value.tv_sec==0) return;
#if 0		
	if (nvram_match("eject_from_web", "1"))
	{
		nvram_set("eject_from_web", "0");
		eval("run_ftpsamba");
	}
#endif
	// reboot signal checking
	if (nvram_match("reboot", "1"))
	{
		reboot_count++;
		if (reboot_count >= 2) kill(1, SIGTERM);
	}
	else if (nvram_match("reboot", "0")) return;

	watchdog_period = (watchdog_period+1)%10;

	if (watchdog_period) return;

	dm_block_chk();

	time(&now);

#ifdef BTN_SETUP
	if (btn_pressed_setup >= BTNSETUP_START) return;
#endif
//	printf("now: %d\n", (long )now);
	/* sync time to ntp server if necessary */
	if (nvram_invmatch("wan_ipaddr_t", ""))
	ntp_timesync();

	/* check for time-dependent services */
	svc_timecheck();

	/* http server check */
#ifdef DLM
	if (!nvram_match("usb_storage_busy2", "1"))
#endif
		http_processcheck();

	/* nas check for radius fail auth */
#if 0
	if (int_nas_enable) {
		if (nas_check_interval > 7) {
			nas_check_interval = 0;
			nas_processcheck();
		}
		else
			++nas_check_interval;
	}
#endif
#ifdef USB_SUPPORT

	/* web cam process */
	if (!nvram_match("usb_web_device", ""))
	{	
		if (!nvram_match("usb_webdriver_x", ""))
		{
			if (!rcamd_processcheck())
			{
//				refresh_rcamd();
			}
		}				
		else
		{	
//			hotplug_usb_webcam(nvram_safe_get("usb_web_device"), atoi(nvram_safe_get("usb_web_flag")));
//			nvram_set("usb_web_device", "");
//			nvram_set("usb_web_flag", "");
			// reset WEBCAM status	
			refresh_rcamd();
			svcStatus[WEBACTIVE] = -1;
		}
	}

	/* storage process */
// 2007.12 James {
	char *remove_disk = nvram_safe_get("usb_storage_device_remove");
	char *add_disk = nvram_safe_get("usb_storage_device");
	
	if (strlen(remove_disk) > 0) {
printf("*** start to remove usb disk. ***\n");
		nvram_set("usb_storage_busy", "1");	// 2007.12 James
		remove_usb_mass(remove_disk);
		nvram_set("usb_storage_busy", "0");	// 2007.12 James
printf("*** end to remove usb disk. ***\n");
	}
	else if (strlen(add_disk) > 0) {
printf("*** start to add usb disk. ***\n");
		nvram_set("usb_storage_busy", "1");	// 2007.12 James
		hotplug_usb_mass(add_disk);
		nvram_set("usb_storage_busy", "0");	// 2007.12 James
printf("*** end to add usb disk. ***\n");
	}
// 2007.12 James }

#endif

	/* station or ethernet bridge handler */
//	sta_check();

#ifdef CDMA
	/* CDMA_DOWN = 99, none
	 * CDMA_DOWN = 1, currently down
	 * CDMA_DOWN = 2, currently up
	 * CDMA_DOWN = 0, currently trying to connect
	 */
	if (nvram_match("cdma_down", "1"))
	{
		logmessage("CDMA client", "cdma is down(%d)!", cdma_down);

		cdma_down++;
		cdma_connect=0;
		
		if (cdma_down==2)
		{
			stop_wan();
			start_wan();
		}
		else if (cdma_down >= 12) /* 2 minutes timeout for retry */
		{
			cdma_down=0;
		}
	}
	else if (nvram_match("cdma_down", "0"))
	{
		logmessage("CDMA client", "cdma try connect(%d)!", cdma_connect);
		cdma_down=0;
		cdma_connect++;

		if (cdma_connect > 12) /* 2 minitues timeout for connecting */
		{
			nvram_set("cdma_down", "1");
		}
	}
	else
	{
		cdma_down=0;
		cdma_connect=0;
	}
#endif
}

int
gpio_main(int ledin)
{
#ifdef BTN_SETUP
	printf("BTN:%d,%d\n", gpio_read("/dev/gpio/in", BTN_RESET), gpio_read("/dev/gpio/in", BTN_SETUP));
#else
	printf("BTN:%d,0\n", gpio_read("/dev/gpio/in", BTN_RESET));
#endif
}

int 
watchdog_main(int argc, char *argv[])
{
	FILE *fp;
#ifdef WSC/* Cherry Cho added in 2008/7/16. */	
#ifdef PBCENR
	WSC_ENABLED = nvram_match("sw_mode_ex", "1") && nvram_match("wps_mode", "enabled");
#else
	WSC_ENABLED = nvram_match("wps_mode", "enabled");
#endif
#endif	
	
#ifdef REMOVE
	/* Run it under background */
	switch (fork()) {
	case -1:
		exit(0);
		break;
	case 0:	
		// start in a new session	
		(void) setsid();
		break;
	default:	
		/* parent process should just die */
		_exit(0);
	}
#endif

	/* write pid */
	if ((fp=fopen("/var/run/watchdog.pid", "w"))!=NULL)
	{
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}

	/* set the signal handler */
	signal(SIGUSR1, catch_sig);
	signal(SIGUSR2, catch_sig);
	signal(SIGTSTP, catch_sig);/* Cherry Cho added in 2008/2/12 */
	signal(SIGALRM, watchdog);

	/* Start GPIO function */
	gpio_init();

	/* Start POWER LED */
	LED_CONTROL(LED_POWER, LED_ON);

	/* Start sync time */
	sync_interval=1;
	start_ntpc();

#ifdef BTN_SETUP
	if (!nvram_match("sharedkeystr", ""))
	{
		printf("Debug::sharedkeystr=%s\n", nvram_get("sharedkeystr"));
		btn_pressed_flag++;
		btn_stage=1;
	}
	nvram_set("bs_mode", "");	
#endif

	/* nas check for radius fail auth */
	if (nvram_match("wl0_akm", "wpa" )	// WPA-Enterprise
			|| nvram_match("wl0_akm", "wpa2" )	// WPA2-Enterprise
			|| nvram_match("wl0_akm", "wpa wpa2" )	// WPA-Auto-Enterprise
			|| nvram_match("wl0_auth_mode", "radius" ) // Radius with 802.1x
			)
		int_nas_enable = 1;

	/* set timer */
	alarmtimer(NORMAL_PERIOD, 0);

	/* Most of time it goes to sleep */
	while(1)
	{
		pause();
	}
	
	return 0;
}

#if 1
void wl_led_ctrl(int ctrl)
{
	int led;
	if (!ctrl)
	{
		gpio_write("/dev/gpio/out", LED_RADIO, ctrl);
		led=0;
		wl_ioctl(nvram_get("wl0_ifname"), WLC_SET_LED, &led, sizeof(led));
	}
	else
	{
		tgpio_fd_init();
		tgpio_ioctl(BCMGPIO_REG_RELEASE, LED_RADIO, LED_RADIO);
		tgpio_fd_cleanup();
	}
}
#endif

int radio_main(int ctrl)
{
	if (ctrl)
	{
		eval("wl", "radio", "on");
		wl_led_ctrl(1);
	}
	else
	{
		eval("wl", "radio", "off");
		wl_led_ctrl(0);
	}
}
/*
int blink_all_led()
{
	eval("rmmod", "et");
	eval("insmod", "et");
	wl_ioctl("eth1", WLC_SET_VAR, AIR_LED_ON, sizeof(AIR_LED_ON));
	LED_CONTROL(LED_POWER, LED_ON);
	usleep(1200 * 1000);
	wl_ioctl("eth1", WLC_SET_VAR, AIR_LED_OFF, sizeof(AIR_LED_OFF));
	LED_CONTROL(LED_POWER, LED_OFF);
	usleep(666 * 1000);
	wl_ioctl("eth1", WLC_SET_VAR, AIR_LED_ON, sizeof(AIR_LED_ON));
	LED_CONTROL(LED_POWER, LED_ON);
	eval("rc", "restart");
}
*/
#if 0
static int
write_to_wps(int fd, char *cmd)
{
        int n;
        int len;
        struct sockaddr_in to;

        len = strlen(cmd)+1;

        /* open loopback socket to communicate with wps */
        memset(&to, 0, sizeof(to));
        to.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        to.sin_family = AF_INET;
        to.sin_port = htons(WPS_UI_PORT);

        n = sendto(fd, cmd, len, 0, (struct sockaddr *)&to,
                sizeof(struct sockaddr_in));

        /* Sleep 100 ms to make sure
 *            WPS have received socket */
        USLEEP(100*1000);
        return n;
}

static int
read_from_wps(int fd, char *databuf, int datalen)
{
        int n, max_fd = -1;
        fd_set fdvar;
        struct timeval timeout;
        int recvBytes;
        struct sockaddr_in addr;
        socklen_t size = sizeof(struct sockaddr);

        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        FD_ZERO(&fdvar);

        /* get ui fd */
        if (fd >= 0) {
                FD_SET(fd, &fdvar);
                max_fd = fd;
        }

        if (max_fd == -1) {
                fprintf(stderr, "wps ui utility: no fd set!\n");
                return -1;
        }

        n = select(max_fd + 1, &fdvar, NULL, NULL, &timeout);

        if (n < 0) {
                return -1;
        }

        if (n > 0) {
                if (fd >= 0) {
                        if (FD_ISSET(fd, &fdvar)) {
                                recvBytes = recvfrom(fd, databuf, datalen,
                                        0, (struct sockaddr *)&addr, &size);

                                if (recvBytes == -1) {
                                        fprintf(stderr,
                                        "wps ui utility:recv failed, recvBytes = %d\n", recvBytes);
                                        return -1;
                                }

                                return recvBytes;
                        }

                        return 0;
                }
        }

        return -1;
}

int
parse_wps_env(char *buf)
{
        char *argv[32] = {0};
        char *item, *p, *next;
        char *name, *value;
        int i;
        int unit, subunit;
        char nvifname[IFNAMSIZ];

        /* Seperate buf into argv[] */
        for (i = 0, item = buf, p = item;
                item && item[0];
                item = next, p = 0, i++) {
                /* Get next token */
                strtok_r(p, " ", &next);
                argv[i] = item;
        }

        /* Parse message */
        wps_config_command = 0;
        wps_action = 0;

        for (i = 0; argv[i]; i++) {
                value = argv[i];
                name = strsep(&value, "=");
                if (name) {
                        if (!strcmp(name, "wps_config_command"))
                                wps_config_command = atoi(value);
                        else if (!strcmp(name, "wps_action"))
                                wps_action = atoi(value);
                        else if (!strcmp(name, "wps_uuid"))
                                strcpy(wps_uuid ,value);
                        else if (!strcmp(name, "wps_ifname")) {
                                if (strlen(value) <=0)
                                        continue;
                                if (osifname_to_nvifname(value, nvifname, sizeof(nvifname)) != 0)
                                        continue;

                                if (get_ifname_unit(nvifname, &unit, &subunit) == -1)
                                        continue;
                                if (unit < 0)
                                        unit = 0;
                                if (subunit < 0)
                                        subunit = 0;

                                if (subunit)
                                        sprintf(wps_unit, "%d.%d", unit, subunit);
                                else
                                        sprintf(wps_unit, "%d", unit);
                        }
                }
        }

        return 0;
}

static int
get_wps_env()
{
        int fd = -1;
        char databuf[256];
        int datalen = sizeof(databuf);
        if ((fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
                fprintf(stderr, "wps ui utility: failed to open loopback socket\n");
                return -1;
        }

        write_to_wps(fd, "GET");

        /* Receive response */
        if (read_from_wps(fd, databuf, datalen) > 0)
                parse_wps_env(databuf);
        else
                /* Show error message ? */

        close(fd);
        return 0;
}


static int
set_wps_env(char *uibuf)
{
        int wps_fd = -1;
        struct sockaddr_in to;
        int sentBytes = 0;
        uint32 uilen = strlen(uibuf);

        if ((wps_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {

                goto exit;
        }

        /* send to WPS */
        to.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        to.sin_family = AF_INET;
        to.sin_port = htons(WPS_UI_PORT);

        sentBytes = sendto(wps_fd, uibuf, uilen, 0, (struct sockaddr *) &to,
                sizeof(struct sockaddr_in));

        if (sentBytes != uilen) {
                goto exit;
        }

        /* Sleep 100 ms to make sure
 *            WPS have received socket */
        USLEEP(100*1000);
        return 0;
exit:
        if (wps_fd > 0)
                close(wps_fd);

        /* Show error message ?  */
        return -1;
}
#endif
