#include <stdlib.h>
#include <string.h>     /* strncmp */

#include "station_info.h"
#include "manager.h"
#include "os_tool.h"
#include "log.h"

void sta_hash_add(struct stainfo_manager *mgr, struct sta_info *sta) {
    sta->hnext = mgr->sta_hash[STA_HASH(sta->mac)];
    mgr->sta_hash[STA_HASH(sta->mac)] = sta;
}

/* Remove station entry from hash list. */
void sta_hash_del(struct stainfo_manager *mgr, struct sta_info *sta) {
    struct sta_info *s = NULL;

    /* Get the hash list head by the mac hash. */
    s = mgr->sta_hash[STA_HASH(sta->mac)];
    if (NULL == s)  return;
    if (0 == memcmp(s->mac, sta->mac, 6)) {
        /* The station entry is at the head of the hash list. */
        mgr->sta_hash[STA_HASH(sta->mac)] = s->hnext;
    } else {
        while (NULL != s->hnext && (0 != memcmp(s->hnext->mac, sta->mac, 6)))
            s = s->hnext;
        if (NULL != s->hnext)
            s->hnext = s->hnext->hnext;
        else
            _log(APP_ERROR, "error in sta hash delete: "MAC_FORMAT, sta->mac[0], 
                sta->mac[1], sta->mac[2], sta->mac[3], sta->mac[4], sta->mac[5]);
    }
}

/* Remove station entry from real list. */
void sta_del(struct stainfo_manager *mgr, struct sta_info *sta) {
    struct sta_info *tmp = NULL;

    if (sta == mgr->sta_list) {
        /* The station entry is at the head of the list. */
        mgr->sta_list = sta->next;
        return;
    }

    tmp = mgr->sta_list;
    while (NULL != tmp && tmp->next != sta)
        tmp = tmp->next;
    if (NULL == tmp)
        _log(APP_ERROR, "error in sta delete: "MAC_FORMAT, sta->mac[0], 
                sta->mac[1], sta->mac[2], sta->mac[3], sta->mac[4], sta->mac[5]);
    else
        tmp->next = sta->next;
}

struct sta_info *sta_get(struct stainfo_manager *mgr, const unsigned char *mac) {
    struct sta_info *s = NULL;

    LOCK_MANAGER_LIST();
    s = mgr->sta_hash[STA_HASH(mac)];
    while (NULL != s && (0 != os_memcmp((const char *)s->mac, (const char *)mac, 6)))
        s = s->hnext;
    UNLOCK_MANAGER_LIST();
    return s;
}

struct sta_info *sta_add(struct stainfo_manager *mgr, const unsigned char *mac) {
    struct sta_info *s = NULL;
    LOCK_MANAGER_LIST();
    if (mgr->sta_num >= mgr->mgrconf.max_sta) {
        _log(APP_NOTICE, "station information entry table full. %d", mgr->sta_num);
        // TODO: remove unqueried or timeout entry.
     UNLOCK_MANAGER_LIST();
        return NULL;
    }

    s = (struct sta_info *)malloc(sizeof(struct sta_info));
    if (NULL == s) {
        _log(APP_ERROR, "station information entry malloc failed.");
     UNLOCK_MANAGER_LIST();
        return NULL;
    }
    memset(s, 0, sizeof(struct sta_info));
    memcpy(s->mac, mac, sizeof(s->mac));
    s->next = mgr->sta_list;
    mgr->sta_list = s;
    mgr->sta_num++;
    sta_hash_add(mgr, s);
    UNLOCK_MANAGER_LIST();
    
    return s;
}

int sta_remove(struct stainfo_manager *mgr, struct sta_info *sta) {
    LOCK_MANAGER_LIST();
    sta_hash_del(mgr, sta);
    sta_del(mgr, sta);

    mgr->sta_num--;
    free(sta);
    UNLOCK_MANAGER_LIST();

    return 0;
}

int sta_list_remove(struct stainfo_manager *mgr) {
    struct sta_info *list_head;
    struct sta_info *tmp;
    
    if (NULL == mgr)
        return -1;

    list_head = mgr->sta_list;
    while (list_head) {
        tmp = list_head;
        list_head = list_head->next;
        _log(APP_INFO, "list remove sta: "MAC_FORMAT, MAC_ADDR(tmp->mac));
        sta_remove(mgr, tmp);   
    }
    return 0;
}
