/*
	$Id: datafile_writer.cpp,v 1.11 2002/04/06 19:50:45 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#include "Core/precomp.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#ifdef __MACOS__
#include <Types.h>
#include <io.h>
#else
#include <sys/types.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>

#ifdef WIN32
	#include <io.h>
#else
	#include <unistd.h>
#endif

#include "datafile_writer.h"

#ifndef WIN32
	#define OPEN_FLAGS O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
#else
	#define OPEN_FLAGS _O_RDWR | O_CREAT | O_TRUNC | O_BINARY, _S_IREAD | _S_IWRITE
#endif

/*********************************
 OutputSourceProvider_Datafile
*********************************/

OutputSourceProvider_Datafile::OutputSourceProvider_Datafile(const std::string &_filename)
{
	filename = _filename;
	datafile_fd = open(filename, OPEN_FLAGS);
	compression = true;

	int index_pos = -1;
	write(datafile_fd, datafile_id, strlen(datafile_id));
	write(datafile_fd, &index_pos, sizeof(int));
}

OutputSourceProvider_Datafile::~OutputSourceProvider_Datafile()
{
	int index_pos = lseek(datafile_fd, 0, SEEK_CUR);
	int num_indexes = indexes.size();

	write(datafile_fd, &num_indexes, sizeof(int));

	for (
		std::list<DatafileIndex*>::iterator it = indexes.begin();
		it != indexes.end();
		it++)
	{
		short name_len = (*it)->name.get_length()+1;
		write(datafile_fd, &name_len, sizeof(short));
		write(datafile_fd, (*it)->name, name_len);
		write(datafile_fd, &(*it)->pos, sizeof(int));
		write(datafile_fd, &(*it)->size, sizeof(int));
		delete *it;
	}

	lseek(datafile_fd, strlen(datafile_id), SEEK_SET);
	write(datafile_fd, &index_pos, sizeof(int));

	close(datafile_fd);
}

void OutputSourceProvider_Datafile::set_compression(bool enable)
{
	compression = enable;
}

int OutputSourceProvider_Datafile::create_index(const std::string &index)
{
	indexes.push_back(
		new DatafileIndex(
			index,
			lseek(datafile_fd, 0, SEEK_CUR)));
#ifdef __MACOS__
	return datafile_fd;
#else
	return dup(datafile_fd);
#endif
}

void OutputSourceProvider_Datafile::close_index(int size)
{
//	indexes.get_last()->size = lseek(datafile_fd, 0, SEEK_CUR) - indexes.get_last()->pos;

	indexes.back()->size = size;
/*
	std::cout << "Index written:" << std::endl;
	std::cout << "  name: " << indexes.get_last()->name << std::endl;
	std::cout << "  pos: " << indexes.get_last()->pos << std::endl;
	std::cout << "  size: " << indexes.get_last()->size << std::endl;
	std::cout << std::endl;
*/
}

CL_OutputSource *OutputSourceProvider_Datafile::open_source(const std::string &handle)
{
	return new OutputSource_Datafile(handle, this);
}

CL_OutputSourceProvider *OutputSourceProvider_Datafile::clone()
{
	return NULL;
}

/*************************
 OutputSource_Datafile
*************************/

OutputSource_Datafile::OutputSource_Datafile(
	const std::string &handle,
	OutputSourceProvider_Datafile *_provider)
{
	provider = _provider;
	output_fd = provider->create_index(handle);
	char compression = provider->get_compression() ? 1 : 0;
	::write(output_fd, &compression,1);
	if (provider->get_compression()) output_zfd = gzdopen(output_fd, "w+b");
	pos = 1;
}

OutputSource_Datafile::~OutputSource_Datafile()
{
	if (provider->get_compression()) gzclose(output_zfd);
	else ::close(output_fd);

	provider->close_index(pos);
}

int OutputSource_Datafile::write(const void *data, int size)
{
	if (provider->get_compression())
		gzwrite(output_zfd, (void *) data, size);
	else
		::write(output_fd, (void *) data, size);

	pos += size;
	return size;
}

void OutputSource_Datafile::open()
{
}

void OutputSource_Datafile::close()
{
}

CL_OutputSource *OutputSource_Datafile::clone()
{
	return NULL;
}

