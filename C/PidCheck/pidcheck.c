#include <sys/types.h>  /* pid_t */
#include <sys/stat.h>   /* umask */
#include <unistd.h>     /* fork */
#include <stdio.h>      /* fprintf */
#include <string.h>     /* strerror */
#include <errno.h>      /* errno */
#include <stdlib.h>     /* exit */
#include <fcntl.h>      /* open */
#include <signal.h>     /* kill */

#define STAINFO_MANAGER_PIDFILE_PATH    "/var/run/stainfo_manager.pid"

int check_running(const char *pid_file) {
    char buffer[64] = { 0 };
    int fd;
    pid_t pid;

    if (NULL == pid_file || 0 == strlen(pid_file))
        return -1;

    if ((fd = open(pid_file, O_RDONLY)) < 0)
        return 0;
    
    if (read(fd, buffer, 63)) {
        if ((pid = atol(buffer)) > 0) {
            if (!kill(pid, 0)) {
                close(fd);
                return -2;
            }
        }
    }
    
    close(fd);
    return 0;
}

int write_pidfile(const char *pid_file, int pid) {
    char pid_str[16] = { 0 };
    int pid_str_len;
    int fd;
    
    if (NULL == pid_file || 0 == strlen(pid_file))
        return -1;
    
    if ((fd = open(pid_file, O_WRONLY|O_CREAT, 0666)) < 0) {
        fprintf(stderr, "Unable to open pidfile for writing %s\n", pid_file);
        return -1;
    }

    pid_str_len = snprintf(pid_str, sizeof(pid_str), "%d\n", pid);
    if (pid_str_len <= 0) {
        fprintf(stderr, "Unable to write to pidfile %s: snprintf(): FAILED.\n", pid_file);
        close(fd);
        return -1;
    } else {
        if (write(fd, pid_str, pid_str_len) < 0) {
            fprintf(stderr, "Unable to write to pidfile %s\n", pid_file);
            close(fd);
            return -1;
        }
    }

    close(fd);
    return 0;
}


