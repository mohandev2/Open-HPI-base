/*
 * (C) Copyright IBM Corp. 2004
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *   Konrad Rzeszutek <konradr@us.ibm.com>

 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.array-user.conf,v 5.18 2003/11/07 17:03:52 rstory Exp $
 *
 * $Id$
 *
 */
#ifndef SAHPISENSORTHDLOWCRITICALTABLE_H
#define SAHPISENSORTHDLOWCRITICALTABLE_H

#ifdef __cplusplus
extern "C"
{
#endif


#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>
#include <SaHpi.h>
#include <hpiSubagent.h>


	/** Index saHpiDomainID is external */
	/** Index saHpiResourceID is external */
	/** Index saHpiSensorIndex is external */

  typedef struct saHpiSensorThdLowCriticalTable_context_s
  {
    netsnmp_index index;

    /** TruthValue = ASN_INTEGER */
    long            saHpiSensorThdLowCriticalIsReadable;
    
    /** TruthValue = ASN_INTEGER */
    long            saHpiSensorThdLowCriticalIsWritable;
    
    /** TruthValue = ASN_INTEGER */
    long            saHpiSensorThdLowCriticalIsFixed;
    
    /** INTEGER = ASN_INTEGER */
    long            saHpiSensorThdLowCriticalValuesPresent;

    /** UNSIGNED32 = ASN_UNSIGNED */
    unsigned long   saHpiSensorThdLowCriticalRaw;

	/** OCTETSTR = ASN_OCTET_STR */
    unsigned char
      saHpiSensorThdLowCriticalInterpreted[SENSOR_THD_INTER_MAX];
    size_t saHpiSensorThdLowCriticalInterpreted_len;



    long sensor_id;
    long resource_id;
    long domain_id;
    long hash;


  } saHpiSensorThdLowCriticalTable_context;

/*************************************************************
 * function declarations
 */

  void initialize_table_saHpiSensorThdLowCriticalTable (void);

  int
    delete_ThdLowCritical_row (SaHpiDomainIdT domain_id,
			       SaHpiResourceIdT resource_id,
			       SaHpiSensorNumT num);

  int populate_ThdLowCritical (SaHpiDomainIdT domain_id,
			       SaHpiResourceIdT resource_id,
			       SaHpiSensorNumT sensor_id,
			       SaHpiSensorThdDefnT *threshold_def,
			       SaHpiSensorReadingT * reading);


  int set_ThdLowCritical (saHpiSensorThdLowCriticalTable_context *ctx);

  int
    saHpiSensorThdLowCriticalTable_get_value (netsnmp_request_info *,
					      netsnmp_index *,
					      netsnmp_table_request_info *);


/*************************************************************
 * oid declarations
 */
  extern oid saHpiSensorThdLowCriticalTable_oid[];
  extern size_t saHpiSensorThdLowCriticalTable_oid_len;

#define saHpiSensorThdLowCriticalTable_TABLE_OID hpiSensor_OID,8

/*************************************************************
 * column number definitions for table saHpiSensorThdLowCriticalTable
 */
#define COLUMN_SAHPISENSORTHDLOWCRITICALISREADABLE 1
#define COLUMN_SAHPISENSORTHDLOWCRITICALISWRITABLE 2
#define COLUMN_SAHPISENSORTHDLOWCRITICALISFIXED 3
#define COLUMN_SAHPISENSORTHDLOWCRITICALVALUESPRESENT 4
#define COLUMN_SAHPISENSORTHDLOWCRITICALRAW 5
#define COLUMN_SAHPISENSORTHDLOWCRITICALINTERPRETED 6
#define saHpiSensorThdLowCriticalTable_COL_MIN 1
#define saHpiSensorThdLowCriticalTable_COL_MAX 6

  int
    saHpiSensorThdLowCriticalTable_extract_index
    (saHpiSensorThdLowCriticalTable_context * ctx, netsnmp_index * hdr);

  void saHpiSensorThdLowCriticalTable_set_reserve1 (netsnmp_request_group *);
  void saHpiSensorThdLowCriticalTable_set_reserve2 (netsnmp_request_group *);
  void saHpiSensorThdLowCriticalTable_set_action (netsnmp_request_group *);
  void saHpiSensorThdLowCriticalTable_set_commit (netsnmp_request_group *);
  void saHpiSensorThdLowCriticalTable_set_free (netsnmp_request_group *);
  void saHpiSensorThdLowCriticalTable_set_undo (netsnmp_request_group *);

    saHpiSensorThdLowCriticalTable_context
    * saHpiSensorThdLowCriticalTable_duplicate_row
    (saHpiSensorThdLowCriticalTable_context *);
    netsnmp_index
    * saHpiSensorThdLowCriticalTable_delete_row
    (saHpiSensorThdLowCriticalTable_context *);

  int
    saHpiSensorThdLowCriticalTable_can_delete
    (saHpiSensorThdLowCriticalTable_context * undo_ctx,
     saHpiSensorThdLowCriticalTable_context * row_ctx,
     netsnmp_request_group * rg);



    saHpiSensorThdLowCriticalTable_context
    * saHpiSensorThdLowCriticalTable_create_row (netsnmp_index *);

#ifdef __cplusplus
};
#endif

#endif /** SAHPISENSORTHDLOWCRITICALTABLE_H */