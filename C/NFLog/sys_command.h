#ifndef __SYS_COMMAND_H__
#define __SYS_COMMAND_H__

#define SYS_EBTABLES_RULE_MAX_LEN 1024
#define SYS_COMMAND_MAX_LEN 2048

#define NF_LOG_STAINFO_TABLE "nat"
#define NF_LOG_STAINFO_INGRESS_CHAIN "PREROUTING"
#define NF_LOG_STAINFO_EGRESS_CHAIN "POSTROUTING"

#define DEFAULT_NFLOG_GROUP 3

#define NF_LOG_RULE_FMT_ADD_ARP  \
    " -t " NF_LOG_STAINFO_TABLE	\
	" -A " NF_LOG_STAINFO_INGRESS_CHAIN	\
	" -i rai+ "	\
	" --proto arp " \
	" --nflog-group %d " \
	" -j CONTINUE"

#define NF_LOG_RULE_FMT_DEL_ARP  \
    " -t " NF_LOG_STAINFO_TABLE	\
	" -D " NF_LOG_STAINFO_INGRESS_CHAIN	\
	" -i rai+ "	\
	" --proto arp " \
	" --nflog-group %d " \
	" -j CONTINUE"

#define NF_LOG_RULE_FMT_ADD_DHCP_WDP_67  \
    " -t " NF_LOG_STAINFO_TABLE	\
	" -A " NF_LOG_STAINFO_INGRESS_CHAIN	\
	" -i ! eth+"	\
	" --proto ipv4 " \
	" --ip-protocol UDP " \
	" --ip-destination-port 67 " \
	" --nflog-group %d " \
	" -j CONTINUE"

#define NF_LOG_RULE_FMT_DEL_DHCP_WDP_67  \
    " -t " NF_LOG_STAINFO_TABLE	\
	" -D " NF_LOG_STAINFO_INGRESS_CHAIN	\
	" -i ! eth+"	\
	" --proto ipv4 " \
	" --ip-protocol UDP " \
	" --ip-destination-port 67 " \
	" --nflog-group %d " \
	" -j CONTINUE"
	
#define NF_LOG_RULE_FMT_ADD_DHCP_EDP_67  \
    " -t " NF_LOG_STAINFO_TABLE	\
	" -A " NF_LOG_STAINFO_INGRESS_CHAIN	\
	" -i eth3.+"	\
	" --proto ipv4 " \
	" --ip-protocol UDP " \
	" --ip-destination-port 67 " \
	" --nflog-group %d " \
	" -j CONTINUE"

#define NF_LOG_RULE_FMT_DEL_DHCP_EDP_67  \
    " -t " NF_LOG_STAINFO_TABLE	\
	" -D " NF_LOG_STAINFO_INGRESS_CHAIN	\
	" -i eth3.+"	\
	" --proto ipv4 " \
	" --ip-protocol UDP " \
	" --ip-destination-port 67 " \
	" --nflog-group %d " \
	" -j CONTINUE"

#define NF_LOG_RULE_FMT_ADD_DHCP_DP_68  \
    " -t " NF_LOG_STAINFO_TABLE	\
	" -A " NF_LOG_STAINFO_EGRESS_CHAIN	\
	" --proto ipv4 " \
	" --ip-protocol UDP " \
	" --ip-destination-port 68 " \
	" --nflog-group %d " \
	" -j CONTINUE"

#define NF_LOG_RULE_FMT_DEL_DHCP_DP_68  \
    " -t " NF_LOG_STAINFO_TABLE	\
	" -D " NF_LOG_STAINFO_EGRESS_CHAIN	\
	" --proto ipv4 " \
	" --ip-protocol UDP " \
	" --ip-destination-port 68 " \
	" --nflog-group %d " \
	" -j CONTINUE"

int add_rule_arp();
int delete_rule_arp();
int add_rule_dhcp_dport_67();
int add_rule_dhcp_dport_E67();
int delete_rule_dhcp_dport_E67();
int delete_rule_dhcp_dport_67();
int add_rule_dhcp_dport_68();
int delete_rule_dhcp_dport_68();

#endif /* __SYS_COMMAND_H__ */
