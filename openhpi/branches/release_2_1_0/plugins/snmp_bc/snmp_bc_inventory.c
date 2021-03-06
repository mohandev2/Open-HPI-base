/*      -*- linux-c -*-
 *
 * (C) Copyright IBM Corp. 2003, 2005
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Author(s):
 *      peter d phan  <pdphan@sourceforge.net>
 *      Renier Morales <renierm@users.sf.net>
 *
 *	07/19/04 HPI-B Inventory   
 */

#include <snmp_bc_plugin.h>

static
SaErrorT snmp_bc_build_idr( void *hnd,
               SaHpiResourceIdT         ResourceId,
               SaHpiIdrIdT              IdrId,
               struct bc_inventory_record 	*i_record);

static	       
SaErrorT snmp_bc_idr_build_field(struct snmp_bc_hnd *custom_handle,
		SaHpiEntityPathT *ep,
		const gchar *oidstr,
		SaHpiIdrFieldT  *thisField,
		struct bc_idr_area *thisInventoryArea);

/************************************************************************/
/* Inventory functions   						*/
/************************************************************************/

/**
 * snmp_bc_get_idr_info:
 * @hnd: Pointer to handler's data
 * @ResourceId: Resource identifier for this operation 
 * @IdrId:  Identifier for the Inventory Data Repository
 * @IdrInfo: Pointer to the information describing the requested Inventory Data Repository
 *
 * Build the Inventory Data Record for the inputed resource id, idr id.
 * Copy the IdrInfo found for the input resource id and idr id
 *
 * Return value:
 * SA_OK - Normal 
 * SA_ERR_HPI_INVALID_PARAMS - NULL input pointers, hnd or IdrInfo
 * SA_ERR_HPI_NOT_PRESENT - If can not find idr with matched requested IdrId 
 **/
SaErrorT snmp_bc_get_idr_info( void *hnd,  
		SaHpiResourceIdT        ResourceId,
		SaHpiIdrIdT             IdrId,
		SaHpiIdrInfoT          *IdrInfo)
{
	SaErrorT  rv = SA_OK;
	struct bc_inventory_record *i_record;
	struct oh_handler_state *handle = (struct oh_handler_state *)hnd;

	if (!hnd || !IdrInfo)
		return(SA_ERR_HPI_INVALID_PARAMS);
	struct snmp_bc_hnd *custom_handle = (struct snmp_bc_hnd *)handle->data;
		
	i_record = (struct bc_inventory_record *)g_malloc0(sizeof(struct bc_inventory_record));
 	if (!i_record) {
  		dbg("Cannot allocate working buffer memory");
		return(SA_ERR_HPI_OUT_OF_MEMORY);
	}
	
	snmp_bc_lock_handler(custom_handle);
	rv = snmp_bc_build_idr(hnd, ResourceId, IdrId, i_record);
		
	if (rv == SA_OK) {
		if (IdrId == i_record->idrinfo.IdrId) 
			memcpy(IdrInfo, &(i_record->idrinfo), sizeof(SaHpiIdrInfoT));
		else 
			rv = SA_ERR_HPI_NOT_PRESENT;
	}

	g_free(i_record);
	snmp_bc_unlock_handler(custom_handle);
	return rv;
}

/**
 * snmp_bc_get_idr_area_header:
 * @hnd: Pointer to handler's data
 * @ResourceId: Resource identifier for this operation 
 * @IdrId:  Identifier for the Inventory Data Repository
 * @AreaType: Type of Inventory Data Area
 * @AreaId: Identifier of Area entry to retrieve from the IDR
 * @NextAreaId: Pointer to location to store the AreaId of the next area
 *              of the requested type within the IDR  
 * @Header: Pointer to Inventory Data Area Header into which the header 
 	    information is placed
 *
 * Build the Inventory Data Record for the inputed resource id, idr id.
 * Copy the Inventory Data Area Header to the space provided by user.
 *
 * Internal code makes an assumption that there is only one (1) Idr per
 * resource in snmp_bc plugin. For this to be expanded to more than one 
 * Idr per resource, the base bc_resources.c/h has to be changed together
 * with the rest of inventory code.
 * 
 * Return value:
 * SA_OK - Normal 
 * SA_ERR_HPI_INVALID_PARAMS - NULL input pointers, hnd or NextAreaId or Header
 * SA_ERR_HPI_OUT_OF_MEMORY - If can not allocate temp work space
 * SA_ERR_HPI_NOT_PRESENT - If can not find idr area with matched requested AreaId 
 **/
