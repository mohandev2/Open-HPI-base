
/*
 * Copyright (c) 2003, Intel Corporation
 * All rights reserved.
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
 * Neither the name of Intel Corporation nor the names 
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
 */

#ifndef HPITESTAPP_H
#define HPITESTAPP_H

#define SA_ERR_HPI_ERROR_STR 		"SA_ERR_HPI_ERROR"
#define SA_ERR_HPI_UNSUPPORTED_API_STR	"SA_ERR_HPI_UNSUPPORTED_API"
#define SA_ERR_UNSUPPORTED_API_STR 	"SA_ERR_UNSUPPORTED_API"
#define SA_ERR_HPI_BUSY_STR		"SA_ERR_HPI_BUSY"
#define SA_ERR_HPI_INVALID_STR		"SA_ERR_HPI_INVALID"
#define  SA_ERR_HPI_INVALID_CMD_STR 	"SA_ERR_HPI_INVALID_CMD"
#define SA_ERR_HPI_TIMEOUT_STR 		"SA_ERR_HPI_TIMEOUT"
#define SA_ERR_HPI_OUT_OF_SPACE_STR 	"SA_ERR_HPI_OUT_OF_SPACE"
#define SA_ERR_HPI_DATA_TRUNCATED_STR 	"SA_ERR_HPI_DATA_TRUNCATED"
#define SA_ERR_HPI_DATA_LEN_INVALID_STR	"SA_ERR_HPI_DATA_LEN_INVALID"
#define SA_ERR_HPI_DATA_EX_LIMITS_STR	"SA_ERR_HPI_DATA_EX_LIMITS"
#define SA_ERR_HPI_INVALID_PARAMS_STR 	"SA_ERR_HPI_INVALID_PARAMS"
#define SA_ERR_HPI_INVALID_DATA_STR	"SA_ERR_HPI_INVALID_DATA"
#define SA_ERR_HPI_NOT_PRESENT_STR	"SA_ERR_HPI_NOT_PRESENT"
#define SA_ERR_HPI_INVALID_DATA_FIELD_STR 	"SA_ERR_HPI_INVALID_DATA_FIELD"
#define SA_ERR_HPI_INVALID_SENSOR_CMD_STR 	"SA_ERR_HPI_INVALID_SENSOR_CMD"
#define SA_ERR_HPI_NO_RESPONSE_STR	"SA_ERR_HPI_NO_RESPONSE"
#define SA_ERR_HPI_DUPLICATE_STR	"SA_ERR_HPI_DUPLICATE"
#define SA_ERR_HPI_INITIALIZING_STR	"SA_ERR_HPI_INITIALIZING"
#define SA_ERR_HPI_UNKNOWN_STR		"SA_ERR_HPI_UNKNOWN"
#define SA_ERR_HPI_INVALID_SESSION_STR	"SA_ERR_HPI_INVALID_SESSION"
#define SA_ERR_HPI_INVALID_RESOURCE_STR	"SA_ERR_HPI_INVALID_RESOURCE"
#define SA_ERR_HPI_INVALID_REQUEST_STR	"SA_ERR_HPI_INVALID_REQUEST"
#define SA_ERR_HPI_ENTITY_NOT_PRESENT_STR	"SA_ERR_HPI_ENTITY_NOT_PRESENT"
#define SA_ERR_HPI_UNINITIALIZED_STR	"SA_ERR_HPI_UNINITIALIZED"

int discover_resources(SaHpiDomainIdT domainId);
void display_resource_entry(SaHpiRptEntryT rptEntry);
const char * get_error_string(SaErrorT error);

#endif /* HPITESTAPP_H */
