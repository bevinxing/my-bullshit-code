#ifndef __STAINFO_MANAGER_H__
#define __STAINFO_MANAGER_H__

#define STA_HASH_SIZE 256
#define STA_HASH(sta) (sta[5])

#define DEFAULT_MAX_STATION_NUMBER  1024
#define DEFAULT_MAX_CLIENT_CONN_NUMBER  256
#define DEFAULT_CLIENT_LISTEN_BACKLOG  5

#define STAINFO_MANAGER_PIDFILE_PATH    "/var/run/stainfo_manager.pid"

struct manager_config {
    unsigned short daemonize;
    int debug;
    int debug_level;
    int log_level;
    int max_sta;
    int max_conn;
};

struct stainfo_manager {
    struct sta_info *sta_list;  /* station list head associated with AP. */
    struct sta_info *sta_hash[STA_HASH_SIZE];   /* hash list head array. */
    int server_sock;                /* socket which Client connected with. */
    int netlink_sock;               /* socket for netlink comm to listen Wi-Fi driver events. */
    int max_fd;                     /* maximum fd to be selected. */
    int sta_num;                    /* station number associated. */

    struct client_conn *conn_list;    /* list head of client connections with manager server. */
    int conn_num;
    struct nf_log_capturer *pnflc;  /* pointer to capturer which is in order to capture certain type nf log packets. */

    struct manager_config mgrconf;
};

struct stainfo_manager *manager_init();
void manager_free(struct stainfo_manager *mgr);
int manager_routine(struct stainfo_manager *mgr);
int client_server_create(const char *srv_path);
int netlink_server_create();

#endif /* __STAINFO_MANAGER_H__ */