SaErrorT snmp_bc_get_idr_area_header( void *hnd,
		SaHpiResourceIdT         ResourceId,
		SaHpiIdrIdT              IdrId,
		SaHpiIdrAreaTypeT        AreaType,
		SaHpiEntryIdT            AreaId,
		SaHpiEntryIdT           *NextAreaId,
		SaHpiIdrAreaHeaderT     *Header)
{

	SaErrorT rv = SA_OK;
	struct bc_inventory_record *i_record;
	struct oh_handler_state *handle = (struct oh_handler_state *)hnd;

	if (!hnd || !NextAreaId || !Header)
		return(SA_ERR_HPI_INVALID_PARAMS);
	
	struct snmp_bc_hnd *custom_handle = (struct snmp_bc_hnd *)handle->data;
	i_record = (struct bc_inventory_record *)g_malloc0(sizeof(struct bc_inventory_record));
 	if (!i_record) {
  		dbg("Cannot allocate working buffer memory");
		return(SA_ERR_HPI_OUT_OF_MEMORY);
	}
	
	snmp_bc_lock_handler(custom_handle);
	rv = snmp_bc_build_idr(hnd, ResourceId, IdrId, i_record);
		
	if (rv == SA_OK) {
		rv = SA_ERR_HPI_NOT_PRESENT;		
		if (IdrId == i_record->idrinfo.IdrId) {
			if ( (i_record->area[0].idrareas.Type == AreaType) ||
					(SAHPI_IDR_AREATYPE_UNSPECIFIED == AreaType) )
			{  
				if ( (i_record->area[0].idrareas.AreaId == AreaId) || 
					(SAHPI_FIRST_ENTRY == AreaId) )
				{
					memcpy(Header, &(i_record->area[0].idrareas), sizeof(SaHpiIdrAreaHeaderT));
					*NextAreaId = SAHPI_LAST_ENTRY;
					rv = SA_OK;
				}				
			}
		}
	}
	g_free(i_record);
	snmp_bc_unlock_handler(custom_handle);
	return (rv);

}


/**
 * snmp_bc_add_idr_area:
 * @hnd: Pointer to handler's data
 * @ResourceId: Resource identifier for this operation 
 * @IdrId:  Identifier for the Inventory Data Repository
 * @AreaType: Type of Inventory Data Area
 * @AreaId: Pointer to store the identifier of the newly allocated Inventory Area
 *
 * This function is not suported/implemented for snmp_bc plugin
 * 
 * Return value:
 * SA_ERR_HPI_READ_ONLY - Normal - snmp_bc does not allow Inventory Update 
 **/
SaErrorT snmp_bc_add_idr_area( void *hnd,
		SaHpiResourceIdT         ResourceId,
		SaHpiIdrIdT              IdrId,
		SaHpiIdrAreaTypeT        AreaType,
		SaHpiEntryIdT           *AreaId)

{
	return SA_ERR_HPI_READ_ONLY;
}


/**
 * snmp_bc_del_idr_area:
 * @hnd: Pointer to handler's data
 * @ResourceId: Resource identifier for this operation 
 * @IdrId:  Identifier for the Inventory Data Repository
 * @AreaId: Identifier of Area entry to delete from the IDR
 *
 * This function is not suported/implemented for snmp_bc plugin
 * 
 * Return value:
 * SA_ERR_HPI_READ_ONLY - Normal - snmp_bc does not allow Inventory Update 
 **/
SaErrorT snmp_bc_del_idr_area( void *hnd,
		SaHpiResourceIdT       ResourceId,
		SaHpiIdrIdT            IdrId,
		SaHpiEntryIdT          AreaId)
{
	return SA_ERR_HPI_READ_ONLY;
}


