#include "Shader.h"
#include <string>
#include "Game.h"
#include "FileSystem.h"
#include "ConstantBuffers.h"
#include "GraphicsEngine.h"
#include <d3dcompiler.h>
#include "d3dx12.h"
Shader::Shader( Shader_ID id, const char* inName, int cb_Size )
	: id( id ), Device( GraphicsEngine::getDevice() )
{
	cb_Size;
	char buff[200];

	
	strcpy_s( buff, "Debug\\" );
	strcat_s(buff, inName);
	strcat_s(buff, ".vs.cso");
	CompileVertexShader( buff, vs );

	strcpy_s( buff, "Debug\\" );
	strcat_s(buff, inName);
	strcat_s(buff, ".ps.cso");
	CompilePixelShader( buff, ps );

	VS = CD3DX12_SHADER_BYTECODE(vs.Get());
	PS = CD3DX12_SHADER_BYTECODE(ps.Get());

	layout = new D3D12_INPUT_ELEMENT_DESC[3];

	layout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	layout[1] = { "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	layout[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	layoutCnt = 3;
}

Shader::~Shader(void)
{
	delete[] layout;
}

D3D12_SHADER_BYTECODE& Shader::getVS()
{
	return VS;
}

D3D12_SHADER_BYTECODE& Shader::getPS()
{
	return PS;
}

D3D12_INPUT_ELEMENT_DESC * Shader::getLayout()
{
	return &layout[0];
}

unsigned int Shader::getLayoutCnt()
{
	return layoutCnt;
}

void Shader::CompileVertexShader( char* vsName, ComPtr<ID3DBlob>& Vs )
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

	err = File::seek(fh, FILE_SEEK_BEGIN, 0 );
	assert( err == FILE_SUCCESS );
	
	HRESULT res = D3DCreateBlob( sz, Vs.GetAddressOf() );
	assert( res == S_OK );

	err = File::read(fh, (char*)Vs->GetBufferPointer(), sz);
	assert(err == FILE_SUCCESS);

	err = File::close( fh );
	assert( err == FILE_SUCCESS );
}

void Shader::CompilePixelShader( char* psName, ComPtr<ID3DBlob>& Ps )
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

	err = File::seek(fh, FILE_SEEK_BEGIN, 0 );
	assert( err == FILE_SUCCESS );
	
	HRESULT res = D3DCreateBlob(sz, Ps.GetAddressOf());
	assert(res == S_OK);

	err = File::read(fh, (char*)Ps->GetBufferPointer(), sz);
	assert(err == FILE_SUCCESS);
}
