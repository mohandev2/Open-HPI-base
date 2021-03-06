/*
 * Copyright (C) 2007-2008, Hewlett-Packard Development Company, LLP
 *                     All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Hewlett-Packard Corporation, nor the names
 * of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author(s)
 *      Raghavendra M.S. <raghavendra.ms@hp.com>
 */
#ifndef _OA_SOAP_ANNUNCIATOR_H
#define _OA_SOAP_ANNUNCIATOR_H

/* Include files */
#include <SaHpi.h>
#include <oh_error.h>

SaErrorT oa_soap_get_next_announce(void *oh_handler,
                                   SaHpiResourceIdT resource_id,
                                   SaHpiAnnunciatorNumT num,
                                   SaHpiSeverityT severity,
                                   SaHpiBoolT unacknowledged_only,
                                   SaHpiAnnouncementT *announcement);

SaErrorT oa_soap_get_announce(void *oh_handler,
                              SaHpiResourceIdT resource_id,
                              SaHpiAnnunciatorNumT num,
                              SaHpiEntryIdT entry,
                              SaHpiAnnouncementT *announcement);

SaErrorT oa_soap_ack_announce(void *oh_handler,
                              SaHpiResourceIdT resource_id,
                              SaHpiAnnunciatorNumT num,
                              SaHpiEntryIdT entry,
                              SaHpiSeverityT severity);

SaErrorT oa_soap_get_annunc_mode(void *oh_handler,
                                 SaHpiResourceIdT resource_id,
                                 SaHpiAnnunciatorNumT num,
                                 SaHpiAnnunciatorModeT *mode);

SaErrorT oa_soap_set_annunc_mode(void *oh_handler,
                                 SaHpiResourceIdT resource_id,
                                 SaHpiAnnunciatorNumT num,
                                 SaHpiAnnunciatorModeT mode);

SaErrorT oa_soap_del_announce(void *oh_handler,
                              SaHpiResourceIdT resource_id,
                              SaHpiAnnunciatorNumT num,
                              SaHpiEntryIdT entry,
                              SaHpiSeverityT severity);

SaErrorT oa_soap_add_announce(void *oh_handler,
                              SaHpiResourceIdT resource_id,
                              SaHpiAnnunciatorNumT num,
                              SaHpiAnnouncementT *announcement);
#endif
