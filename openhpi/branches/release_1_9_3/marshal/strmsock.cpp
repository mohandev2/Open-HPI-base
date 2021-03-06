/*      -*- linux-c -*-
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
 *      W. David Ashley <dashley@us.ibm.com>
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <errno.h>
#include <glib.h>
#include <byteswap.h>

#include "strmsock.h"
#include "marshal.h"


// Local definitions
#ifndef INADDR_NONE
#define INADDR_NONE  0xffffffff
#endif


/*--------------------------------------------------------------------*/
/* Stream Sockets base class methods                   */
/*--------------------------------------------------------------------*/

void strmsock::Close(void)
{
	if (fOpen) {
		close(s);
		fOpen = FALSE;
	}
	errcode = 0;
	return;
}


int strmsock::GetErrcode(void)
{
	return(errcode);
}


void strmsock::SetDomain(
        int lNewDomain)                 // the new domain
{
	domain = lNewDomain;
	errcode = 0;
	return;
}


void strmsock::SetProtocol(
        int lNewProtocol)               // the new protocol
{
	protocol = lNewProtocol;
	errcode = 0;
	return;
}


void strmsock::SetType(
        int lNewType)                   // the new type
{
	type = lNewType;
	errcode = 0;
	return;
}

bool strmsock::WriteMsg(const void *request, const void *reply)
{
	char data[dMaxMessageLength];
	int l = (sizeof(cMessageHeader) * 2) + reqheader.m_len + repheader.m_len;
        int x = 0;

	if (!fOpen) {
		return true;
	}

	if (l > dMaxMessageLength) {
		return true;
	}

	memcpy(&data[x], &reqheader, sizeof(cMessageHeader));
        x += sizeof(cMessageHeader);
	memcpy(&data[x], &request, reqheader.m_len);
        x += reqheader.m_len;
	memcpy(&data[x], &repheader, sizeof(cMessageHeader));
        x += sizeof(cMessageHeader);
	memcpy(&data[x], &reply, repheader.m_len);

	int rv = write(s, data, l);

	if ( rv != l ) {
		return true;
	}

	return false;
}

void *strmsock::ReadMsg(void)
{
	char *data = (char *)malloc(dMaxMessageLength);
        int x = 0;

	if (!fOpen) {
		return NULL;
	}

	unsigned int len = read( s, data, dMaxMessageLength);

	if (len < 0) {
		return NULL;
	} else if (len == 0) {	//connection has been aborted by the peer
		Close();
		return NULL;
	} else if (len < sizeof(cMessageHeader) * 2) {
		return NULL;
	}

	memcpy(&reqheader, &data[x], sizeof(cMessageHeader));
        x += sizeof(cMessageHeader);
        // swap id and len if nessesary in the request header
	if ((reqheader.m_flags & dMhEndianBit) != MarshalByteOrder()) {
		reqheader.m_id  = bswap_32(reqheader.m_id);
		reqheader.m_len = bswap_32(reqheader.m_len);
	}
        x += reqheader.m_len;
	memcpy(&repheader, &data[x], sizeof(cMessageHeader));
        x += sizeof(cMessageHeader);
        // swap id and len if nessesary in the reply header
	if ((repheader.m_flags & dMhEndianBit) != MarshalByteOrder()) {
		repheader.m_id  = bswap_32(repheader.m_id);
		repheader.m_len = bswap_32(repheader.m_len);
	}

	if ( (repheader.m_flags >> 4) != dMhVersion ) {
		return NULL;
	}

	return (data);
}

void strmsock::ReqMessageHeaderInit(tMessageType mtype, unsigned char flags,
		          	    unsigned int id, unsigned int len )
{
	reqheader.m_type    = mtype;
	reqheader.m_flags   = flags;

        // set version
	reqheader.m_flags &= 0x0f;
	reqheader.m_flags |= dMhVersion << 4;

        // set endian
	reqheader.m_flags &= ~dMhEndianBit;
	reqheader.m_flags |= MarshalByteOrder();

	reqheader.m_id = id;
	reqheader.m_len = len;
}

void strmsock::RepMessageHeaderInit(tMessageType mtype, unsigned char flags,
		          	    unsigned int id, unsigned int len )
{
	repheader.m_type    = mtype;
	repheader.m_flags   = flags;

        // set version
	repheader.m_flags &= 0x0f;
	repheader.m_flags |= dMhVersion << 4;

        // set endian
	repheader.m_flags &= ~dMhEndianBit;
	repheader.m_flags |= MarshalByteOrder();

	repheader.m_id = id;
	repheader.m_len = len;
}

