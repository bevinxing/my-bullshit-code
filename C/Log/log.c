
#include <stdio.h>
#include <sys/time.h>   /* gettimeofday */
#include <stdarg.h>     /* va_list */
#include <syslog.h>     /* syslog */

#include "log.h"

int G_log_level = APP_DEFAULT_LOG_LEVEL;
int G_debug = 0;
int G_debug_level = APP_DEFAULT_DEBUG_LEVEL;

void _log_open(const char *ident, int debug, int debug_level, int log_level, int option, int facility) {
    G_debug = debug;
    G_debug_level = debug_level;
    G_log_level = log_level;
    openlog(ident, option, facility);
}

void _log_close() {
    closelog();
}

/* log input with raw string without length limit. */
void _log_raw(int level, const char *msg) {
    const int syslogLevelMap[] = { LOG_DEBUG, LOG_INFO, LOG_NOTICE, LOG_WARNING, LOG_ERR };
    char buf[64] = { 0 };

    if (G_debug && ((level & 0xff) >= G_debug_level)) {
        int off;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        off = strftime(buf, sizeof(buf), "%d %b %H:%M:%S.", localtime(&tv.tv_sec));
        snprintf(buf+off, sizeof(buf)-off, "%03d", (int)tv.tv_usec/1000);
        printf("%s: [STAINFO_MANAGER] %s\n", buf, msg);
    }

    if ((level & 0xff) >= G_log_level)
        syslog(syslogLevelMap[level], "%s", msg);
}

/* log input with format and length limit. */
void _log(int level, const char *fmt, ...) {
    va_list ap;
    char msg[MAX_LOGMSG_LEN] = { 0 };

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    _log_raw(level, msg);
}

