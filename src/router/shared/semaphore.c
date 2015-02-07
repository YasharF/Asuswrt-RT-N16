#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "semaphore_mfp.h"

int semaphore_create()
{
	int flag_sem = O_CREAT|O_RDWR;
	int init_value = 1;

	semaphore_unlink(SEM_NAME);
	
	if(semaphore_open(SEM_NAME, flag_sem, FILE_MODE, init_value) == -1)
	{
		perror("semaphore_create fail");
		return -1;
	}

	return 0;
}

int semaphore_close()
{
	if (Semaphore.sem_magic != SEM_MAGIC)
	{
		errno = EINVAL;
		return -1;
	}

	Semaphore.sem_magic = 0;	// in case caller tries to use it later
	if (close(Semaphore.sem_fd[0]) == -1 || close(Semaphore.sem_fd[1]) == -1)
	{
		memset(&Semaphore, '\0', sizeof(semaphore_t));
		return -1;
	}

	memset(&Semaphore, '\0', sizeof(semaphore_t));
	return 0;
}

int semaphore_open(const char *pathname, int oflag, ... )
{
	int	i, flags, save_errno;
	char	c;
	mode_t	mode;
	va_list	ap;
	unsigned int	value = 0;
                                                                                                                                               
	if (oflag & O_CREAT)
	{
		va_start(ap, oflag);			// init ap to final named argument
		mode = va_arg(ap, mode_t);
		value = va_arg(ap, unsigned int);
		va_end(ap);

		if (mkfifo(pathname, mode) < 0)
		{
			if (errno == EEXIST && (oflag & O_EXCL) == 0)
				oflag &= ~O_CREAT;      // already exists, OK 
			else
			{
				perror("Sen_open: mkfifo fail");
				return -1;
			}
		}
	}

	Semaphore.sem_fd[0] = Semaphore.sem_fd[1] = -1;

	if ( (Semaphore.sem_fd[0] = open(pathname, O_RDONLY | O_NONBLOCK)) < 0)
		goto error;
	if ( (Semaphore.sem_fd[1] = open(pathname, O_WRONLY | O_NONBLOCK)) < 0)
		goto error;
	if ( (flags = fcntl(Semaphore.sem_fd[0], F_GETFL, 0)) < 0)
		goto error;
	flags &= ~O_NONBLOCK;
	if (fcntl(Semaphore.sem_fd[0], F_SETFL, flags) < 0)
		goto error;
	if (oflag & O_CREAT)	// initialize semaphore
	{
		for (i = 0; i < value; i++)
			if (write(Semaphore.sem_fd[1], &c, 1) != 1)
				goto error;
	}

	Semaphore.sem_magic = SEM_MAGIC;
	return 0;

error:
	save_errno = errno;
	if (oflag & O_CREAT)
		unlink(pathname);		// if we created FIFO
	close(Semaphore.sem_fd[0]);		// ignore error
	close(Semaphore.sem_fd[1]);		// ignore error

	memset(&Semaphore, '\0',sizeof(semaphore_t));
	errno = save_errno;
	perror("semaphore_open error");

	return -1;
}

int semaphore_unlink(const char *pathname)
{
	return unlink(pathname);
}

int semaphore_post()
{
	char	c;

	if (Semaphore.sem_magic != SEM_MAGIC)
	{
		errno = EINVAL;
		return -1;
	}
                                                                                                                                               
	if (write(Semaphore.sem_fd[1], &c, 1) == 1)
		return 0;

        return -1;
}

int semaphore_wait()
{
	char	c;

	if (Semaphore.sem_magic != SEM_MAGIC)
	{
		errno = EINVAL;
		return -1;
	}

	if (read(Semaphore.sem_fd[0], &c, 1) == 1)
		return 0;

        return -1;
}
