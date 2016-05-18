
#include <stdio.h>      /* snprintf */
#include <stdlib.h>     /* system */
#include <sys/wait.h>   /* WIFEXITED, WEXITSTATUS */

#include "sys_command.h"
#include "nf_log_capturer.h"

static int system_do_command(const char *cmd) {
    int status;
    int ret = 0;

    status = system(cmd);
    if (-1 == status) {
        _log(APP_ERROR, "system call error!!! %s", cmd);
        ret = -1;
    } else {
        _log(APP_DEBUG, "system call status: %d cmd: %s", status, cmd);
        /* WIFEXITED(status) is non-zero if child process terminated normally. */
        if (WIFEXITED(status)) {
            /* script exit code or return value will be stored in the low-order 8 of the status. */
            if (0 == WEXITSTATUS(status))
                ret = 0;
            else
                ret = -2;   /* Here we considered that "ebtables" will exit with non-zero if failed. */
        } else {
            _log(APP_ERROR, "system child terminated abnormally. cmd: %s", cmd);
            ret = -1;
        }
    }
    return ret;
}

static int ebtables_do_command(const char *rule) {
    char cmd[SYS_COMMAND_MAX_LEN + 1] = { 0 };

    snprintf(cmd, SYS_COMMAND_MAX_LEN+1, "ebtables %s 2>/dev/null", rule);
    return system_do_command(cmd);
}

int add_rule_arp() {
    char rule[SYS_EBTABLES_RULE_MAX_LEN + 1] = { 0 };

    snprintf(rule, SYS_EBTABLES_RULE_MAX_LEN+1, NF_LOG_RULE_FMT_ADD_ARP, NFLOG_INGRESS_WGROUPID);
    if (ebtables_do_command(rule) < 0)
        return -1;
    else
        return 0;
}

int delete_rule_arp() {
    char rule[SYS_EBTABLES_RULE_MAX_LEN + 1] = { 0 };

    snprintf(rule, SYS_EBTABLES_RULE_MAX_LEN+1, NF_LOG_RULE_FMT_DEL_ARP, NFLOG_INGRESS_WGROUPID);
    if (ebtables_do_command(rule) < 0)
        return -1;
    else
        return 0;
}

int add_rule_dhcp_dport_67() {
    char rule[SYS_EBTABLES_RULE_MAX_LEN + 1] = { 0 };

    snprintf(rule, SYS_EBTABLES_RULE_MAX_LEN+1, NF_LOG_RULE_FMT_ADD_DHCP_WDP_67, NFLOG_INGRESS_WGROUPID);
    if (ebtables_do_command(rule) < 0)
        return -1;
    else
        return 0;
}

int delete_rule_dhcp_dport_67() {
    char rule[SYS_EBTABLES_RULE_MAX_LEN + 1] = { 0 };

    snprintf(rule, SYS_EBTABLES_RULE_MAX_LEN+1, NF_LOG_RULE_FMT_DEL_DHCP_WDP_67, NFLOG_INGRESS_WGROUPID);
    if (ebtables_do_command(rule) < 0)
        return -1;
    else
        return 0;
}

int add_rule_dhcp_dport_E67() {
    char rule[SYS_EBTABLES_RULE_MAX_LEN + 1] = { 0 };

    snprintf(rule, SYS_EBTABLES_RULE_MAX_LEN+1, NF_LOG_RULE_FMT_ADD_DHCP_EDP_67, NFLOG_INGRESS_EGROUPID);
    if (ebtables_do_command(rule) < 0)
        return -1;
    else
        return 0;
}

int delete_rule_dhcp_dport_E67() {
    char rule[SYS_EBTABLES_RULE_MAX_LEN + 1] = { 0 };

    snprintf(rule, SYS_EBTABLES_RULE_MAX_LEN+1, NF_LOG_RULE_FMT_DEL_DHCP_EDP_67, NFLOG_INGRESS_EGROUPID);
    if (ebtables_do_command(rule) < 0)
        return -1;
    else
        return 0;
}
int add_rule_dhcp_dport_68() {
    char rule[SYS_EBTABLES_RULE_MAX_LEN + 1] = { 0 };

    snprintf(rule, SYS_EBTABLES_RULE_MAX_LEN+1, NF_LOG_RULE_FMT_ADD_DHCP_DP_68, NFLOG_EGRESS_GROUPID);
    if (ebtables_do_command(rule) < 0)
        return -1;
    else
        return 0;
}

int delete_rule_dhcp_dport_68() {
    char rule[SYS_EBTABLES_RULE_MAX_LEN + 1] = { 0 };

    snprintf(rule, SYS_EBTABLES_RULE_MAX_LEN+1, NF_LOG_RULE_FMT_DEL_DHCP_DP_68, NFLOG_EGRESS_GROUPID);
    if (ebtables_do_command(rule) < 0)
        return -1;
    else
        return 0;
}


