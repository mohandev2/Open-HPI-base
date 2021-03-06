/* -*- linux-c -*-
 *
 *
 * (C) Copyright IBM Corp 2003,2004
 * Authors:  
 *     Peter D.Phan pdphan@users.sourceforge.net
 *  
 *     01/13/2004 pdphan  module created
 *			  Change clock for event log on IBM Blade Center E.
 *     03/10/2004 pdphan  Remove reference to IBM Blade Center.
 */
/*
Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

  a.. Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer. 
  b.. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation 
      and/or other materials provided with the distribution. 
  c.. Neither the name of Intel Corporation nor the names of its contributors 
      may be used to endorse or promote products derived from this software 
      without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

#include <SaHpi.h>
#include <oh_utils.h>

char progver[] = "1.0";
int fdebug = 0;
int findate = 0;
int fintime = 0;

static void usage(char **argv)
{
	printf("Usage\n\t%s -d mm/dd/yyyy -t HH:MM:SS [-x]\n",argv[0]);
	printf("\twhere -d date in mm/dd/yyyy format\n");
	printf("\t      -t time of day in 24-hr format\n");
	printf("\t      -x displays eXtra debug messages\n");

	return;
}

int main(int argc, char **argv)
{
	int c;
	char i_newdate[20];
	char i_newtime[20];
	struct tm  new_tm_time;
	SaErrorT rv;
	SaHpiVersionT hpiVer;
	SaHpiSessionIdT sessionid;
	SaHpiRptInfoT rptinfo;
	SaHpiRptEntryT rptentry;
	SaHpiEntryIdT rptentryid;
	SaHpiEntryIdT nextrptentryid;
	SaHpiResourceIdT resourceid;
	SaHpiEventLogEntryIdT entryid;
	SaHpiTimeT oldtime;
	SaHpiTimeT newtime;
	SaHpiTimeT readbacktime;
	SaHpiTextBufferT buffer;
        
	printf("%s: version %s\n",argv[0],progver); 
        
	while ( (c = getopt( argc, argv,"d:t:x")) != EOF )
	{
		switch(c) {
			case 'd': 
				findate = 1;
				strcpy(i_newdate, optarg);
				break;
			case 't': 
				fintime = 1;
				strcpy(i_newtime, optarg);
				break;
			case 'x':
				fdebug = 1;
				break;
			default:
				usage(argv);
				exit(1);
		}
	}
	
	if ( !findate || !fintime) {
		usage(argv);
		exit(1);
	}

	if (findate) {
		if (fdebug) printf("New date to be set: %s\n",i_newdate);
	        sscanf(i_newdate,"%2d/%2d/%4d",
                  &new_tm_time.tm_mon, &new_tm_time.tm_mday, &new_tm_time.tm_year);
		new_tm_time.tm_mon --;
		new_tm_time.tm_year -= 1900;
	} else {
		printf("No Date\n");
	}

	if (fintime) {
		if (fdebug)  printf("New time to be set:  %s\n",i_newtime);
	        sscanf(i_newtime,"%2d:%2d:%2d",
                  &new_tm_time.tm_hour, &new_tm_time.tm_min, &new_tm_time.tm_sec);
	} else {
		printf("No Time\n");
	}

	if (fdebug) printf("Values passed to mktime():\n\tmon %d\n\tday %d\n\tyear %d\n\tHH %d\n\tMM %d\n\tSS %d\n",
			new_tm_time.tm_mon, new_tm_time.tm_mday, new_tm_time.tm_year,
			new_tm_time.tm_hour, new_tm_time.tm_min, new_tm_time.tm_sec);

	newtime = (SaHpiTimeT) mktime(&new_tm_time) * 1000000000;
	if (fdebug) printf("New date and time in SaHpiTimeT %lli\n", (long long int)newtime);
	rv = saHpiInitialize(&hpiVer);
	if (rv != SA_OK) {
		printf("saHpiInitialize: %s\n", oh_lookup_error(rv));
		exit(-1);
	}

	rv = saHpiSessionOpen(SAHPI_DEFAULT_DOMAIN_ID,&sessionid,NULL);
	if (rv != SA_OK) {
		if (rv == SA_ERR_HPI_ERROR) 
			printf("saHpiSessionOpen: error %d, SpiLibd not running\n",rv);
		else
			printf("saHpiSessionOpen: %s\n", oh_lookup_error(rv));
		exit(-1);
	}
 
	rv = saHpiResourcesDiscover(sessionid);
	if (fdebug) printf("saHpiResourcesDiscover %s\n", oh_lookup_error(rv));
	rv = saHpiRptInfoGet(sessionid,&rptinfo);
	if (fdebug) printf("saHpiRptInfoGet %s\n", oh_lookup_error(rv));
	printf("RptInfo: UpdateCount = %d, UpdateTime = %lx\n",
		rptinfo.UpdateCount, (unsigned long)rptinfo.UpdateTimestamp);
        
	/* walk the RPT list */
	rptentryid = SAHPI_FIRST_ENTRY;
	while ((rv == SA_OK) && (rptentryid != SAHPI_LAST_ENTRY))
	{
                rv = saHpiRptEntryGet(sessionid,rptentryid,&nextrptentryid,&rptentry);
                if (fdebug) printf("saHpiRptEntryGet %s\n", oh_lookup_error(rv));
                if ((rv == SA_OK) && (rptentry.ResourceCapabilities & SAHPI_CAPABILITY_SEL)) {
                        resourceid = rptentry.ResourceId;
                        if (fdebug) printf("RPT %x capabilities = %x\n", resourceid,
                                           rptentry.ResourceCapabilities);
			rv = saHpiEventLogTimeGet(sessionid, resourceid, &oldtime);
			oh_decode_time(oldtime, &buffer);
			printf ("\nCurrent event log time on HPI target: %s\n", buffer.Data);
			printf ("Setting new event log time on HPI target ...\n");
		 	rv = saHpiEventLogTimeSet(sessionid, resourceid, newtime);
			if (rv != SA_OK) 
			{
                		printf("saHpiEventLogTimeSet %s\n", oh_lookup_error(rv));
			}
			rv = saHpiEventLogTimeGet(sessionid, resourceid, &readbacktime);
			oh_decode_time(readbacktime, &buffer);
			printf ("Read-Back-Check event log time: %s\n", buffer.Data);

                }
                entryid = SAHPI_OLDEST_ENTRY;
                rptentryid = nextrptentryid;
	} 
        
        rv = saHpiSessionClose(sessionid);
        rv = saHpiFinalize();
        
        exit(0);
        return(0);
}
 
/* end hpisettime.c */
