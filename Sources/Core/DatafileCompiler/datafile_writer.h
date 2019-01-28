/*
	$Id: datafile_writer.h,v 1.7 2001/12/11 20:44:21 mbn Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#ifndef header_datafilewriter
#define header_datafilewriter

#define datafile_id "ClanSoft datafile version 4.0"

#include <list>

#ifdef __BEOS__
#include <gnu/zlib.h>
#else
#include <zlib.h> // Zlib compression library
#endif

#include "API/Core/System/clanstring.h"
#include "API/Core/IOData/outputsource.h"
#include "API/Core/IOData/outputsource_provider.h"

/*
class CL_Datafile_Write
{
protected:
	char *filename;

	int datafile;
	int index;
	int data;
	gzFile data_indexed;
	bool index_open;
	int num_indexes;

	void init_datafile();
	void finish_datafile();

	void copy_file(int dest, int source, long len);

	int cur_index_size;

public:
	CL_Datafile_Write(char *filename);
	virtual ~CL_Datafile_Write();

	void create_index(char *index);
	void write_data(void *data, int data_size);
	gzFile get_file_pointer();
};
*/

class DatafileIndex
{
public:
	CL_String name;
	int pos;
	int size;

	DatafileIndex(const std::string &_name, int _pos)
	{
		name = _name;
		pos = _pos;
		size = 0;
	}
};

class OutputSourceProvider_Datafile : public CL_OutputSourceProvider
{
protected:
	CL_String filename;
	int datafile_fd;

	std::list<DatafileIndex*> indexes;

	bool compression;

public:
	OutputSourceProvider_Datafile(const std::string &filename);
	virtual ~OutputSourceProvider_Datafile();

	void set_compression(bool enable = true);

	bool get_compression() const { return compression; }

	int create_index(const std::string &index);
	void close_index(int size);

	virtual CL_OutputSource *open_source(const std::string &handle);
	virtual CL_OutputSourceProvider *clone();
};

class OutputSource_Datafile : public CL_OutputSource
{
// Construction:
public:
	OutputSource_Datafile(const std::string &handle, OutputSourceProvider_Datafile *provider);
	virtual ~OutputSource_Datafile();

// Attributes:
public:
	virtual int tell() const;
	virtual int size() const;

// Operations:
public:
	virtual int write(const void *data, int size);

	virtual void open();

	virtual void close();

	virtual CL_OutputSource *clone();

// Implementation:
protected:
	OutputSourceProvider_Datafile *provider;
	int output_fd;
	gzFile output_zfd;
	int pos;
};

#endif