/**
 * snmp_bc_get_idr_field:
 * @hnd: Pointer to handler's data
 * @ResourceId: Resource identifier for this operation 
 * @IdrId:  Identifier for the Inventory Data Repository
 * @AreaId: Identifier of Area for the IDA
 * @FieldType: Type of Inventory Data Field
 * @FieldId: Identier of Field to retrieve from the IDA
 * @NextFieldId: Pointer to location to store the FieldId
 *               of the next field of the requested type in IDA
 * @Field: Pointer to Inventory Data Field into which the field information will be placed.
 *
 * Return value:
 * SA_OK - Normal 
 * SA_ERR_HPI_INVALID_PARAMS - NULL input pointers, hnd or IdrInfo
 * SA_ERR_HPI_NOT_PRESENT - If can not find requested field 
 **/
SaErrorT snmp_bc_get_idr_field( void *hnd,
		SaHpiResourceIdT       ResourceId,
		SaHpiIdrIdT             IdrId,
		SaHpiEntryIdT           AreaId,
		SaHpiIdrFieldTypeT      FieldType,
		SaHpiEntryIdT           FieldId,
		SaHpiEntryIdT          *NextFieldId,
		SaHpiIdrFieldT         *Field)
{
	SaErrorT rv = SA_OK;
	struct bc_inventory_record *i_record;
	int i;
	SaHpiBoolT foundit = SAHPI_FALSE;
	struct oh_handler_state *handle = (struct oh_handler_state *)hnd;

	if (!hnd || !NextFieldId || !Field)
		return(SA_ERR_HPI_INVALID_PARAMS);
		
	struct snmp_bc_hnd *custom_handle = (struct snmp_bc_hnd *)handle->data;
	i_record = (struct bc_inventory_record *)g_malloc0(sizeof(struct bc_inventory_record));
	
 	if (!i_record) {
  		dbg("Cannot allocate working buffer memory");
		return(SA_ERR_HPI_OUT_OF_MEMORY);
	}
	
	snmp_bc_lock_handler(custom_handle);
	rv = snmp_bc_build_idr(hnd, ResourceId, IdrId, i_record);
		
	if (rv == SA_OK) {
		rv = SA_ERR_HPI_NOT_PRESENT;
		if (i_record->area[0].idrareas.AreaId == AreaId) {
			/* Search for fieldId here */
			for (i=0; i < i_record->area[0].idrareas.NumFields; i++) {
				if ( ((i_record->area[0].field[i].FieldId == FieldId) ||
								 (SAHPI_FIRST_ENTRY == FieldId)) 
                                   && ((i_record->area[0].field[i].Type == FieldType) || 
				   		(SAHPI_IDR_FIELDTYPE_UNSPECIFIED == FieldType)) )
				{
					memcpy(Field, &(i_record->area[0].field[i]), sizeof(SaHpiIdrFieldT));
					foundit = SAHPI_TRUE;
					rv = SA_OK;
					break;
				}
			}
			
			*NextFieldId = SAHPI_LAST_ENTRY;
			i++;
			if (foundit) {
                                if (i < i_record->area[0].idrareas.NumFields) {
                                        do { 
                                                if ((i_record->area[0].field[i].Type == FieldType) || 
								(SAHPI_IDR_FIELDTYPE_UNSPECIFIED == FieldType))
                                                {
                                                        *NextFieldId = i_record->area[0].field[i].FieldId;                                         
                                                        break;
                                                }
                                                i++;
                                        } while (i < i_record->area[0].idrareas.NumFields);                                        
                                }
                                        
			}
		}
	}

	g_free(i_record);
	snmp_bc_unlock_handler(custom_handle);
	return rv;
}


/**
 * snmp_bc_add_idr_field:
 * @hnd: Pointer to handler's data
 * @ResourceId: Resource identifier for this operation 
 * @IdrId:  Identifier for the Inventory Data Repository
 * @Field: Pointer to Inventory Data Field which contains field information to be added.
 *
 * This function is not suported/implemented for snmp_bc plugin
 * 
 * Return value:
 * SA_ERR_HPI_READ_ONLY - Normal - snmp_bc does not allow Inventory Update 
 **/
SaErrorT snmp_bc_add_idr_field( void *hnd,
		SaHpiResourceIdT         ResourceId,
		SaHpiIdrIdT              IdrId,
		SaHpiIdrFieldT        *Field)
{
	return SA_ERR_HPI_READ_ONLY;
}


