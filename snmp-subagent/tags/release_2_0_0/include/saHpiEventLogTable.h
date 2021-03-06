/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.array-user.conf,v 5.18.2.2 2004/02/09 18:21:47 rstory Exp $
 *
 * $Id$
 *
 * Yes, there is lots of code here that you might not use. But it is much
 * easier to remove code than to add it!
 */
#ifndef SAHPIEVENTLOGTABLE_H
#define SAHPIEVENTLOGTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>

        /** Index saHpiDomainId is external */
        /** Index saHpiResourceId is external */
        /** Index saHpiEventLogIndex is internal */

typedef struct saHpiEventLogTable_context_s {
    netsnmp_index index; /** THIS MUST BE FIRST!!! */

    /*************************************************************
     * You can store data internally in this structure.
     *
     * TODO: You will probably have to fix a few types here...
     */
    /** TODO: add storage for external index(s)! */
        /** SaHpiEntryId = ASN_UNSIGNED */
            unsigned long saHpiEventLogIndex;

        /** INTEGER = ASN_INTEGER */
            long saHpiEventLogType;

        /** SaHpiTime = ASN_COUNTER64 */
    /** TODO: Is this type correct? */
            long saHpiEventLogAddedTimestamp;

        /** RowPointer = ASN_OBJECT_ID */
            oid saHpiEventLogRowPointer[MAX_OID_LEN];
            long saHpiEventLogRowPointer_len;


    /*
     * OR
     *
     * Keep a pointer to your data
     */
    void * data;

    /*
     *add anything else you want here
     */

} saHpiEventLogTable_context;

/*************************************************************
 * function declarations
 */
void init_saHpiEventLogTable(void);
void initialize_table_saHpiEventLogTable(void);
const saHpiEventLogTable_context * saHpiEventLogTable_get_by_idx(netsnmp_index *);
const saHpiEventLogTable_context * saHpiEventLogTable_get_by_idx_rs(netsnmp_index *,
                                        int row_status);
int saHpiEventLogTable_get_value(netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);


/*************************************************************
 * oid declarations
 */
extern oid saHpiEventLogTable_oid[];
extern size_t saHpiEventLogTable_oid_len;

#define saHpiEventLogTable_TABLE_OID 1,3,6,1,4,1,18568,2,1,1,3,2,2
    
/*************************************************************
 * column number definitions for table saHpiEventLogTable
 */
#define COLUMN_SAHPIEVENTLOGINDEX 1
#define COLUMN_SAHPIEVENTLOGTYPE 2
#define COLUMN_SAHPIEVENTLOGADDEDTIMESTAMP 3
#define COLUMN_SAHPIEVENTLOGROWPOINTER 4
#define saHpiEventLogTable_COL_MIN 2
#define saHpiEventLogTable_COL_MAX 4

/* comment out the following line if you don't handle SET-REQUEST for saHpiEventLogTable */
#define saHpiEventLogTable_SET_HANDLING

/* comment out the following line if you can't create new rows */
#define saHpiEventLogTable_ROW_CREATION

/* comment out the following line if you don't want the secondary index */
#define saHpiEventLogTable_IDX2

/* uncommend the following line if you allow modifications to an
 * active row */
/** define saHpiEventLogTable_CAN_MODIFY_ACTIVE_ROW */

#ifdef saHpiEventLogTable_SET_HANDLING

int saHpiEventLogTable_extract_index( saHpiEventLogTable_context * ctx, netsnmp_index * hdr );

void saHpiEventLogTable_set_reserve1( netsnmp_request_group * );
void saHpiEventLogTable_set_reserve2( netsnmp_request_group * );
void saHpiEventLogTable_set_action( netsnmp_request_group * );
void saHpiEventLogTable_set_commit( netsnmp_request_group * );
void saHpiEventLogTable_set_free( netsnmp_request_group * );
void saHpiEventLogTable_set_undo( netsnmp_request_group * );

saHpiEventLogTable_context * saHpiEventLogTable_duplicate_row( saHpiEventLogTable_context* );
netsnmp_index * saHpiEventLogTable_delete_row( saHpiEventLogTable_context* );

int saHpiEventLogTable_can_activate(saHpiEventLogTable_context *undo_ctx,
                      saHpiEventLogTable_context *row_ctx,
                      netsnmp_request_group * rg);
int saHpiEventLogTable_can_deactivate(saHpiEventLogTable_context *undo_ctx,
                        saHpiEventLogTable_context *row_ctx,
                        netsnmp_request_group * rg);
int saHpiEventLogTable_can_delete(saHpiEventLogTable_context *undo_ctx,
                    saHpiEventLogTable_context *row_ctx,
                    netsnmp_request_group * rg);
    
    
#ifdef saHpiEventLogTable_ROW_CREATION
saHpiEventLogTable_context * saHpiEventLogTable_create_row( netsnmp_index* );
#endif
#endif

#ifdef saHpiEventLogTable_IDX2
saHpiEventLogTable_context * saHpiEventLogTable_get( const char *name, int len );
#endif

#ifdef __cplusplus
};
#endif

#endif /** SAHPIEVENTLOGTABLE_H */
