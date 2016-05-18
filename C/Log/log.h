#ifndef __LOG_H__
#define __LOG_H__

#include <syslog.h>

#define APP_DEBUG 0
#define APP_INFO 1
#define APP_NOTICE 2
#define APP_WARNING 3
#define APP_ERROR 4
#define APP_DEFAULT_LOG_LEVEL APP_NOTICE
#define APP_DEFAULT_DEBUG_LEVEL APP_INFO

#define APP_SYSLOG_OPT LOG_PID

#define APP_SYSLOG_FACILITY LOG_DAEMON

#define MAX_LOGMSG_LEN    1024 /* Default maximum length of syslog messages */

#define MAC_FORMAT  "%02X:%02X:%02X:%02X:%02X:%02X"
#define MAC_ADDR(mac) mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]

void _log_open(const char *ident, int debug, int debug_level, int log_level, int option, int facility);
void _log_close();
void _log_raw(int level, const char *msg);
void _log(int level, const char *fmt, ...);

#endif /* __LOG_H__ */