/**
 * snmp_bc_set_idr_field:
 * @hnd: Pointer to handler's data
 * @ResourceId: Resource identifier for this operation 
 * @IdrId:  Identifier for the Inventory Data Repository
 * @Field: Pointer to Inventory Data Field which contains updated field information.
 *
 * This function is not suported/implemented for snmp_bc plugin
 * 
 * Return value:
 * SA_ERR_HPI_READ_ONLY - Normal - snmp_bc does not allow Inventory Update 
 **/
SaErrorT snmp_bc_set_idr_field( void *hnd,
		SaHpiResourceIdT         ResourceId,
		SaHpiIdrIdT              IdrId,
		SaHpiIdrFieldT           *Field)
{
	return SA_ERR_HPI_READ_ONLY;
}


/**
 * snmp_bc_del_idr_field:
 * @hnd: Pointer to handler's data
 * @ResourceId: Resource identifier for this operation 
 * @IdrId:  Identifier for the Inventory Data Repository
 * @AreaId: Identifier of Inventory Area whose field is to bo deleted
 * @FieldId: Identifier of field to be deleted
 *
 * This function is not suported/implemented for snmp_bc plugin
 * 
 * Return value:
 * SA_ERR_HPI_READ_ONLY - Normal - snmp_bc does not allow Inventory Update 
 **/
SaErrorT snmp_bc_del_idr_field( void *hnd, 
		SaHpiResourceIdT         ResourceId,
		SaHpiIdrIdT              IdrId,
		SaHpiEntryIdT            AreaId,
		SaHpiEntryIdT            FieldId)
{
	return SA_ERR_HPI_READ_ONLY;
}

/**
 * snmp_bc_build_idr:
 * @hnd: Pointer to handler's data
 * @ResourceId: Resource identifier for this operation 
 * @IdrId:  Identifier for the Inventory Data Repository
 * @i_record: Pointer into which inventory data is stored
 * 	
 * Build the complete Inventory Record for the resource identifier 
 *
 * Return value:
 * SA_OK - Normal
 * SA_ERR_HPI_INVALID_PARAMS - If any in pointer is NULL
 * SA_ERR_HPI_NOT_PRESENT - If Inventory RDR is not found in rptcache
 * 
 **/
static
SaErrorT snmp_bc_build_idr( void *hnd, 
		SaHpiResourceIdT  ResourceId,
		SaHpiIdrIdT       IdrId,
		struct bc_inventory_record *i_record)
{
	SaErrorT rv = SA_OK;	

	if (!hnd || !i_record)
		return(SA_ERR_HPI_INVALID_PARAMS);
	
	struct oh_handler_state *handle = (struct oh_handler_state *) hnd;
	struct snmp_bc_hnd *custom_handle = handle->data;
	SaHpiRdrT *rdr = oh_get_rdr_by_type(handle->rptcache, ResourceId, SAHPI_INVENTORY_RDR, IdrId);
	
	struct snmp_value get_value;
	
	/* Local work spaces */
	SaHpiIdrFieldT	thisField;
	struct bc_idr_area  thisInventoryArea;

	
	
