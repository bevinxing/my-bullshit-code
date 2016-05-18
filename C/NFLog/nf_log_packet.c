
#include <stdlib.h>     /* NULL */
#include <net/if_arp.h> /* arphdr */
#include <string.h>     /* memcpy */
#include <arpa/inet.h>  /* inet_ntop */
#include <linux/ip.h>   /* struct iphdr */
#include <linux/udp.h>  /* struct udphdr */
#include <asm/byteorder.h>

#include "nf_log_packet.h"
#include "log.h"
#include "os_tool.h"

int arp_packet_parser(unsigned char *data, struct arp_packet *arp_pkt) {
    struct arphdr *arp = NULL;
    unsigned char *arp_addr_p = NULL;
    unsigned int sip = 0;

    arp = (struct arphdr *)data;
    arp_addr_p = (unsigned char *)(arp + 1);

    memcpy(arp_pkt->smac, arp_addr_p, 6);
    arp_addr_p += 6;
    memcpy(&sip, arp_addr_p, 4);

    inet_ntop(AF_INET, &sip, arp_pkt->sip, INET_ADDRSTRLEN);
    
    return 0;
}

int dhcp_packet_parser(unsigned char *data, int data_len, struct dhcp_packet_info *dhcp_info) {
    struct bootp_msg *msg;
    unsigned char *p;
    unsigned char *end;
    int iphdr_len, udphdr_len;
    int dhcp_msg_type = 0;
    unsigned int lease_time = -1;
    int length = 0;
    char hostname[DHCP_HOSTNAME_MAX_LEN] = { 0 };
    unsigned char ifname[6] = {0};
    unsigned char uplink_mac[6] = {0};
    //char ifname_type[3] = "eth";
	
    /* IP header */
    struct iphdr *piphdr = (struct iphdr *)data;
    iphdr_len = piphdr->ihl * 4;

    /* UDP header */
    udphdr_len = sizeof(struct udphdr);

    /* For bootp Application data */
    msg = (struct bootp_msg *)(data + iphdr_len + udphdr_len);
    p = msg->options + 4;   /* offset of magic cookie */
    end = msg->options + sizeof(msg->options);
    /* ****** option structure *****
         * |-opt id-|-length-|-variable value-|
         */
    while (p < end) {
        switch(*p) {
	 case dhcpPrivateOption:
		length = p[1];
		_log(APP_DEBUG, "DHCP message private length: %d",length);
		os_memcpy(uplink_mac, &p[3 + (length - 18)], 6);
	       _log(APP_DEBUG, "nflog packet recv:"MAC_FORMAT, MAC_ADDR(uplink_mac));
		os_memcpy(ifname, &p[10 + (length - 18)], 4);
		_log(APP_DEBUG, "nflog packet recv sta type: %02x %02x %02x %02x", ifname[0], ifname[1], ifname[2], ifname[3]);
	       p += p[1];
	       p += 2;
		break;
        case endOption:
            goto OUT;
        case padOption:
            p++;
            break;
        case dhcpIPaddrLeaseTime:
            if (p[1]){
		#define GET_UINT32(_val) \
			memcpy (&_val, &p[2], sizeof (uint32_t));
		#define GET_UINT32_H(_val) \
			GET_UINT32 (_val); \
			_val = ntohl (_val);
		GET_UINT32_H(lease_time);
	     }
            p += p[1];
            p += 2;
            break;
        case hostName:
            if (p[1])
                os_memcpy(hostname, &p[2], MIN(p[1], DHCP_HOSTNAME_MAX_LEN));
            p += p[1];
            p += 2;
            break;
        case dhcpMessageType:
            if (p[1])
                dhcp_msg_type = p[2];
            p += p[1];
            p += 2;
            break;
        default:
            p += p[1];
            p += 2;
            break;
        }
    }
OUT:
    dhcp_info->dhcp_msg_type = dhcp_msg_type;
    _log(APP_DEBUG, "DHCP message type: %d", dhcp_msg_type);
    switch(dhcp_msg_type) {
    case DHCP_OFFER:
    case DHCP_ACK:     
        if(dhcp_msg_type == DHCP_ACK)
            inet_ntop(AF_INET, &msg->yiaddr, dhcp_info->sta_ip, INET_ADDRSTRLEN);
        dhcp_info->lease_time = lease_time;
        os_memcpy(dhcp_info->sta_mac, msg->chaddr, sizeof(dhcp_info->sta_mac));
        break;
    case DHCP_DISCOVER:
    case DHCP_REQUEST:
    //case DHCP_INFORM:
        inet_ntop(AF_INET, &msg->ciaddr, dhcp_info->sta_ip, INET_ADDRSTRLEN);
        os_memcpy(dhcp_info->sta_mac, msg->chaddr, sizeof(dhcp_info->sta_mac));
        os_memcpy(dhcp_info->sta_hostname, hostname, MIN(sizeof(dhcp_info->sta_hostname), DHCP_HOSTNAME_MAX_LEN));
        os_memcpy(dhcp_info->uplink_mac, uplink_mac, sizeof(dhcp_info->uplink_mac));
        os_memcpy(dhcp_info->ifname, ifname, sizeof(dhcp_info->ifname));
        break;
    case DHCP_RELEASE:
        os_memcpy(dhcp_info->sta_mac, msg->chaddr, sizeof(dhcp_info->sta_mac));
    case DHCP_NAK:
        os_memcpy(dhcp_info->sta_mac, msg->chaddr, sizeof(dhcp_info->sta_mac));
	 break;
    default:
        break;
    }    
    
    return 0;
}


