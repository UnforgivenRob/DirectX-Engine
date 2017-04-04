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

	{
		CD3DX12_HEAP_PROPERTIES props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(1024 * 64);
		HRESULT res = GraphicsEngine::getDevice()->CreateCommittedResource(
			&props,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constantBuffer));

		// Describe and create a constant buffer view.
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = constantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = (sizeof(BaseBuffer) + 255) & ~255;	// CB size is required to be 256-byte aligned.
		GraphicsEngine::getDevice()->CreateConstantBufferView(&cbvDesc, GraphicsEngine::getCBVHeap()->GetCPUDescriptorHandleForHeapStart());

		// Map and initialize the constant buffer. We don't unmap this until the
		// app closes. Keeping things mapped for the lifetime of the resource is okay.
		CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
		res = constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&cbvStart));
		assert(res == S_OK);
		memcpy(cbvStart, &buffer, sizeof(buffer));
	}

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

UINT8* Shader::getCBStart()
{
	return cbvStart;
}

BaseBuffer * Shader::getBuffer()
{
	return &buffer;
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