/*--------------------------------------------------------------------*/
/* Stream Sockets client class methods                                */
/*--------------------------------------------------------------------*/


cstrmsock::cstrmsock()
{
	fOpen = FALSE;
	ulBufSize = 4096;
	domain = AF_INET;
	type = SOCK_STREAM;
	protocol = 0;
	s = -1;
	next = NULL;
}


cstrmsock::~cstrmsock()
{
	if (fOpen)
		Close();
}


bool cstrmsock::Open(
		const char * pszHost,		// the remote host
		const int lPort)		// the remote port
{
	struct sockaddr_in  addr;		// address structure
	struct hostent     *phe;		 // pointer to a host entry

        // get a socket
	s = socket(domain, type, protocol);
	if (s == -1) {
		errcode = errno;
		return(TRUE);
	}
        // convert the host entry/name to an address
	phe = gethostbyname(pszHost);
	if (phe)
		bcopy(phe -> h_addr, (char *) &addr.sin_addr, phe -> h_length);
	else
		addr.sin_addr.s_addr = inet_addr(pszHost);
	if (addr.sin_addr.s_addr == INADDR_NONE) {
		errcode = 67;  // bad network name
		close(s);
		return(TRUE);
	}
        // connect to the remote host
	addr.sin_family = domain;
	addr.sin_port = htons(lPort);
	errcode = connect(s, (struct sockaddr *) &addr, sizeof(addr));
	if (errcode == -1) {
		errcode = errno;
		close(s);
		return(TRUE);
	}

	errcode = 0;
	fOpen = TRUE;
	return(FALSE);
}


/*--------------------------------------------------------------------*/
/* Stream Sockets server class methods                 */
/*--------------------------------------------------------------------*/


sstrmsock::sstrmsock()
{
	fOpen = FALSE;
	fOpenS = FALSE;
	ulBufSize = 4096;
	domain = AF_INET;
	type = SOCK_STREAM;
	protocol = 0;
	backlog = 20;
	s = -1;
	ss = -1;
}


sstrmsock::sstrmsock(const sstrmsock &copy)
{
	if (this == &copy) {
		return;
	}
	fOpen = copy.fOpen;
	fOpenS = copy.fOpenS;
	ulBufSize = copy.ulBufSize;
	domain = copy.domain;
	type = copy.type;
	protocol = copy.protocol;
	backlog = copy.backlog;
	s = copy.s;
	ss = copy.ss;
	errcode = 0;
}


sstrmsock::~sstrmsock()
{
// Note: do NOT close the server socket by default! This will cause
// an error in a multi-threaded environment.
	if (fOpen)
		Close();
}


bool sstrmsock::Accept(void)
{
	socklen_t sz = sizeof(addr);

	if (!fOpenS) {
		return(TRUE);
	}
// accept the connection and obtain the connection socket
	sz = sizeof (struct sockaddr);
	s = accept(ss, (struct sockaddr *) &addr, &sz);
	if (s == -1) {
		errcode = errno;
		fOpen = FALSE;
		return(TRUE);
	}

	fOpen = TRUE;
	return(FALSE);
}

void sstrmsock::CloseSrv(void)
{
	if (fOpenS) {
		close(ss);
		fOpenS = FALSE;
	}
	errcode = 0;
	return;
}


bool sstrmsock::Create(
        int    Port)                    // the local port
{
	int    Rc;				// return code
	int    so_reuseaddr = TRUE;	// socket reuse flag

        // get a server socket
	ss = socket(domain, type, protocol);
	if (ss == -1) {
		errcode = errno;
		return(TRUE);
	}
        // set the socket option to reuse the address
	setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr,
							sizeof(so_reuseaddr));
        // bind the server socket to a port
	memset(&addr, sizeof (addr), 0);
	addr.sin_family = domain;
	addr.sin_port = htons(Port);
	addr.sin_addr.s_addr = INADDR_ANY;
	Rc = bind(ss, (struct sockaddr *) &addr, sizeof(addr));
	if (Rc == -1) {
		errcode = errno;
		return (TRUE);
	}
        // listen for a client at the port
	Rc = listen(ss, backlog);
	if (Rc == -1) {
		errcode = errno;
		return(TRUE);
	}

	errcode = 0;
	fOpenS = TRUE;
	return(FALSE);
}