	if (rdr != NULL) {
	
		struct InventoryInfo *s =
                        (struct InventoryInfo *)oh_get_rdr_data(handle->rptcache, ResourceId, rdr->RecordId);

		
		i_record->idrinfo.IdrId = IdrId;
		i_record->idrinfo.UpdateCount = 0;
		i_record->idrinfo.ReadOnly = SAHPI_TRUE;
		i_record->idrinfo.NumAreas = 1; /* pdp - expand to 2 for chassis HW & Firmware */
		
		thisInventoryArea.idrareas.AreaId = 1;
		thisInventoryArea.idrareas.Type = s->mib.area_type;
		thisInventoryArea.idrareas.ReadOnly = SAHPI_TRUE;
		thisInventoryArea.idrareas.NumFields = 0; /* Increment it as we find field */		
				
		thisField.AreaId = thisInventoryArea.idrareas.AreaId;
		thisField.ReadOnly = SAHPI_TRUE;
		thisField.Field.Language = SAHPI_LANG_ENGLISH; /*  SaHpiLanguageT */

		/**
		 *
		 */
		thisField.FieldId = 1;
		thisField.Type = SAHPI_IDR_FIELDTYPE_CHASSIS_TYPE;

		if(s->mib.oid.OidChassisType != NULL) {
			rv = snmp_bc_idr_build_field(custom_handle, &(rdr->Entity),
					s->mib.oid.OidChassisType, &thisField, &thisInventoryArea);
			if (rv != SA_OK)
				dbg("Having problem building Chassis Idr Field, continue to next field.\n");
			
		}

		/**
		 *
		 */
		memset(thisField.Field.Data, 0, SAHPI_MAX_TEXT_BUFFER_LENGTH);		
		thisField.FieldId = 2;
		thisField.Type = SAHPI_IDR_FIELDTYPE_MFG_DATETIME;
		thisField.Field.DataLength = 0; /* SaHpiUint8T  */
		
		if(s->mib.oid.OidMfgDateTime == NULL){
			thisField.Field.DataLength = sizeof("SAHPI_TIME_UNSPECIFIED"); /* SaHpiUint8T  */	
			thisField.Field.DataType = SAHPI_TL_TYPE_TEXT; /* SaHpiTextTypeT */
			strcpy((char *)thisField.Field.Data,"SAHPI_TIME_UNSPECIFIED");
		
		} else {
                        rv = snmp_bc_oid_snmp_get(custom_handle, &(rdr->Entity),s->mib.oid.OidMfgDateTime, &get_value, SAHPI_TRUE);
                        if(rv != SA_OK) {
                                dbg("SNMP could not read %s; Type=%d.\n",s->mib.oid.OidMfgDateTime,get_value.type);
                                return rv;
                        } else if((rv == SA_OK) && (get_value.type == ASN_OCTET_STR )) {
				thisField.Field.DataLength = get_value.str_len;
				thisField.Field.DataType = SAHPI_TL_TYPE_TEXT;
				memcpy(thisField.Field.Data, get_value.string, get_value.str_len); 
                        } else 
                                dbg("%s Invalid type for MfgDateTime inventory data\n",s->mib.oid.OidMfgDateTime);
        	}
		
		if (thisField.Field.DataLength != 0) {
			memcpy(&thisInventoryArea.field[thisInventoryArea.idrareas.NumFields], &thisField, sizeof(SaHpiIdrFieldT));
			thisInventoryArea.idrareas.NumFields++;
		}
		
		/**
		 *
		 */
		thisField.FieldId = 3;
		thisField.Type = SAHPI_IDR_FIELDTYPE_MANUFACTURER;

		if(s->mib.oid.OidManufacturer != NULL) {
			rv = snmp_bc_idr_build_field(custom_handle, &(rdr->Entity),
					s->mib.oid.OidManufacturer, &thisField, &thisInventoryArea);
			if (rv != SA_OK)
				dbg("Having problem building ManufacturerId Idr Field, continue to next field.\n");
		}
			
		/**
		 *
		 */

		thisField.FieldId = 4;
		thisField.Type = SAHPI_IDR_FIELDTYPE_PRODUCT_NAME;

		if(s->mib.oid.OidProductName != NULL) {
			rv = snmp_bc_idr_build_field(custom_handle, &(rdr->Entity),
					s->mib.oid.OidProductName, &thisField, &thisInventoryArea);
			if (rv != SA_OK)
				dbg("Having problem building ProductName Idr Field, continue to next field.\n");
		}

		/**
		 *
		 */
		
		thisField.FieldId = 5;
		thisField.Type = SAHPI_IDR_FIELDTYPE_PRODUCT_VERSION;

		if(s->mib.oid.OidProductVersion != NULL) {
			rv = snmp_bc_idr_build_field(custom_handle, &(rdr->Entity),
				s->mib.oid.OidProductVersion, &thisField, &thisInventoryArea);
			if (rv != SA_OK)
				dbg("Having problem building ProductVersion Idr Field, continue to next field.\n");
		}
			
		/**
		 *
		 */
	
		thisField.FieldId = 6;
		thisField.Type = SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER;

		if(s->mib.oid.OidSerialNumber != NULL) {
			rv = snmp_bc_idr_build_field(custom_handle, &(rdr->Entity),
				s->mib.oid.OidSerialNumber, &thisField, &thisInventoryArea);
			if (rv != SA_OK)
				dbg("Having problem building SerialNumber Idr Field, continue to next field.\n");
		}
			
		/**
		 *
		 */
		
		thisField.FieldId = 7;
		thisField.Type = SAHPI_IDR_FIELDTYPE_PART_NUMBER;

		if(s->mib.oid.OidPartNumber != NULL) {
			rv = snmp_bc_idr_build_field(custom_handle, &(rdr->Entity),
				s->mib.oid.OidPartNumber, &thisField, &thisInventoryArea);
				if (rv != SA_OK)
					dbg("Having problem building PartNumber Idr Field, continue to next field.\n");
		}
			
		/**
		 *
		 */
			
		thisField.FieldId = 8;
		thisField.Type = SAHPI_IDR_FIELDTYPE_FILE_ID;

		if(s->mib.oid.OidFileId != NULL) {
			rv = snmp_bc_idr_build_field(custom_handle, &(rdr->Entity),
				s->mib.oid.OidFileId, &thisField, &thisInventoryArea);
				if (rv != SA_OK)
					dbg("Having problem building FileID Idr Field, continue to next field.\n");
		}
			
		/**
		 *
		 */

		thisField.FieldId = 9;
		thisField.Type = SAHPI_IDR_FIELDTYPE_ASSET_TAG;

		if(s->mib.oid.OidAssetTag != NULL) {
			rv = snmp_bc_idr_build_field(custom_handle, &(rdr->Entity),
				s->mib.oid.OidAssetTag, &thisField, &thisInventoryArea);
				if (rv != SA_OK)
					dbg("Having problem building AssetTag Idr Field, continue ...\n");
		}
			
		memcpy( &(i_record->area[0]), &thisInventoryArea, sizeof(struct bc_idr_area));

	} else {
		rv = SA_ERR_HPI_NOT_PRESENT;
	}
	return rv;
}

