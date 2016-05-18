
#include <stdlib.h>     /* malloc */
#include <string.h>     /* memset */
#include <unistd.h>     /* usleep */
#include <linux/ip.h>   /* struct iphdr */
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <libnetfilter_log/libnetfilter_log.h>
#include <libnfnetlink/libnfnetlink.h>   /* nlif_open */
#include <errno.h>

#include "station_info.h"
#include "nf_log_capturer.h"
#include "nf_log_packet.h"
#include "log.h"
#include "os_tool.h"
#include "sys_command.h"
#include "timer.h"

extern int errno;

/* For nflog packet handler sample. */
static int print_pkt(struct nflog_data *ldata) {
	struct nfulnl_msg_packet_hdr *ph = nflog_get_msg_packet_hdr(ldata);
	u_int32_t mark = nflog_get_nfmark(ldata);
	u_int32_t indev = nflog_get_indev(ldata);
	u_int32_t outdev = nflog_get_outdev(ldata);
	char *prefix = nflog_get_prefix(ldata);
	char *payload;
	int payload_len = nflog_get_payload(ldata, &payload);
	
	if (ph) {
		_log(APP_DEBUG, "hw_protocol=0x%04x hook=%u ", 
			ntohs(ph->hw_protocol), ph->hook);
	}

	_log(APP_DEBUG, "mark=%u ", mark);

	if (indev > 0)
		_log(APP_DEBUG, "indev=%u ", indev);

	if (outdev > 0)
		_log(APP_DEBUG, "outdev=%u ", outdev);


	if (prefix) {
		_log(APP_DEBUG, "prefix=\"%s\" ", prefix);
	}
	if (payload_len >= 0)
		_log(APP_DEBUG, "payload_len=%d ", payload_len);

	return 0;
}