int OutputSource_Datafile::tell() const
{
	return pos;
}

int OutputSource_Datafile::size() const
{
	return pos;
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef WIN32
	#include <io.h>
#else
	#include <unistd.h>
#endif
							
#include "datafilewriter.h"

#ifndef WIN32
#define OPEN_FLAGS	 O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
#else
#define OPEN_FLAGS	 _O_RDWR | O_CREAT | O_TRUNC | O_BINARY, _S_IREAD | _S_IWRITE
#endif

CL_Datafile_Write::CL_Datafile_Write(char *_filename)
{
	filename=_filename;

	cur_index_size=0;

	char open_buffer[256];
	// Open files here
	datafile=open(filename, OPEN_FLAGS);
	
	strcpy(open_buffer, filename);
	strcat(open_buffer, "_Index");
	index=open(open_buffer, OPEN_FLAGS);

	strcpy(open_buffer, filename);
	strcat(open_buffer, "_Data");
	data=open(open_buffer, OPEN_FLAGS);

	num_indexes=0;
	index_open=false;

	init_datafile();
}

CL_Datafile_Write::~CL_Datafile_Write()
{
	finish_datafile();

	close(datafile);
	close(index);
	close(data);

	// Delete temp files...
	char open_buffer[256];
	strcpy(open_buffer, filename);
	strcat(open_buffer, "_Index");
	remove(open_buffer);

	strcpy(open_buffer, filename);
	strcat(open_buffer, "_Data");
	remove(open_buffer);
}

void CL_Datafile_Write::init_datafile()
{
	// Init index file

	write(index, datafile_id, strlen(datafile_id)+1);
}

void CL_Datafile_Write::finish_datafile()
{
	if (index_open==true)
	{
		gzclose(data_indexed);
		index_open=false;

	write(index, &cur_index_size, sizeof(int));
	}

	int end_signal=-1;
	write(index, &end_signal, sizeof(int));

	// Copy index and data together. Also, adjust the index dest values (so help me god)

	long index_size=lseek(index,0,SEEK_CUR);

	long data_size=lseek(data,0,SEEK_CUR);

	lseek(index, 0, SEEK_SET);
	lseek(data, 0, SEEK_SET);

	copy_file(datafile, index, index_size);
	copy_file(datafile, data, data_size);

	lseek(datafile, strlen(datafile_id)+1, SEEK_SET); // Skip smart-ass comment

	for (int index=0; index<num_indexes; index++)
	{
		int size_name;
		read(datafile, &size_name, sizeof(int));
		lseek(datafile, size_name, SEEK_CUR); // skip name

		long index_pos;
		read(datafile, &index_pos, sizeof(long));
		index_pos+=index_size;

		lseek(datafile, -4, SEEK_CUR); // Seek back to index_pos position
		write(datafile, &index_pos, sizeof(long));

		lseek(datafile, 4, SEEK_CUR); // Skip size information
	}
}

void CL_Datafile_Write::copy_file(int dest, int source, long len)
{
	char buffer[16*1024];

	long copy_left=len;
	long copy_done=0;
	long copy_now=16*1024;

	while (copy_left>0)
	{
		if (copy_left<copy_now) copy_now=copy_left;
		copy_done+=copy_now;
		copy_left-=copy_now;

		read(source, buffer, copy_now);
		write(dest, buffer, copy_now);
	}
}

void CL_Datafile_Write::create_index(char *index_name)
{
	if (index_open==true)
	{
		gzclose(data_indexed);
		index_open=false;

	write(index, &cur_index_size, sizeof(int));
	}

	int size_name=strlen(index_name)+1;
	long data_pos=lseek(data,0,SEEK_CUR);
	write(index, &size_name, sizeof(int));
	write(index, index_name, size_name);
	write(index, &data_pos, sizeof(long));

	index_open=true;
	data_indexed=gzdopen(dup(data), "w+b");

	num_indexes++;

	cur_index_size=0;
}

void CL_Datafile_Write::write_data(void *data, int data_size)
{
	if (index_open==false) return;

	gzwrite(data_indexed, data, data_size);
	cur_index_size += data_size;
}

gzFile CL_Datafile_Write::get_file_pointer()
{
	return data_indexed;
}
*/