/**
 * snmp_bc_idr_build_field:
 * @custom_handle: snmp_bc custom handler data
 * @oid: SNMP Object Id of the BC Inventory object
 * @thisField: Pointer to Inventory Field under construction
 * @thisInventoryArea: Pointer to Inventory Record under contruction
 * 
 * Get data from target snmp agent for the corresponding oid.
 * Construct IDR Field for the retrieved data and place in Inventory Record
 *
 * Return value:
 * SA_OK - Normal
 * SA_ERR_HPI_INVALID_PARAMS - If any in pointer is NULL
 * SA_ERR_HPI_INTERNAL_ERROR - If can not process get_value.type from bc snmp agent
 * 
 **/

static
SaErrorT snmp_bc_idr_build_field(struct snmp_bc_hnd *custom_handle,
				 SaHpiEntityPathT *ep,
				 const gchar *oidstr,
		  		 SaHpiIdrFieldT  *thisField,
				 struct bc_idr_area *thisInventoryArea)
{

	if (!custom_handle || !thisField || !thisInventoryArea)
		return(SA_ERR_HPI_INVALID_PARAMS);

	struct snmp_value get_value;
	SaErrorT rv = SA_OK;

	/**
	 *
	 */
	memset(thisField->Field.Data, 0, SAHPI_MAX_TEXT_BUFFER_LENGTH);		
	thisField->Field.DataLength = 0; /* SaHpiUint8T  */	

        rv = snmp_bc_oid_snmp_get(custom_handle, ep, oidstr, &get_value, SAHPI_TRUE);
	if(rv != SA_OK) {	
		dbg("SNMP could not read %s; Type=%d.\n",oidstr ,get_value.type);		
                return(rv);
	} else {
		if( get_value.type == ASN_OCTET_STR ) {
			thisField->Field.DataLength = get_value.str_len;
			thisField->Field.DataType = SAHPI_TL_TYPE_TEXT;
			memcpy(thisField->Field.Data, get_value.string, get_value.str_len); 
		} else if ( get_value.type == ASN_INTEGER ){
			thisField->Field.DataLength = sizeof(long);
			thisField->Field.DataType = SAHPI_TL_TYPE_TEXT;
			snprintf((char *)thisField->Field.Data, SAHPI_MAX_TEXT_BUFFER_LENGTH,
				 "%ld",get_value.integer );
		} else
			dbg("%s Invalid data type for Chassis data\n",oidstr);
	}

		
	if (thisField->Field.DataLength != 0) {
		memcpy(&thisInventoryArea->field[thisInventoryArea->idrareas.NumFields], 
							thisField, sizeof(SaHpiIdrFieldT));
		thisInventoryArea->idrareas.NumFields++;
	}

	return(SA_OK);

}
