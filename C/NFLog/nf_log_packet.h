#ifndef __NF_LOG_PACKET_H__
#define __NF_LOG_PACKET_H__

/* DHCP message type */
#define	DHCP_DISCOVER		1
#define DHCP_OFFER		2
#define	DHCP_REQUEST		3
#define	DHCP_DECLINE		4
#define	DHCP_ACK		5
#define DHCP_NAK		6
#define	DHCP_RELEASE		7
#define DHCP_INFORM		8

#define DHCP_HOSTNAME_MAX_LEN 64
#define DHCP_LEASETIME_MAX_LEN 32
#define DHCP_PRIVATE_VALUE_LEN 18

enum {
    padOption				=	0,
    subnetMask				=	1,
    timerOffset				=	2,
    routersOnSubnet			=	3,
    timeServer				=	4,
    nameServer				=	5,
    dns					=	6,
    logServer				=	7,
    cookieServer				=	8,
    lprServer				=	9,
    impressServer				=	10,
    resourceLocationServer		=	11,
    hostName				=	12,
    bootFileSize				=	13,
    meritDumpFile				=	14,
    domainName				=	15,
    swapServer				=	16,
    rootPath				=	17,
    extentionsPath			=	18,
    IPforwarding				=	19,
    nonLocalSourceRouting			=	20,
    policyFilter				=	21,
    maxDgramReasmSize			=	22,
    defaultIPTTL				=	23,
    pathMTUagingTimeout			=	24,
    pathMTUplateauTable			=	25,
    ifMTU					=	26,
    allSubnetsLocal			=	27,
    broadcastAddr				=	28,
    performMaskDiscovery			=	29,
    maskSupplier				=	30,
    performRouterDiscovery		=	31,
    routerSolicitationAddr		=	32,
    staticRoute				=	33,
    trailerEncapsulation			=	34,
    arpCacheTimeout			=	35,
    ethernetEncapsulation			=	36,
    tcpDefaultTTL				=	37,
    tcpKeepaliveInterval			=	38,
    tcpKeepaliveGarbage			=	39,
    nisDomainName				=	40,
    nisServers				=	41,
    ntpServers				=	42,
    vendorSpecificInfo			=	43,
    netBIOSnameServer			=	44,
    netBIOSdgramDistServer		=	45,
    netBIOSnodeType			=	46,
    netBIOSscope				=	47,
    xFontServer				=	48,
    xDisplayManager			=	49,
    dhcpRequestedIPaddr			=	50,
    dhcpIPaddrLeaseTime			=	51,
    dhcpOptionOverload			=	52,
    dhcpMessageType			=	53,
    dhcpServerIdentifier			=	54,
    dhcpParamRequest			=	55,
    dhcpMsg				=	56,
    dhcpMaxMsgSize			=	57,
    dhcpT1value				=	58,
    dhcpT2value				=	59,
    dhcpClassIdentifier			=	60,
    dhcpClientIdentifier			=	61,
    dhcpPrivateOption		= 	225,
    endOption				=	255
};

struct arp_packet {
    unsigned char smac[6];
    char sip[16];
};

struct dhcp_packet_info {
    unsigned char sta_mac[6];
    char sta_ip[16];
    char sta_hostname[65];
    unsigned int lease_time;
    unsigned char uplink_mac[6];
    unsigned char ifname[6];
    unsigned int dhcp_msg_type;
};

struct bootp_msg {
    unsigned char   op;		/* message type */
    unsigned char   htype;	/* hardware address type */
    unsigned char   hlen;		/* hardware address length */
    unsigned char   hops;		/* should be zero in client's message */
    unsigned int    xid;		/* transaction id */
    unsigned short  secs;		/* elapsed time in sec. from trying to boot */
    unsigned short  flags;
    unsigned int    ciaddr;	/* (previously allocated) client IP address */
    unsigned int    yiaddr;	/* 'your' client IP address */
    unsigned int    siaddr;	/* should be zero in client's messages */
    unsigned int    giaddr;	/* should be zero in client's messages */
    unsigned char   chaddr[16];	/* client's hardware address */
    unsigned char   sname[64];	/* server host name, null terminated string */
    unsigned char   file[128];	/* boot file name, null terminated string */
    unsigned char   options[312];	/* message options, the first 4 bytes are  magic cookie*/
} __attribute__((packed));


int arp_packet_parser(unsigned char *data, struct arp_packet *arp_pkt);
int dhcp_packet_parser(unsigned char *data, int data_len, struct dhcp_packet_info *dhcp_info);

#endif /* __NF_LOG_PACKET_H__ */

