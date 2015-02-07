#ifndef __SEMAPHORE_MFP_H
#define __SEMAPHORE_MFP_H

#pragma pack(1)

#define SEM_MAGIC       0x89674523
#define SEM_NAME	"/tmp/Semaphore"
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

typedef struct
{
        int sem_fd[2];
        int sem_magic;
}semaphore_t;

semaphore_t Semaphore;

int  semaphore_create();
int  semaphore_close();
int  semaphore_open(const char *, int, ... );
int  semaphore_unlink(const char *);
int  semaphore_post();
int  semaphore_wait();

#pragma pack()

#endif
