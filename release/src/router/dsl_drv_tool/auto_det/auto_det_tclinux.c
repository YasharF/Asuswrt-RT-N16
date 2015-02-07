#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include <bcmnvram.h>
#include "../msg_define.h"
#include "shared.h"
#include "shutils.h"

#define WAIT_TP_INIT	300
#define WAIT_AUTO_DET	120
static int m_exit = 0;
static int m_msqid_to_d = 0;
static int m_msqid_from_d = 0;

void handlesigterm()
{
	m_exit = 1;
	return;
}

void TestAdslAteAskQuit(void)
{
	// if adslate want to quit auto det
	if( nvram_match("dsltmp_adslatequit", "1"))
		m_exit = 1;
	return;
}

int start_auto_det(void)
{
	msgbuf send_buf;
	msgbuf receive_buf;
	int* pInt;

	if ((m_msqid_from_d = msgget(IPC_PRIVATE, 0700)) < 0) {
		perror("start_auto_det: msgget");
		return -1;
	}

	send_buf.mtype=IPC_CLIENT_MSG_Q_ID;
	pInt = (int*)send_buf.mtext;      
	*pInt = m_msqid_from_d;
	if(msgsnd(m_msqid_to_d, &send_buf, MAX_IPC_MSG_BUF, 0) < 0) {
		perror("start_auto_det: msgsnd");
		return -1;
	}

	send_buf.mtype=IPC_START_AUTO_DET;
	strcpy(send_buf.mtext,"startdet");
	if(msgsnd(m_msqid_to_d,&send_buf, MAX_IPC_MSG_BUF, 0) < 0) {
		perror("start_auto_det: msgsnd2");
		return -1;
	}
	else {
		while (!m_exit) {
			if(msgrcv(m_msqid_from_d, &receive_buf, MAX_IPC_MSG_BUF, 0, 0) < 0) {
				if (errno == EINTR) {
					continue;
				}
				else {
					perror("stop_auto_det: msgrcv");
					break;
				}
			}
			else {
				printf("%s\n", receive_buf.mtext);
				break;
			}
		}
	}

	return 0;
}

int stop_auto_det(void)
{
	msgbuf send_buf;
	msgbuf receive_buf;
	struct msqid_ds stMsgInfo;

	send_buf.mtype=IPC_STOP_AUTO_DET;
	strcpy(send_buf.mtext,"stopdet");

	if(msgsnd(m_msqid_to_d, &send_buf, MAX_IPC_MSG_BUF, 0) < 0) {
		perror("stop_auto_det: msgsnd");
	}
	else {
		while (!m_exit) {
			if(msgrcv(m_msqid_from_d, &receive_buf, MAX_IPC_MSG_BUF, 0, 0) < 0) {
				if (errno == EINTR) {
					continue;
				}
				else {
					perror("stop_auto_det: msgrcv");
					break;
				}
			}
			else {
				printf("%s\n", receive_buf.mtext);
				break;
			}
		}
	}

	// delete msg queue from kernel
	msgctl(m_msqid_from_d, IPC_RMID, &stMsgInfo);

	printf("exit auto_det\n");
	return 0;
}

int auto_det_loop(void)
{
	int cnt = WAIT_AUTO_DET;
	msgbuf send_buf;
	msgbuf receive_buf;

	while(!m_exit && cnt) {
		if(--cnt % 5) {
			sleep(1);
			continue;
		}

		send_buf.mtype=IPC_GET_AUTO_DET_RESULT;
		if(msgsnd(m_msqid_to_d, &send_buf, MAX_IPC_MSG_BUF, 0) < 0) {
			perror("start_auto_det: msgsnd");
		}
		else {
			while (!m_exit) {
				if(msgrcv(m_msqid_from_d, &receive_buf, MAX_IPC_MSG_BUF, 0, 0) < 0) {
					if (errno == EINTR) {
						perror("auto_det_loop: msgrcv");
						sleep(1);
						continue;
					}
					else {
						perror("auto_det_loop: msgrcv");
						break;
					}
				}
				else {
					if(!strcmp(receive_buf.mtext, "Done")) {
						stop_auto_det();
						return 0;
					}
					else
						break;
				}
			}
		}
		sleep(1);
		TestAdslAteAskQuit();
	}

	stop_auto_det();
	return -1;
}

int main(int argc, char* argv[])
{
	int tp_init_ready = 0;
	int cnt = WAIT_TP_INIT;
	char buf[32];
	sigset_t sigs_to_catch;
	msgbuf send_buf;

	sigemptyset(&sigs_to_catch);
	sigaddset(&sigs_to_catch, SIGTERM);
	sigprocmask(SIG_UNBLOCK, &sigs_to_catch, NULL);

	if (signal(SIGTERM, handlesigterm) == SIG_ERR)
	{
		dbg("Cannot handle SIGTERM!\n");
		return -1;
	}

	// wait tp_init
	while (!m_exit && cnt) {
		if(nvram_match("dsltmp_tcbootup", "1")) {
			tp_init_ready = 1;
			break;
		}
		sleep(1);
		cnt--;
		TestAdslAteAskQuit();
	}
	if(!tp_init_ready) {
		dbg("Tp_init is not ready.\n");
		return -1;
	}

	strcpy(buf, nvram_safe_get("dsltmp_tc_resp_to_d"));
	m_msqid_to_d = atoi(buf);

	start_auto_det();
	send_buf.mtype=IPC_INIT_AUTO_DET;
	if(msgsnd(m_msqid_to_d, &send_buf, MAX_IPC_MSG_BUF, 0) < 0) {
		perror("auto_det_main: msgsnd");
		return -1;
	}
	//stop_auto_det();

	return auto_det_loop();
}
