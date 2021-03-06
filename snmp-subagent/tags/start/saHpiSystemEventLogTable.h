/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.array-user.conf,v 5.15.2.1 2003/02/27 05:59:41 rstory Exp $
 *
 * $Id$
 *
 * Yes, there is lots of code here that you might not use. But it is much
 * easier to remove code than to add it!
 */
#ifndef SAHPISYSTEMEVENTLOGTABLE_H
#define SAHPISYSTEMEVENTLOGTABLE_H

#ifdef __cplusplus
extern          "C" {
#endif


#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>

        /** Index saHpiSystemEventLogEntryId is internal */

    typedef struct saHpiSystemEventLogTable_context_s {
        netsnmp_index   index;
                         /** THIS MUST BE FIRST!!! */

    /*************************************************************
     * You can store data internally in this structure.
     *
     * TODO: You will probably have to fix a few types here...
     */
        /** UNSIGNED32 = ASN_UNSIGNED */
        unsigned long   saHpiSystemEventLogEntryId;

        /** TimeStamp = ASN_TIMETICKS */
        unsigned long   saHpiSystemEventLogTimestamp;

        /** RowPointer = ASN_OBJECT_ID */
        oid             saHpiSystemEventLogged[MAX_OID_LEN];
        long            saHpiSystemEventLogged_len;


        /*
         * OR
         *
         * Keep a pointer to your data
         */
        void           *data;

        /*
         *add anything else you want here
         */

    } saHpiSystemEventLogTable_context;

/*************************************************************
 * function declarations
 */
    void            init_saHpiSystemEventLogTable(void);
    void            initialize_table_saHpiSystemEventLogTable(void);
    const saHpiSystemEventLogTable_context
        *saHpiSystemEventLogTable_get_by_idx(netsnmp_index *);
    const saHpiSystemEventLogTable_context
        *saHpiSystemEventLogTable_get_by_idx_rs(netsnmp_index *,
                                                int row_status);
    int             saHpiSystemEventLogTable_get_value(netsnmp_request_info
                                                       *, netsnmp_index *,
                                                       netsnmp_table_request_info
                                                       *);


/*************************************************************
 * oid declarations
 */
    extern oid      saHpiSystemEventLogTable_oid[];
    extern size_t   saHpiSystemEventLogTable_oid_len;

#define saHpiSystemEventLogTable_TABLE_OID 1,3,6,1,3,90,2,2,9

/*************************************************************
 * column number definitions for table saHpiSystemEventLogTable
 */
#define COLUMN_SAHPISYSTEMEVENTLOGENTRYID 1
#define COLUMN_SAHPISYSTEMEVENTLOGTIMESTAMP 2
#define COLUMN_SAHPISYSTEMEVENTLOGGED 3
#define saHpiSystemEventLogTable_COL_MIN 1
#define saHpiSystemEventLogTable_COL_MAX 3

    /*
     * comment out the following line if you don't handle SET-REQUEST for saHpiSystemEventLogTable 
     */
#define saHpiSystemEventLogTable_SET_HANDLING

    /*
     * comment out the following line if you can't create new rows 
     */
#define saHpiSystemEventLogTable_ROW_CREATION

    /*
     * comment out the following line if you don't want the secondary index 
     */
#define saHpiSystemEventLogTable_IDX2

#ifdef saHpiSystemEventLogTable_SET_HANDLING

    int            
        saHpiSystemEventLogTable_extract_index
        (saHpiSystemEventLogTable_context * ctx, netsnmp_index * hdr);

    void           
        saHpiSystemEventLogTable_set_reserve1(netsnmp_request_group *);
    void           
        saHpiSystemEventLogTable_set_reserve2(netsnmp_request_group *);
    void           
        saHpiSystemEventLogTable_set_action(netsnmp_request_group *);
    void           
        saHpiSystemEventLogTable_set_commit(netsnmp_request_group *);
    void            saHpiSystemEventLogTable_set_free(netsnmp_request_group
                                                      *);
    void            saHpiSystemEventLogTable_set_undo(netsnmp_request_group
                                                      *);

    saHpiSystemEventLogTable_context
        *saHpiSystemEventLogTable_duplicate_row
        (saHpiSystemEventLogTable_context *);
    netsnmp_index 
        *saHpiSystemEventLogTable_delete_row
        (saHpiSystemEventLogTable_context *);

    int            
        saHpiSystemEventLogTable_can_delete
        (saHpiSystemEventLogTable_context * undo_ctx,
         saHpiSystemEventLogTable_context * row_ctx,
         netsnmp_request_group * rg);


#ifdef saHpiSystemEventLogTable_ROW_CREATION
    saHpiSystemEventLogTable_context
        *saHpiSystemEventLogTable_create_row(netsnmp_index *);
#endif
#endif

#ifdef saHpiSystemEventLogTable_IDX2
    saHpiSystemEventLogTable_context *saHpiSystemEventLogTable_get(const
                                                                   char
                                                                   *name,
                                                                   int
                                                                   len);
#endif

#ifdef __cplusplus
};
#endif

#endif /** SAHPISYSTEMEVENTLOGTABLE_H */
