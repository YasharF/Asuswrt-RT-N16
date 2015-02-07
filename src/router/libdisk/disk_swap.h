#ifndef _DISK_SWAP_
#define _DISK_SWAP_

#define SWAP_MIN 1
#define SWAP_MAX 1024*1024

int start_swap(const char *, const int);
int stop_swap_from_proc();
int stop_swap(const char *);
int run_swap(const int);
int swap_check();
extern int do_swap_for_format(const char *);

#endif // _DISK_SWAP_
