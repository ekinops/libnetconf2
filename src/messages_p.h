/**
 * \file messages_p.h
 * \author Radek Krejci <rkrejci@cesnet.cz>
 * \brief libnetconf2's private functions and structures of NETCONF messages.
 *
 * Copyright (c) 2015 CESNET, z.s.p.o.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Company nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 */

#ifndef NC_MESSAGES_P_H_
#define NC_MESSAGES_P_H_

#include <libyang/libyang.h>

#include "messages.h"

extern const char *rpcedit_dfltop2str[];
extern const char *rpcedit_testopt2str[];
extern const char *rpcedit_erropt2str[];

typedef enum {
    NC_ERR_EMPTY,
    NC_ERR_IN_USE,
    NC_ERR_INVALID_VALUE,
    NC_ERR_TOO_BIG,
    NC_ERR_MISSING_ATTR,
    NC_ERR_BAD_ATTR,
    NC_ERR_UNKNOWN_ATTR,
    NC_ERR_MISSING_ELEM,
    NC_ERR_BAD_ELEM,
    NC_ERR_UNKNOWN_ELEM,
    NC_ERR_UNKNOWN_NS,
    NC_ERR_ACCESS_DENIED,
    NC_ERR_LOCK_DENIED,
    NC_ERR_RES_DENIED,
    NC_ERR_ROLLBACK_FAILED,
    NC_ERR_DATA_EXISTS,
    NC_ERR_DATA_MISSING,
    NC_ERR_OP_NOT_SUPPORTED,
    NC_ERR_OP_FAILED,
    NC_ERR_MALFORMED_MSG
} NC_ERR;

struct nc_server_rpc {
    struct lyxml_elem *root; /**< RPC element of the received XML message */
    struct lyd_node *tree;   /**< libyang data tree of the message (NETCONF operation) */
};

struct nc_rpc {
    NC_RPC_TYPE type;
};

struct nc_rpc_generic {
    NC_RPC_TYPE type;       /**< NC_RPC_GENERIC */
    int has_data;           /**< 1 for content.data, 0 for content.xml_str */
    union {
        struct lyd_node *data;  /**< parsed RPC data */
        char *xml_str;          /**< raw XML string */
    } content;
    char free;
};

struct nc_rpc_getconfig {
    NC_RPC_TYPE type;        /**< NC_RPC_GETCONFIG */
    NC_DATASTORE source;     /**< NETCONF datastore being queried */
    char *filter;            /**< either XML subtree (starts with '<') or an XPath (starts with '/' or an alpha) */
    NC_WD_MODE wd_mode;
    char free;
};

struct nc_rpc_edit {
    NC_RPC_TYPE type;        /**< NC_RPC_EDIT */
    NC_DATASTORE target;
    NC_RPC_EDIT_DFLTOP default_op;
    NC_RPC_EDIT_TESTOPT test_opt;
    NC_RPC_EDIT_ERROPT error_opt;
    char *edit_cont;         /**< either URL (starts with aplha) or config (starts with '<') */
    char free;
};

struct nc_rpc_copy {
    NC_RPC_TYPE type;        /**< NC_RPC_COPY */
    NC_DATASTORE target;
    char *url_trg;
    NC_DATASTORE source;
    char *url_config_src;    /**< either URL (starts with aplha) or config (starts with '<') */
    NC_WD_MODE wd_mode;
    char free;
};

struct nc_rpc_delete {
    NC_RPC_TYPE type;        /**< NC_RPC_DELETE */
    NC_DATASTORE target;
    char *url;
    char free;
};

struct nc_rpc_lock {
    NC_RPC_TYPE type;        /**< NC_RPC_LOCK or NC_RPC_UNLOCK */
    NC_DATASTORE target;
};

struct nc_rpc_get {
    NC_RPC_TYPE type;        /**< NC_RPC_GET */
    char *filter;            /**< either XML subtree (starts with '<') or an XPath (starts with '/' or an alpha) */
    NC_WD_MODE wd_mode;
    char free;
};

struct nc_rpc_kill {
    NC_RPC_TYPE type;        /**< NC_RPC_KILL */
    uint32_t sid;
};

struct nc_rpc_commit {
    NC_RPC_TYPE type;        /**< NC_RPC_COMMIT */
    int confirmed;
    uint32_t confirm_timeout;
    char *persist;
    char *persist_id;
    char free;
};

struct nc_rpc_cancel {
    NC_RPC_TYPE type;        /**< NC_RPC_CANCEL */
    char *persist_id;
    char free;
};

struct nc_rpc_validate {
    NC_RPC_TYPE type;        /**< NC_RPC_VALIDATE */
    NC_DATASTORE source;
    char *url_config_src;    /**< either URL (starts with alpha) or config (starts with '<') */
    char free;
};

struct nc_rpc_getschema {
    NC_RPC_TYPE type;        /**< NC_RPC_GETSCHEMA */
    char *identifier;        /**< requested model identifier */
    char *version;           /**< either YANG version (1.0/1.1) or revision date */
    char *format;            /**< model format */
    char free;
};

struct nc_rpc_subscribe {
    NC_RPC_TYPE type;        /**< NC_RPC_SUBSCRIBE */
    char *stream;            /**< stream name */
    char *filter;            /**< either XML subtree (starts with '<') or an XPath (starts with '/' or an alpha) */
    char *start;
    char *stop;
    char free;
};

#endif /* NC_MESSAGES_P_H_ */
