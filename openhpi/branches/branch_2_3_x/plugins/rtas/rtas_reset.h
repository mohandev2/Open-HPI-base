/*      -*- linux-c -*-
 *
 * (C) Copyright IBM Corp. 2005
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Author(s):
 *        Renier Morales <renierm@users.sf.net>
 *        Daniel de Araujo <ddearauj@us.ibm.com>
 */

#ifndef RTAS_RESET_H
#define RTAS_RESET_H

#include <glib.h>
#include <SaHpi.h>
 
SaErrorT rtas_get_reset_state(void *hnd,
                                 SaHpiResourceIdT id,
                                 SaHpiResetActionT *act);
				 
SaErrorT rtas_set_reset_state(void *hnd,
                                 SaHpiResourceIdT id,
                                 SaHpiResetActionT act); 

#endif
