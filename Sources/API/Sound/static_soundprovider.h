/*
	$Id: static_soundprovider.h,v 1.5 2001/09/22 15:52:18 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------
*/

//! clanSound="Audio Mixing"
//! header=sound.h

#ifndef header_static_soundprovider
#define header_static_soundprovider

#include "soundformat.h"

//: Static Soundprovider class.
//- <p>The static soundprovider provides the data  for any static (non-streamed)
//- samples played using the ClanLib sound system.</p>
class CL_StaticSoundProvider
{
public:
//! Construction:
	//: Static Sound Provider Destructor
	virtual ~CL_StaticSoundProvider() { ; }

//! Operations:
	//: <p>Locks the soundprovider. Locking a provider means, that 
	//: the contents of the provider should be available following the 
	//: call to lock(), which means that lock loads any files from disk, 
	//: and performs any necessary initializations.</p>
	virtual void lock()=0;

	//: <p>Unlocks the soundprovider. Unlocking a provider means, that 
	//: the contents of the provider should be deinitialized, thus 
	//: removing any loaded sample data from memory.</p>
	virtual void unlock()=0;

//! Attributes:
	//: Returns the soundformat of the data stored by the provider. 
	//: <p>The data stored can be either 8-bit signed, 8-bit unsigned, 
	//: 16-bit signed and 16-bit unsigned.</p>
	//- Returns - The soundformat of the data stored.
	virtual SoundFormat get_format() const =0;

	//: Returns the total size in bytes of the data stored. 
	//- Returns - Size of data stored (in bytes).
	virtual int data_size() const =0;

	//: Returns a pointer to the data stored. 
	//- Returns - A pointer to the data stored.
	virtual void *get_data() const =0;

	//: Returns the frequency of the data stored. 
	//- Returns - Frequency of data stored.
	virtual int get_frequency() const =0;
};

#endif
