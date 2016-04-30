#include "Shader.h"
#include <string>
#include "Game.h"
#include "FileSystem.h"

Shader::Shader( Shader_ID id, char* inName, Game* inGame )
	: id( id ), Device( inGame->getDevice() ), Context( inGame->getContext() )
{
	char buff[200];
	
	strcpy_s( buff, "Debug\\" );
	strcat_s(buff, inName);
	strcat_s(buff, ".vs.cso");
	CompileVertexShader(buff); 

	strcpy_s( buff, "Debug\\" );
	strcat_s(buff, inName);
	strcat_s(buff, ".ps.cso");
	CompilePixelShader( buff );
}

Shader::~Shader(void)
{
	ReleaseCOM(vs);
	ReleaseCOM(ps);
}

void Shader::CompileVertexShader( char* vsName )
{
	FileHandle fh;
	FileError err;
	err = File::open( fh, vsName, FILE_READ );
	assert( err == FILE_SUCCESS );

	err = File::seek(fh, FILE_SEEK_END, 0 );
	assert( err == FILE_SUCCESS );

	int sz;
	err = File::tell( fh, sz );
	assert( err == FILE_SUCCESS );

	byte* vsByteCode = new byte[sz];

	err = File::seek(fh, FILE_SEEK_BEGIN, 0 );
	assert( err == FILE_SUCCESS );
	
	err = File::read( fh, vsByteCode, sz );
	assert( err == FILE_SUCCESS );

	HRESULT res = Device->CreateVertexShader( vsByteCode, sz, nullptr, &vs );
	assert( res == S_OK );
}

void Shader::CompilePixelShader( char* psName )
{
	FileHandle fh;
	FileError err;
	err = File::open( fh, psName, FILE_READ );
	assert( err == FILE_SUCCESS );

	err = File::seek(fh, FILE_SEEK_END, 0 );
	assert( err == FILE_SUCCESS );

	int sz;
	err = File::tell( fh, sz );
	assert( err == FILE_SUCCESS );

	byte* psByteCode = new byte[sz];

	err = File::seek(fh, FILE_SEEK_BEGIN, 0 );
	assert( err == FILE_SUCCESS );
	
	err = File::read( fh, psByteCode, sz );
	assert( err == FILE_SUCCESS );

	HRESULT res = Device->CreatePixelShader( psByteCode, sz, nullptr, &ps );
	assert( res == S_OK );
}