int get_iface_mac(const char *ifname, unsigned char *hwaddr)
{
     int r, s;
     struct ifreq ifr;

     strcpy(ifr.ifr_name, ifname);

     s = socket(PF_INET, SOCK_DGRAM, 0);
     if (-1 == s) {
         _log(APP_ERROR, "get_iface_mac socket: %s", strerror(errno));
         return -1;
     }

     r = ioctl(s, SIOCGIFHWADDR, &ifr);
     if (r == -1) {
        _log(APP_ERROR, "get_iface_mac ioctl(SIOCGIFHWADDR): %s", strerror(errno));
        close(s);
        return -1;
     }

     memcpy(hwaddr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
     close(s);
     return 0;
}

static int nflog_recv_eingress_callback(struct nflog_g_handle *gh, 
        struct nfgenmsg *nfmsg,
		struct nflog_data *nfa, 
		void *data) {
	struct stainfo_manager *mgr = (struct stainfo_manager *)data;
    struct nfulnl_msg_packet_hdr *pkthdr = NULL;
    struct iphdr *iphdrp = NULL;
    struct sta_info *sta = NULL;
    struct dhcp_packet_info dhcp_info;
    char ifname_physin[16] = { 0 };
    int ifname_query_ret = 0;
    u_int16_t proto;
    u_int32_t ifindex;
    char *payload = NULL;
    int payload_len = 0;

    /* ifindex will be 0 if the packet was locally generated
         * or the physical input interface is no longer known
         */
    ifindex = nflog_get_physindev(nfa);
    if (ifindex) {
        struct nlif_handle *h = nlif_open();
        if (h) {
            nlif_query(h);
            ifname_query_ret = nlif_index2name(h, ifindex, ifname_physin);
            nlif_close(h);
        }
    }
    if (ifname_query_ret <= 0)
        return -1;
    
    /* Get the metaheader that wraps the packet. */
    pkthdr = nflog_get_msg_packet_hdr(nfa);
    if (NULL == pkthdr) 
        return -1;

    proto = ntohs(pkthdr->hw_protocol);
    payload_len = nflog_get_payload(nfa, &payload);
    if (payload_len < 0) {
        _log(APP_ERROR, "get nflog packet payload failed.");
        return -1;
    }

    if (0x0800 == proto) {
        iphdrp = (struct iphdr *)payload;
        if (IPPROTO_UDP == iphdrp->protocol) {
            os_memset(&dhcp_info, 0, sizeof(struct dhcp_packet_info));
            dhcp_packet_parser((unsigned char *)payload, payload_len, &dhcp_info);
            //_log(APP_DEBUG, "nflog packet recv: %s: %s: "MAC_FORMAT" proto: 0x%04x",
            //    ifname_physin, dhcp_info.sta_ip, MAC_ADDR(dhcp_info.sta_mac), proto);
            
            sta = sta_get(mgr, dhcp_info.sta_mac);
	     if(NULL != sta && (dhcp_info.dhcp_msg_type == DHCP_RELEASE  || 
		 		dhcp_info.dhcp_msg_type == DHCP_DECLINE))
	    {
		  	 _log(APP_DEBUG, "STA get msg type %d",dhcp_info.dhcp_msg_type);
			sta_remove(mgr, sta);
			// stop timer
			timer_start(mgr);

			return 0;
	     }

            if (NULL != sta) {
                if (0 != dhcp_info.sta_ip[0] && os_strcmp(dhcp_info.sta_ip, "0.0.0.0")) {
                    os_memcpy(sta->ip, dhcp_info.sta_ip, sizeof(sta->ip));
                    _log(APP_INFO, "OLD STA: "MAC_FORMAT" IP: %s", MAC_ADDR(sta->mac), sta->ip);
                }
                os_memcpy(sta->hostname, dhcp_info.sta_hostname, 
                            MIN(sizeof(sta->hostname), sizeof(dhcp_info.sta_hostname)));  
                os_memcpy(sta->ifname, dhcp_info.ifname, sizeof(sta->ifname));
       		if(!os_memcmp(sta->ifname, "rai", 3)){
       		        os_memcpy(sta->link_type, "Wireless", sizeof(sta->link_type));		  	
       		}
       		else if(!os_memcmp(sta->ifname, "eth", 3))
       		{
       		        os_memcpy(sta->link_type, "Wired", sizeof(sta->link_type));
       		}else{
       		        os_memcpy(sta->link_type, "Wired", sizeof(sta->link_type));
       		}
                os_memcpy(sta->uplink_mac, dhcp_info.uplink_mac, sizeof(sta->uplink_mac));
            }
        }
    }
    
    return 0;
}

static int nflog_recv_wingress_callback(struct nflog_g_handle *gh, 
        struct nfgenmsg *nfmsg,
		struct nflog_data *nfa, 
		void *data) {
	struct stainfo_manager *mgr = (struct stainfo_manager *)data;
    struct nfulnl_msg_packet_hdr *pkthdr = NULL;
    struct iphdr *iphdrp = NULL;
    struct sta_info *sta = NULL;
    struct arp_packet arp_pkt;
    struct dhcp_packet_info dhcp_info;
    char ifname_physin[16] = { 0 };
	int ifname_query_ret = 0;
    u_int16_t proto;
    u_int32_t ifindex;
    char *payload = NULL;
    int payload_len = 0;

    /* ifindex will be 0 if the packet was locally generated
         * or the physical input interface is no longer known
         */
    ifindex = nflog_get_physindev(nfa);
    if (ifindex) {
        struct nlif_handle *h = nlif_open();
        if (h) {
            nlif_query(h);
            ifname_query_ret = nlif_index2name(h, ifindex, ifname_physin);
            nlif_close(h);
        }
    }
    if (ifname_query_ret <= 0)
        return -1;

    /* Do not parse packets that are not from wireless device interfaces. */
    //if (0 != strncmp("rai", ifname_physin, 3))
    //    return 0;
    
    /* Get  the MAC address of the packet source host. */
    /*unsigned char mac[8] = { 0 };
    struct nfulnl_msg_packet_hw *hwhdr = nflog_get_packet_hw(nfa);
    int i, hwaddr_len;
    if (NULL == hwhdr) {
        _log(APP_ERROR, "get nflog packet hw header failed.");
        return -1;
    }
    hwaddr_len = ntohs(hwhdr->hw_addrlen);
    for (i = 0; i < hwaddr_len; i++)    
        mac[i] = hwhdr->hw_addr[i];*/

    /* Get the metaheader that wraps the packet. */
    pkthdr = nflog_get_msg_packet_hdr(nfa);
    if (NULL == pkthdr) 
        return -1;

    proto = ntohs(pkthdr->hw_protocol);
    payload_len = nflog_get_payload(nfa, &payload);
    if (payload_len < 0) {
        _log(APP_ERROR, "get nflog packet payload failed.");
        return -1;
    }
#if 1
    if (0x0806 == proto) {
        os_memset(&arp_pkt, 0, sizeof(struct arp_packet));
        arp_packet_parser((unsigned char *)payload, &arp_pkt);

        _log(APP_DEBUG, "nflog packet recv: %s: %s: "MAC_FORMAT" proto: 0x%04x",
            ifname_physin, arp_pkt.sip, MAC_ADDR(arp_pkt.smac), proto);

        sta = sta_get(mgr, arp_pkt.smac);
        if (NULL != sta) {
            os_memcpy(sta->ip, arp_pkt.sip, sizeof(sta->ip));
            _log(APP_INFO, "OLD STA: "MAC_FORMAT" IP: %s", MAC_ADDR(sta->mac), sta->ip);
        }
    }
#endif
    if (0x0800 == proto) {
        iphdrp = (struct iphdr *)payload;
        if (IPPROTO_UDP == iphdrp->protocol) {
            os_memset(&dhcp_info, 0, sizeof(struct dhcp_packet_info));
            dhcp_packet_parser((unsigned char *)payload, payload_len, &dhcp_info);
            _log(APP_DEBUG, "nflog packet recv: %s: %s: "MAC_FORMAT" proto: 0x%04x",
                ifname_physin, dhcp_info.sta_ip, MAC_ADDR(dhcp_info.sta_mac), proto);
            
            sta = sta_get(mgr, dhcp_info.sta_mac);
	     if(NULL != sta && (dhcp_info.dhcp_msg_type == DHCP_RELEASE || 
		 	dhcp_info.dhcp_msg_type == DHCP_DECLINE))
	     {
		  	 _log(APP_DEBUG, "station get release information and remove from station list.");
			sta_remove(mgr, sta);
			// stop timer
			timer_start(mgr);

			return 0;
	     }
         if (NULL != sta) {
                if (0 != dhcp_info.sta_ip[0] && os_strcmp(dhcp_info.sta_ip, "0.0.0.0")) {
                    os_memcpy(sta->ip, dhcp_info.sta_ip, sizeof(sta->ip));
                    _log(APP_INFO, "OLD STA: "MAC_FORMAT" IP: %s", MAC_ADDR(sta->mac), sta->ip);
                }
                os_memcpy(sta->hostname, dhcp_info.sta_hostname, 
                            MIN(sizeof(sta->hostname), sizeof(dhcp_info.sta_hostname)));
		 os_memcpy(sta->ifname, dhcp_info.ifname, sizeof(sta->ifname));
		  if(!os_memcmp(sta->ifname, "rai", 3)){
	                os_memcpy(sta->link_type, "Wireless", sizeof(sta->link_type));		  	
		  }
		  else if(!os_memcmp(sta->ifname, "eth", 3))
		  {
	                os_memcpy(sta->link_type, "Wired", sizeof(sta->link_type));
		  }else{
	                os_memcpy(sta->link_type, "Wireless", sizeof(sta->link_type));
		  }
                os_memcpy(sta->uplink_mac, dhcp_info.uplink_mac, sizeof(sta->uplink_mac));
         }
        }
    }
    
    return 0;
}

static int nflog_recv_egress_callback(struct nflog_g_handle *gh, 
        struct nfgenmsg *nfmsg,
		struct nflog_data *nfa, 
		void *data) {
    struct stainfo_manager *mgr = (struct stainfo_manager *)data;
    struct nfulnl_msg_packet_hdr *pkthdr = NULL;
    struct iphdr *iphdrp = NULL;
    struct sta_info *sta = NULL;
    struct dhcp_packet_info dhcp_info;
    char ifname_physout[16] = { 0 };
	int ifname_query_ret = 0;
    u_int16_t proto;
    u_int32_t ifoutdex;
    char *payload = NULL;
    int payload_len = 0;
    time_t now;
    unsigned char interface_mac[6] = {0};
    int i = 0;
    unsigned char name[6] = {0};

    /* ifindex will be 0 if the packet was locally generated
         * or the physical input interface is no longer known
         */
    ifoutdex = nflog_get_physoutdev(nfa);
    if (ifoutdex) {
        struct nlif_handle *h = nlif_open();
        if (h) {
            nlif_query(h);
            ifname_query_ret = nlif_index2name(h, ifoutdex, ifname_physout);
            nlif_close(h);
        }
    }

    if (ifname_query_ret <= 0)   
        return -1;
    
    /* Do not parse packets that are not from wireless device interfaces. */
    //if (0 != strncmp("rai", ifname_physout, 3))
    //    return 0;

    /* Get the metaheader that wraps the packet. */
    pkthdr = nflog_get_msg_packet_hdr(nfa);
    if (NULL == pkthdr) 
        return -1;

    proto = ntohs(pkthdr->hw_protocol);
    payload_len = nflog_get_payload(nfa, &payload);
    if (payload_len < 0) {
        _log(APP_ERROR, "get nflog packet payload failed.");
        return -1;
    }

    if (0x0800 == proto) {
        iphdrp = (struct iphdr *)payload;
        if (IPPROTO_UDP == iphdrp->protocol) {
            os_memset(&dhcp_info, 0, sizeof(struct dhcp_packet_info));
            dhcp_packet_parser((unsigned char *)payload, payload_len, &dhcp_info);
            _log(APP_DEBUG, "nflog packet recv: %s: %s: "MAC_FORMAT" proto: 0x%04x",
                ifname_physout, dhcp_info.sta_ip, MAC_ADDR(dhcp_info.sta_mac), proto);
           
            sta = sta_get(mgr, dhcp_info.sta_mac);

            if (NULL != sta) {
		     if(dhcp_info.lease_time != -1 && dhcp_info.lease_time != 0){
				 if(dhcp_info.dhcp_msg_type == DHCP_OFFER && sta->lease_time > 0)
				    _log(APP_DEBUG, "sta->lease time is exist.");
                 else{
				    sta->lease_time = dhcp_info.lease_time;
			        now = get_sys_uptime();
				    /* Get current time. */
				    sta->timeout = now + sta->lease_time;
				    timer_start(mgr);
				 }
	    	    }
	            if (0 != dhcp_info.sta_ip[0] && os_strcmp(dhcp_info.sta_ip, "0.0.0.0")) {
	                os_memcpy(sta->ip, dhcp_info.sta_ip, sizeof(sta->ip));
	                _log(APP_INFO, "OLD STA: "MAC_FORMAT" IP: %s", MAC_ADDR(sta->mac), sta->ip);
	            }        
            }else{
		        if(dhcp_info.lease_time != -1 && dhcp_info.lease_time != 0){
			        for(i =0; i < 4; i++){
			            sprintf(name,"rai%d",i);
			            if(get_iface_mac(name, interface_mac)) {
			                _log(APP_ERROR, "Could not get MAC address information of %s ", name);
			            }else{
				            _log(APP_DEBUG, "STA MAC: "MAC_FORMAT"interface MAC:"MAC_FORMAT"interface name: %s", MAC_ADDR(dhcp_info.sta_mac), MAC_ADDR(interface_mac), name);
			                if(0 == memcmp(interface_mac, dhcp_info.sta_mac, 6))
			                    goto out;
			            }
                    }                   
                    sta = sta_add(mgr, dhcp_info.sta_mac);
                    sta->lease_time = dhcp_info.lease_time;
                    if (0 != dhcp_info.sta_ip[0] && os_strcmp(dhcp_info.sta_ip, "0.0.0.0")) {
                        os_memcpy(sta->ip, dhcp_info.sta_ip, sizeof(sta->ip));
                        _log(APP_INFO, "OLD STA: "MAC_FORMAT" IP: %s", MAC_ADDR(sta->mac), sta->ip);
                    }
                    now = get_sys_uptime();
                    /* Get current time. */
                    sta->timeout = now + sta->lease_time;
                    timer_start(mgr);
                }
	        }
        }        
    }
out:
    return 0;
}

int nflog_ebtables_create() {
    int i, ret = 0;
#if 0
    for (i = 0; i < NFLOG_EBT_RETRY_NUM; i++) {
        ret = add_rule_arp();
        if (!ret)
            break;
        usleep(NFLOG_EBT_RETRY_INTERVAL_MS * 1000);
    }
#endif

    for (i = 0; i < NFLOG_EBT_RETRY_NUM; i++) {
        ret = add_rule_dhcp_dport_67();
        if (!ret)
            break;
        usleep(NFLOG_EBT_RETRY_INTERVAL_MS * 1000);
    }
	
    for (i = 0; i < NFLOG_EBT_RETRY_NUM; i++) {
        ret = add_rule_dhcp_dport_E67();
        if (!ret)
            break;
        usleep(NFLOG_EBT_RETRY_INTERVAL_MS * 1000);
    }

    for (i = 0; i < NFLOG_EBT_RETRY_NUM; i++) {
        ret = add_rule_dhcp_dport_68();
        if (!ret)
            break;
        usleep(NFLOG_EBT_RETRY_INTERVAL_MS * 1000);
    }

    return ret;
}

int nflog_ebtables_destroy() {
    int i, ret = 0;
#if 0
    for (i = 0; i < NFLOG_EBT_RETRY_NUM; i++) {
        ret = delete_rule_arp();
        if (!ret)
            break;
        usleep(NFLOG_EBT_RETRY_INTERVAL_MS * 1000);
    }
#endif

    for (i = 0; i < NFLOG_EBT_RETRY_NUM; i++) {
        ret = delete_rule_dhcp_dport_67();
        if (!ret)
            break;
        usleep(NFLOG_EBT_RETRY_INTERVAL_MS * 1000);
    }
	
    for (i = 0; i < NFLOG_EBT_RETRY_NUM; i++) {
        ret = delete_rule_dhcp_dport_E67();
        if (!ret)
            break;
        usleep(NFLOG_EBT_RETRY_INTERVAL_MS * 1000);
    }

    for (i = 0; i < NFLOG_EBT_RETRY_NUM; i++) {
        ret = delete_rule_dhcp_dport_68();
        if (!ret)
            break;
        usleep(NFLOG_EBT_RETRY_INTERVAL_MS * 1000);
    }

    return ret;
}

/* */
int nflc_create(struct nf_log_capturer *pnflc) {
    if (NULL == pnflc)  return -1;
	
	char systemMode[32] = {0};

    pnflc->ieh = nflog_open();
    pnflc->iwh = nflog_open();
    pnflc->oh = nflog_open();
    if (NULL == pnflc->ieh || NULL == pnflc->iwh || NULL == pnflc->oh) {
        _log(APP_ERROR, "error during nflog_open.");
        return -1;
    }

    /* unbind existing nf_log handler from AF_BRIDGE address family. */
    if (nflog_unbind_pf(pnflc->ieh, NFLOG_DEFAULT_BINDING_PF) < 0) {
        _log(APP_ERROR, "error unbind ingress nflog pf.");
        return -1;
    }
    if (nflog_unbind_pf(pnflc->iwh, NFLOG_DEFAULT_BINDING_PF) < 0) {
        _log(APP_ERROR, "error unbind ingress nflog pf.");
        return -1;
    }
    if (nflog_unbind_pf(pnflc->oh, NFLOG_DEFAULT_BINDING_PF) < 0) {
        _log(APP_ERROR, "error unbind egress nflog pf.");
        return -1;
    }

    /* bind nfnetlink_log to AF_BRIDGE address family. */
    if (nflog_bind_pf(pnflc->ieh, NFLOG_DEFAULT_BINDING_PF) < 0) {
        _log(APP_ERROR, "error bind ingress nflog pf.");
        return -1;
    }   
    if (nflog_bind_pf(pnflc->iwh, NFLOG_DEFAULT_BINDING_PF) < 0) {
        _log(APP_ERROR, "error bind ingress nflog pf.");
        return -1;
    }
    if (nflog_bind_pf(pnflc->oh, NFLOG_DEFAULT_BINDING_PF) < 0) {
        _log(APP_ERROR, "error bind egress nflog pf.");
        return -1;
    }

    /* bind to group */
    pnflc->ieqh = nflog_bind_group(pnflc->ieh, pnflc->in_group_eid);
    if (NULL == pnflc->ieqh) {
        _log(APP_ERROR, "error bind ingress nflog group.");
        return -1;
    }
    pnflc->iwqh = nflog_bind_group(pnflc->iwh, pnflc->in_group_wid);
    if (NULL == pnflc->iwqh) {
        _log(APP_ERROR, "error bind ingress nflog group.");
        return -1;
    }
    pnflc->oqh = nflog_bind_group(pnflc->oh, pnflc->out_group_id);
    if (NULL == pnflc->oqh) {
        _log(APP_ERROR, "error bind egress nflog group.");
        return -1;
    }

    /* set copy_packet mode */
    if (nflog_set_mode(pnflc->ieqh, NFULNL_COPY_PACKET, 0xffff) < 0) {
        _log(APP_ERROR, "can not set ingress packet copy mode.");
        return -1;
    }
    if (nflog_set_mode(pnflc->iwqh, NFULNL_COPY_PACKET, 0xffff) < 0) {
        _log(APP_ERROR, "can not set ingress packet copy mode.");
        return -1;
    }
    if (nflog_set_mode(pnflc->oqh, NFULNL_COPY_PACKET, 0xffff) < 0) {
        _log(APP_ERROR, "can not set egress packet copy mode.");
        return -1;
    }
	
    pnflc->ifed = nflog_fd(pnflc->ieh);
    pnflc->ifwd = nflog_fd(pnflc->iwh);
    pnflc->ofd = nflog_fd(pnflc->oh);
    
    /* register log receive callback function. */
    nflog_callback_register(pnflc->ieqh, &nflog_recv_eingress_callback, pnflc->stainfo_mgr);
    nflog_callback_register(pnflc->iwqh, &nflog_recv_wingress_callback, pnflc->stainfo_mgr);
    nflog_callback_register(pnflc->oqh, &nflog_recv_egress_callback, pnflc->stainfo_mgr);

    if (0 != nflog_ebtables_create()) {
         _log(APP_ERROR, "ebtables nflog rules add failed.");
        return -1;
    }

    return 0;
}

int nflc_routine(struct nf_log_capturer *pnflc, int fd) {
    int rsz;
    
    if (NULL == pnflc || 0 == fd)  return -1;
    memset(pnflc->buffer, 0, sizeof(pnflc->buffer));

    rsz = recv(fd, pnflc->buffer, sizeof(pnflc->buffer), 0);
    if (rsz < 0) {
        _log(APP_ERROR, "error during nflc receive.");
        return -1;
    } else if (rsz > 0){
        if (fd == pnflc->ifed)
            nflog_handle_packet(pnflc->ieh, pnflc->buffer, rsz);
        else if (fd == pnflc->ifwd)
            nflog_handle_packet(pnflc->iwh, pnflc->buffer, rsz);
        else if (fd == pnflc->ofd)
            nflog_handle_packet(pnflc->oh, pnflc->buffer, rsz);
    }

    return 0;
}

void nflc_destroy(struct nf_log_capturer *pnflc) {
    if (NULL == pnflc)  return;

    nflog_ebtables_destroy();
    if (pnflc->ieqh) {
        nflog_unbind_group(pnflc->ieqh);
        _log(APP_DEBUG, "unbind ingress group.");
    }
    if (pnflc->iwqh) {
        nflog_unbind_group(pnflc->iwqh);
        _log(APP_DEBUG, "unbind ingress group.");
    }
    if (pnflc->oqh) {
        nflog_unbind_group(pnflc->oqh);
        _log(APP_DEBUG, "unbind egress group.");
    }
     if (pnflc->ieh) {
        nflog_unbind_pf(pnflc->ieh, NFLOG_DEFAULT_BINDING_PF);
        _log(APP_DEBUG, "unbind ingress pf.");
        nflog_close(pnflc->ieh);
        _log(APP_DEBUG, "nflog ingress close.");
    }
   
    if (pnflc->iwh) {
        nflog_unbind_pf(pnflc->iwh, NFLOG_DEFAULT_BINDING_PF);
        _log(APP_DEBUG, "unbind ingress pf.");
        nflog_close(pnflc->iwh);
        _log(APP_DEBUG, "nflog ingress close.");
    }
    if (pnflc->oh) {
        nflog_unbind_pf(pnflc->oh, NFLOG_DEFAULT_BINDING_PF);
        _log(APP_DEBUG, "unbind egress pf.");
        nflog_close(pnflc->oh);
        _log(APP_DEBUG, "nflog egress close.");
    }

    pnflc->iwh = NULL;
	pnflc->ieh = NULL;
    pnflc->ieqh = NULL;
    pnflc->iwqh = NULL;
    pnflc->ifed = 0;
    pnflc->ifwd = 0;
    pnflc->oh = NULL;
    pnflc->oqh = NULL;
    pnflc->ofd = 0;
}

struct nf_log_capturer *nflc_init() {
    struct nf_log_capturer *nflc = NULL;

    nflc = (struct nf_log_capturer *)malloc(sizeof(struct nf_log_capturer));
    if (NULL == nflc)
        return NULL;

    memset(nflc, 0, sizeof(struct nf_log_capturer));
    nflc->in_group_eid = NFLOG_INGRESS_EGROUPID;
    nflc->in_group_wid = NFLOG_INGRESS_WGROUPID;
    nflc->out_group_id = NFLOG_EGRESS_GROUPID;
    
    return nflc;
}

void nflc_free(struct nf_log_capturer *pnflc) {
    if (NULL == pnflc)  return;

    /* Destroy resource in nflc_destroy, here we only free memory. 
         * Pay attention that who creat the nflc must destroy it before free.
         */
    free(pnflc);
}


