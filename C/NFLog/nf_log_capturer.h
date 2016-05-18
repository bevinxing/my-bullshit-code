#ifndef __NF_LOG_CAPTURER_H__
#define __NF_LOG_CAPTURER_H__

#include <libnetfilter_log/libnetfilter_log.h>
#include <netinet/in.h> /* AF_BRIDGE */

#define NFLOG_INGRESS_EGROUPID   1
#define NFLOG_INGRESS_WGROUPID   2
#define NFLOG_EGRESS_GROUPID    3
#define NFLOG_DEFAULT_BINDING_PF    AF_BRIDGE

#define NFLOG_EBT_RETRY_NUM 3
#define NFLOG_EBT_RETRY_INTERVAL_MS 300

#define NFLOG_RECV_BUFFSIZE  8192    /* Due to NFNL_BUFFSIZE in libnfnetlink/libnfnetlink.h is 8192 */

struct nf_log_capturer {
    int ifwd;    /* fd for wireless ingress direction */
    int ifed;    /* fd for ethernet ingress direction */
    int ofd;    /* fd for egress direction */
    struct nflog_handle *iwh;
    struct nflog_handle *ieh;
    struct nflog_handle *oh;
    struct nflog_g_handle *ieqh;
    struct nflog_g_handle *iwqh;
    struct nflog_g_handle *oqh;
    int in_group_wid;
    int in_group_eid;
    int out_group_id;
    char buffer[NFLOG_RECV_BUFFSIZE];

    struct stainfo_manager *stainfo_mgr;    /* Back point to stainfo_manager. */
};

struct nf_log_capturer *nflc_init();
void nflc_free(struct nf_log_capturer *pnflc);
int nflc_create(struct nf_log_capturer *pnflc);
void nflc_destroy(struct nf_log_capturer *pnflc);
int nflc_routine(struct nf_log_capturer *pnflc, int fd);

#endif /* __NF_LOG_CAPTURER_H__ */