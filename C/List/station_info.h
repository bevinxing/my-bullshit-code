#ifndef __STATION_INFO_H__
#define __STATION_INFO_H__
#include <pthread.h>

struct sta_info {
    struct sta_info *next;
    struct sta_info *hnext;
    unsigned char mac[6];
    char ip[16];
    char hostname[65];
    unsigned int lease_time;
    char link_type[10];
    unsigned char uplink_mac[6];
    unsigned char ifname[6];
    long timeout; /*timeout=current system boot up time + lease_time*/
    int timer_flag;
};

#define LOCK_MANAGER_LIST() do { \
	pthread_mutex_lock(&manager_list_mutex); \
} while (0)

#define UNLOCK_MANAGER_LIST() do { \
	pthread_mutex_unlock(&manager_list_mutex); \
} while (0)

struct stainfo_manager;
struct sta_info *sta_get(struct stainfo_manager *mgr, const unsigned char *mac);
struct sta_info *sta_add(struct stainfo_manager *mgr, const unsigned char *mac);
int sta_remove(struct stainfo_manager *mgr, struct sta_info *sta);
int sta_list_remove(struct stainfo_manager *mgr);

#endif /* __STATION_INFO_H__ */
