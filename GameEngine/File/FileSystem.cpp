
#include <assert.h>
#include "FileSystem.h"

#define UNUSED(x) x


FileError File::open( FileHandle &fh, const char * const fileName, FileMode mode )
{
	FileError ret;
	switch (mode)
	{
	case FILE_READ:
		fh = myCreateFile(fileName, GENERIC_READ, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL);
		break;
	case FILE_WRITE:
		fh = myCreateFile(fileName, GENERIC_WRITE, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL);
		break;
	case FILE_READ_WRITE:
		fh = myCreateFile(fileName, (GENERIC_READ | GENERIC_WRITE), CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL);
		break;
	}

	if(fh != INVALID_HANDLE_VALUE)
	{
		ret = FILE_SUCCESS;
	}
	else
	{
		ret = FILE_OPEN_FAIL;
	}
		
	return ret;
}

FileError File::close( const FileHandle fh )
{
	FileError ret = FILE_CLOSE_FAIL;
	assert(fh);
	BOOL res = CloseHandle(fh);
	if(res)
	{
		ret = FILE_SUCCESS;
	}
	return ret;
}

FileError File::write( FileHandle fh, const void * const buffer, const size_t inSize )
{
	//make sure params are valid
	assert(fh);

	FileError ret;
	LPDWORD numWritten = (LPDWORD)(new size_t());
	//Write to file
	BOOL res = myWriteFile(fh, (LPVOID)buffer, inSize, numWritten);
	//Test result
	if(res != 0 && *numWritten <= inSize)
	{
		ret = FILE_SUCCESS;
	}
	else
	{
		ret = FILE_WRITE_FAIL;
	}
	return ret;
}

FileError File::read( FileHandle fh,  void * const buffer, const size_t inSize )
{
	//make sure params are valid
	assert(fh);

	FileError ret;
	LPDWORD numRead = (LPDWORD)(new size_t());
	//Read File
	BOOL res = myReadFile(fh, buffer, inSize, numRead);
	//Test result
	if(res != 0 && *numRead <= inSize)
	{
		ret = FILE_SUCCESS;
	}
	else
	{
		ret = FILE_READ_FAIL;
	}
	return ret;
}

FileError File::seek( FileHandle fh, FileSeek seek, int offset )
{
	//make sure params are valid
	assert(fh);

	DWORD res = INVALID_SET_FILE_POINTER;
	//set file pointer to offset of correct position
	switch(seek)
	{
	case FILE_SEEK_BEGIN:
		res = mySetFilePointer(fh, offset, FILE_BEGIN);
		break;
	case FILE_SEEK_CURRENT:
		res = mySetFilePointer(fh, offset, FILE_CURRENT);
		break;
	case FILE_SEEK_END:
		res = mySetFilePointer(fh, offset, FILE_END);
		break;
	}
	FileError ret = FILE_SUCCESS;
	//make sure pointer return is valid
	if( res ==INVALID_SET_FILE_POINTER)
	{
		ret = FILE_SEEK_FAIL;
	}

	return ret;
}

FileError File::tell( FileHandle fh, int &offset )
{	
	offset = mySetFilePointer(fh, 0, FILE_CURRENT);
	
	FileError ret = FILE_SUCCESS;
	//make sure pointer return is valid
	if( offset == INVALID_SET_FILE_POINTER)
	{
		ret = FILE_TELL_FAIL;
	}

	return ret;

}

FileError File::flush( FileHandle fh )
{
	FileError ret = FILE_SUCCESS;
	BOOL res = FlushFileBuffers(fh);
	if (0 == res)
	{
		ret = FILE_FLUSH_FAIL;
	}
	return ret;
}

HANDLE File::myCreateFile(LPCSTR filename, DWORD access, DWORD creationdisposition, DWORD attributes )
{
	return CreateFile(filename, access, 0, 0, creationdisposition, attributes, NULL);
}

BOOL File::myReadFile(HANDLE hfile, LPVOID lpBuffer, DWORD numBytesToRead, LPDWORD bytesRead)
{
	return ReadFile(hfile, lpBuffer, numBytesToRead, bytesRead, NULL);
}

BOOL File::myWriteFile(HANDLE hfile, LPVOID lpBuffer, DWORD numBytesToWrite, LPDWORD bytesWritten)
{
	return WriteFile(hfile, lpBuffer, numBytesToWrite, bytesWritten, NULL);
}

DWORD File::mySetFilePointer(HANDLE hfile, int offset, DWORD whereInFile)
{
	return SetFilePointer(hfile, offset, NULL, whereInFile);
}