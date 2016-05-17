#include <sys/types.h>  /* pid_t */
#include <sys/stat.h>   /* umask */
#include <unistd.h>     /* fork */
#include <stdio.h>      /* fprintf */
#include <string.h>     /* strerror */
#include <errno.h>      /* errno */
#include <stdlib.h>     /* exit */
#include <fcntl.h>      /* open */
#include <signal.h>     /* kill */

void daemonize(const char *pid_path) {
    pid_t pid;
    int fd;

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed, error: %s\n", strerror(errno));
        exit(1);
    }

    if (pid > 0) {
        /* fork succeeded and I am the parent process, exit. */
        exit(0);
    }

    /* I am child process. Open new session and get rid of parent group. */
    if (setsid() < 0) {
        fprintf(stderr, "setsid failed, error: %s\n", strerror(errno));
        exit(1);
    }

    /* Change working directory */
    if (chdir("/") < 0) {
        fprintf(stderr, "change dir failed, error: %s\n", strerror(errno));
        exit(1);
    }

    if (umask(0) < 0) {
        fprintf(stderr, "reset file mask failed, error: %s\n", strerror(errno));
        exit(1);
    }

    /* redirect std to /dev/null */
    fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open dev null failed, error: %s\n", strerror(errno));
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);
}

/* Pseudo Code. Just for example. */
int main(int argc, char **argv)
{
    struct stainfo_manager *mgr = NULL;
    pid_t pid;
    char c;

    if (0 != check_running(STAINFO_MANAGER_PIDFILE_PATH)) {
        fprintf(stdout, "Station Information Manager already running...\n");
        return 0;
    }

    while (-1 != (c = getopt(argc, argv, "Dd:l:h"))) {
        switch(c) {
        case 'D':
            mgr->mgrconf.daemonize = 1;
            break;
        case 'd':
            mgr->mgrconf.debug = 1;
            mgr->mgrconf.debug_level = atoi(optarg);
            break;
        case 'l':
            mgr->mgrconf.log_level = atoi(optarg);
            break;
        case 'h':
            show_help();
            return 0;
        default:
            show_help();
            manager_free(mgr);
            return -1;
        }
    }

    
    if (mgr->mgrconf.daemonize) daemonize(NULL);
    if (mgr->mgrconf.daemonize && mgr->mgrconf.debug) {
        freopen("/dev/console", "w", stdout);
        freopen("/dev/console", "w", stderr);
    }

    /* Register signal handler. */
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGPIPE, signal_handler);
    signal(SIGALRM,timer_signal_handler);
    
    traffic_monitor_start();

    if(sigemptyset(&sigmask) ==  -1)
    {
            _log(APP_ERROR,"sigemptyset error.\n");
            goto FREE;
    }

    if(sigaddset(&sigmask,SIGALRM) == -1)
    {
            _log(APP_ERROR,"sigaddset error.\n");
            goto FREE;
    }
    
    pid = getpid();
    if (0 != write_pidfile(STAINFO_MANAGER_PIDFILE_PATH, pid)) {
        _log(APP_ERROR, "Fail to write pid file: %s, %d", STAINFO_MANAGER_PIDFILE_PATH, pid);
        goto FREE;
    }

FREE:
    unlink(STAINFO_MANAGER_PIDFILE_PATH);

    return 0;
}