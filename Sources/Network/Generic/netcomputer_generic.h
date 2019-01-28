/*
	$Id: netcomputer_generic.h,v 1.4 2001/05/09 18:54:03 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef header_netcomputer_generic
#define header_netcomputer_generic

class CL_NetSession_Generic;

class CL_NetComputer_Generic
{
// Construction:
public:
	CL_NetComputer_Generic(CL_NetSession_Generic *session);
	virtual ~CL_NetComputer_Generic();

// Attributes:
public:
	virtual unsigned long get_address() const=0;
	virtual unsigned short get_port() const=0;
	CL_NetSession_Generic *get_session() const;

// Operations:
public:
	void add_ref();
	void release_ref();
	virtual void disconnect()=0;

// Implementation:
private:
	CL_NetSession_Generic *session;
	int ref_count;
};

#endif
