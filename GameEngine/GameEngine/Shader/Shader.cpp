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

	D3D11_INPUT_ELEMENT_DESC layout[] = 
    { 
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
    }; 
    UINT numElements = ARRAYSIZE( layout ); 
 
    // Create the input layout 
	res = Device->CreateInputLayout( layout, numElements, vsByteCode, sz, &vertexLayout ); 
	assert( res == S_OK );

    // Set the input layout 
    Context->IASetInputLayout( vertexLayout ); 

	err = File::close( fh );
	assert( err == FILE_SUCCESS );
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

ID3D11VertexShader* Shader::getVertexShader()
{
	return vs;
}

ID3D11PixelShader* Shader::getPixelShader()
{
	return ps;
}

ID3D11InputLayout* Shader::getVertexLayout()
{
	return vertexLayout;
}
