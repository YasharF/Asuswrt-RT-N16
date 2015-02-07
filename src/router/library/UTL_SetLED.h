#ifndef _UTL_SETLED_
#define _UTL_SETLED_
				/* cmd */
#define CMD_STARTUP       1	/* -startup */
#define CMD_LOADOK        2	/* -loadok (merge into ready) */
#define CMD_READY         3	/* -ready */
#define CMD_SHUTDOWN      4	/* -shutdown */
#define CMD_UPGRADE       5	/* -upgrade */
#define CMD_NOUPGRADE     6	/* -noupgrade */
#define CMD_DISKFULL      7	/* -diskfull */
#define CMD_GETIPFAIL     8	/* -getipfail */
#define CMD_NOGETIPFAIL   9	/* -nogetipfail */
#define CMD_FIXIP         10	/* -fixip */
#define CMD_NOFIXIP       11	/* -nofixip */
#define CMD_PUSHBUTTON    12	/* -pushbutton */
#define CMD_NOPUSHBUTTON  13	/* -nopushbutton */
#define CMD_HWERROR       14	/* -printporterror */
#define CMD_NOHWERROR     15	/* -noprintporterror */
#define CMD_THERMAL       16	/* -thermal */
#define CMD_LOWTHERMAL    17	/* -lowthermal */
#define CMD_INITBEEP      18	/* -initbeep (merge into startup)*/
#define CMD_CHECKHD       19	/* -checkhd */
#define CMD_NOCHECKHD     20	/* -nocheckhd */
#define CMD_BACKUP        21	/* -backup */
#define CMD_NOBACKUP      22	/* -nobackup */
#define CMD_SETBACKUP     23	/* -setbackup */
#define CMD_NOSETBACKUP   24	/* -nosetbackup */

				/* Return Code */
#define LEDSTATUS_EINVALID  -1	/* Invalid Argument */

/********************************************************************
 * Main Function:                                                   *
 * Read input option and do the action.                             *
 * Input:                                                           *
 *   cmd:      The Option (ARGV_xxxxxx) on above declarations       *
 *   argument: The argument for the input cmd. (DiskFull..)         *
 *             If no need, put LEDSTATUS_EINVALID.                  *
 *             (Function will discard it.)                          *
 ********************************************************************/
int act_setled(int cmd, int argument);

#endif /* _UTL_SETLED_ */
