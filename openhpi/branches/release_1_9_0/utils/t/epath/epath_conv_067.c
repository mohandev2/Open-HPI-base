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
 * Authors:
 *     Sean Dague <http://dague.net/sean>
 */

#include <string.h>

#include <SaHpi.h>
#include <oh_utils.h>

/**
 * main: epathstr -> epath test
 * 
 * This test tests whether an entity path string is converted into
 * an entity path properly.  
 *
 * Return value: 0 on success, 1 on failure
 **/
int main(int argc, char **argv) 
{
        char new[255];
        SaHpiEntityPathT tmp_ep;
        char *entity_root = "{PROCESSOR_BOARD,77}{DRIVE_BACKPLANE,65}";
        
        string2entitypath(entity_root, &tmp_ep);
         
        if (tmp_ep.Entry[0].EntityType != SAHPI_ENT_DRIVE_BACKPLANE) {
	    printf("ERROR! Testcase failed Entry[0].EntityType compare\n");
	    printf("Received=%d; Expected=%d\n", tmp_ep.Entry[0].EntityType, SAHPI_ENT_DRIVE_BACKPLANE);
	    return -1;
	}
                
        if (tmp_ep.Entry[0].EntityLocation != 65) {
	    printf("ERROR! Testcase failed Entry[0].EntityLocation compare\n");
	    printf("Received=%d; Expected=%d\n", tmp_ep.Entry[0].EntityLocation, 65);
	    return -1;
	}
        
        if (tmp_ep.Entry[1].EntityType != SAHPI_ENT_PROCESSOR_BOARD) {
	    printf("ERROR! Testcase failed Entry[1].EntityType compare\n");
	    printf("Received=%d; Expected=%d\n", tmp_ep.Entry[1].EntityType, SAHPI_ENT_PROCESSOR_BOARD);
	    return -1;
	}
        
        if (tmp_ep.Entry[1].EntityLocation != 77) {
	    printf("ERROR! Testcase failed Entry[1].EntityLocation compare\n");
	    printf("Received=%d; Expected=%d\n", tmp_ep.Entry[1].EntityLocation, 77);
	    return -1;
	}

        if (entitypath2string(&tmp_ep, new, 255) < 0) {
	    printf("ERROR! Testcase failed entitypath2string\n");
	    return -1;
	}
        
        if (strcmp(new, entity_root) != 0) {
	    printf("ERROR! Testcase failed strcmp\n");
	    printf("Received string=%s\n", new);
	    return -1;
	}

        return 0;
}
