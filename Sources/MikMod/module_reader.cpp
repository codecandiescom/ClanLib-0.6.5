/*
	$Id: module_reader.cpp,v 1.2 2001/03/13 21:43:02 sphair Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	File purpose:
		Module reader for MikMod

*/

#include "module_reader.h"
#include "../API/Core/IOData/inputsource.h"

BOOL clanlib_reader_feof(MREADER* reader)
// eof function, returns TRUE if we are at the end of the file.
// Since CL_InputSource does not have a built-in eof function,
// we compare the current position to the size of the file.
{
	return (((CL_InputSource *) (((MCLANLIBREADER *) reader)->input))->tell()
		>= (((CL_InputSource *) (((MCLANLIBREADER *) reader)->input))->size()));
}

BOOL clanlib_reader_read(MREADER* reader,void* ptr,size_t size)
// read function, puts the data in a buffer.
// Even if the return type is BOOL, the function returns the number
// of bytes read. MikMod does the same internally.
{
	return (((CL_InputSource *) (((MCLANLIBREADER *) reader)->input))->read(ptr,size));
}

int clanlib_reader_get(MREADER* reader)
// get function, returns a single char from the file
{
	return (((CL_InputSource *) (((MCLANLIBREADER *) reader)->input))->read_char8());
}

BOOL clanlib_reader_seek(MREADER* reader,long offset,int whence)
// seek function, goes to a given position. It's important to note that
// this function is truely necessary for MikMod, and it has to be able to
// go back, ie rewind the file. ClanLib's implementation of this "go back"
// feature is really slow because files are compressed. This is a 
// compromise.
{
	CL_InputSource::SeekEnum cl_whence;

	switch (whence)
	{
	case SEEK_SET:
		cl_whence=CL_InputSource::seek_set;
		break;
	case SEEK_CUR:
		cl_whence=CL_InputSource::seek_cur;
		break;
	case SEEK_END:
		cl_whence=CL_InputSource::seek_end;
		break;
	default:
		cl_whence=CL_InputSource::seek_set;
	}

	((CL_InputSource *) (((MCLANLIBREADER *) reader)->input))->seek(offset,cl_whence);

	return 0;
}

long clanlib_reader_tell(MREADER* reader)
// tell function, returns the current position in the file.
{
	return (((CL_InputSource *) (((MCLANLIBREADER *) reader)->input))->tell());
}

MREADER *new_clanlib_reader(void *input)
// creates a reader. We need this for MCLANLIBREADER is a plain struct
// and not a class, so we have to construct it "manually".
{
	MCLANLIBREADER* reader=new MCLANLIBREADER;
	if (reader)
	{
		reader->core.Eof =&clanlib_reader_feof;
		reader->core.Read=&clanlib_reader_read;
		reader->core.Get =&clanlib_reader_get;
		reader->core.Seek=&clanlib_reader_seek;
		reader->core.Tell=&clanlib_reader_tell;
		reader->input=input;
	}
	return (MREADER*)reader;
}

void delete_clanlib_reader (MREADER* reader)
// deletes the reader. Needed since MCLANLIBREADER is a struct and not
// a C++ class.
{
	if (reader!=NULL) delete(reader);
}

