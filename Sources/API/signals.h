/*
	$Id: signals.h,v 1.10 2001/12/18 15:55:33 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//: <p>ClanLib signal system. Signals is an other word for callback functions. A
//: C++ signal library provides a typecast clean interface to allow for
//: callbacks to be called, without doing nasty pointer casts all the time. You
//: can use this library totally independent of the rest of clanlib, if you
//: want (as soon as you've learned about signals, you will love them - trust me).</p>
//! Global=Signal

#ifndef header_signals
#define header_signals

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "Signals/slot.h"
#include "Signals/signal_v0.h"
#include "Signals/signal_v1.h"
#include "Signals/signal_v2.h"
#include "Signals/signal_v3.h"
#include "Signals/signal_v4.h"
#include "Signals/signal_v5.h"
#include "Signals/slotbuffer_v0.h"
#include "Signals/slotbuffer_v1.h"
#include "Signals/slot_container.h"

#endif
