/* util.c ....... error message utilities.
 *                C. Scott Ananian <cananian@alumni.princeton.edu>
 *
 * $Id: util.c,v 1.1.1.1 2008/07/21 09:19:28 james26_jang Exp $
 */

#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <stdlib.h>
#include "util.h"

#ifndef PROGRAM_NAME
#define PROGRAM_NAME "pptp"
#endif

/* implementation of log_string, defined as extern in util.h */
char *log_string = "anon";

static void open_log(void) __attribute__ ((constructor));
static void close_log(void) __attribute__ ((destructor));

#define MAKE_STRING(label) 				\
va_list ap;						\
char buf[256], string[256];				\
va_start(ap, format);					\
vsnprintf(buf, sizeof(buf), format, ap);		\
snprintf(string, sizeof(string), "%s",	buf);		\
va_end(ap)

/*** open log *****************************************************************/
static void open_log(void) {
    openlog(PROGRAM_NAME, LOG_PID, LOG_DAEMON);
}

/*** close log ****************************************************************/
static void close_log(void)
{
    closelog();
}

/*** print a message to syslog ************************************************/
void _log(const char *func, const char *file, int line, const char *format, ...)
{
    if (log_level > 0) {
        MAKE_STRING("log");
        syslog(LOG_NOTICE, "%s", string);
    }
}

/*** print a warning to syslog ************************************************/
void _warn(const char *func, const char *file, int line, const char *format, ...)
{
    MAKE_STRING("warn");
    fprintf(stderr, "%s\n", string);
    syslog(LOG_WARNING, "%s", string);
}

/*** print a fatal warning to syslog and exit *********************************/
void _fatal(const char *func, const char *file, int line, const char *format, ...)
{
    MAKE_STRING("fatal");
    fprintf(stderr, "%s\n", string);
    syslog(LOG_CRIT, "%s", string);
    exit(1);
}

/*** connect a file to a file descriptor **************************************/
int file2fd(const char *path, const char *mode, int fd)
{
    int ok = 0;
    FILE *file = NULL;
    file = fopen(path, mode);
    if (file != NULL && dup2(fileno(file), fd) != -1)
        ok = 1;
    if (file) fclose(file);
    return ok;
}
