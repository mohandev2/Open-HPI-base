/* -*- linux-c -*-
 * 
 * (C) Copyright IBM Corp. 2004
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Author(s):
 *     Peter D Phan <pdphan@users.sourceforge.net>
 */


#include <snmp_bc_plugin.h>
#include <sahpimacros.h>
#include <tsetup.h>

int main(int argc, char **argv) 
{

	/* ************************
	 * Local variables
	 * ***********************/	 
	int testfail = 0;
	SaErrorT          err;
	SaErrorT expected_err;
	SaHpiRptEntryT rptentry;				
	SaHpiResourceIdT  id = 0;
        SaHpiSessionIdT sessionid;
	SaHpiIdrIdT       idrId = 0;
	SaHpiEntryIdT     areaId = 0;
	SaHpiEntryIdT     fieldId = 0;
	SaHpiIdrFieldT    field; 
	memset (&field, 0, sizeof(SaHpiIdrFieldT));	
	
	struct oh_handler_state l_handle;
	memset(&l_handle, 0, sizeof(struct oh_handler_state));

	/* *************************************	 	 
	 * Find a resource with inventory capability
	 * ************************************* */
	err = tsetup(&sessionid);
	if (err != SA_OK) {
		printf("Error! Can not open session for test environment\n");
		printf("       File=%s, Line=%d\n", __FILE__, __LINE__);
		return -1;
	}

	err = tfind_resource(&sessionid, SAHPI_CAPABILITY_INVENTORY_DATA, SAHPI_FIRST_ENTRY, &rptentry, SAHPI_TRUE);
	if (err != SA_OK) {
		printf("Can not find an Inventory resource for test environment\n");
		printf("       File=%s, Line=%d\n", __FILE__, __LINE__);
		err = tcleanup(&sessionid);
		return SA_OK;
	}
	
	id = rptentry.ResourceId;
	/************************** 
	 * Test: Add to a read-only Idr Area
	 **************************/
	expected_err = SA_ERR_HPI_READ_ONLY;                   
	err = saHpiIdrAreaAdd(sessionid, id, idrId, SAHPI_IDR_AREATYPE_UNSPECIFIED, &areaId);
	checkstatus(err, expected_err, testfail);

	/************************** 
	 * Test: Add to a read-only Idr Field
	 * expected_err = SA_ERR_HPI_READ_ONLY;                   
	 **************************/
	err = saHpiIdrFieldAdd(sessionid, id, idrId, &field);
	checkstatus(err, expected_err, testfail);

	/************************** 
	 * Test: Del a read-only Idr Area
	 * expected_err = SA_ERR_HPI_READ_ONLY;                   
	 **************************/
	err = saHpiIdrAreaDelete(sessionid, id, idrId, areaId);
	checkstatus(err, expected_err, testfail);

	/************************** 
	 * Test: Del a read-only Idr Field
	 * expected_err = SA_ERR_HPI_READ_ONLY;                   
	 **************************/
	err = saHpiIdrFieldDelete(sessionid, id, idrId, areaId, fieldId);
	checkstatus(err, expected_err, testfail);

	/************************** 
	 * Test: Set a read-only Idr Field
	 * expected_err = SA_ERR_HPI_READ_ONLY;                   
	 **************************/
	err = saHpiIdrFieldSet(sessionid, id, idrId, &field); 
	checkstatus(err, expected_err, testfail);

	/**************************&*
	 * Cleanup after all tests
	 ***************************/
	err = tcleanup(&sessionid);
	return testfail;

}

#include <tsetup.c>
