#include "Cylinder.h"
#include "GraphicsEngine.h"
#include "d3dx12.h"
#include "MathEngine.h"
#include <math.h>

struct vert
{
	float x;
	float y;
	float z;
	float s;
	float t;
	float nx;
	float ny;
	float nz;
};

Cylinder::Cylinder( Model_ID id )
	: Primitive( id )
{
	initModel();
}
		
Cylinder::~Cylinder()
{
}

void Cylinder::initModel() 
{
	vert verts[42]; 

	verts[0].x = 0.0f;
	verts[0].y = 1.0f;
	verts[0].z = 0.0f;
	verts[0].s = 0.0f;
	verts[0].t = 0.0f;
	verts[0].nx = 0.0f;
	verts[0].ny = 0.0f;
	verts[0].nz = 0.0f;

	for( int i = 1; i < 21; ++i )
	{
		verts[i].x = cosf( ( (float)( i - 1 ) / 20.0f ) * MATH_2PI );
		verts[i].y = 1.0f;
		verts[i].z = sinf( ( (float)( i - 1 ) / 20.0f ) * MATH_2PI );
		verts[i].s = 0.0f;
		verts[i].t = 0.0f;
		verts[i].nx = 0.0f;
		verts[i].ny = 0.0f;
		verts[i].nz = 0.0f;
	}

	verts[21].x = 0.0f;
	verts[21].y = -1.0f;
	verts[21].z = 0.0f;
	verts[21].s = 0.0f;
	verts[21].t = 0.0f;
	verts[21].nx = 0.0f;
	verts[21].ny = 0.0f;
	verts[21].nz = 0.0f;

	for( int i = 22; i < 42; ++i )
	{
		verts[i].x = cosf( ( (float)( i - 22 ) / 20.0f ) * MATH_2PI );
		verts[i].y = -1.0f;
		verts[i].z = sinf( ( (float)( i - 22 ) / 20.0f ) * MATH_2PI );
		verts[i].s = 0.0f;
		verts[i].t = 0.0f;
		verts[i].nx = 0.0f;
		verts[i].ny = 0.0f;
		verts[i].nz = 0.0f;
	}

	unsigned int indices[ 3 * 80 ];

	for( int i = 0; i < 20; ++i )
	{
		indices[ i * 3 ] = 0;
		indices[ i * 3 + 1 ] = i + 2;
		indices[ i * 3 + 2 ] = i + 1;
	}
	indices[58] = 1;

	for( int i = 20; i < 40; ++i )
	{
		indices[ i * 3 ] = 21;
		indices[ i * 3 + 1 ] = i + 2;
		indices[ i * 3 + 2 ] = i + 3;
	}
	indices[119] = 22;

	for( int i = 40; i < 60; ++i )
	{
		indices[ i * 3 ] = i - 39;
		indices[ i * 3 + 1 ] = i - 38;
		indices[ i * 3 + 2 ] = i - 17;
	}
	indices[178] = 1;
	indices[179] = 22;

	for( int i = 60; i < 80; ++i )
	{
		indices[ i * 3 ] = i - 37;
		indices[ i * 3 + 1 ] = i - 38;
		indices[ i * 3 + 2 ] = i - 59;
	}
	indices[237] = 22;

	ComPtr<ID3D12Device> device = GraphicsEngine::getDevice();
	ComPtr<ID3D12GraphicsCommandList> commandList = GraphicsEngine::getCommandList();

	D3D12_HEAP_PROPERTIES heapProps;
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

	{
		D3D12_RESOURCE_DESC resourceDesc;
		resourceDesc.Width = 42 * sizeof(vert);

		HRESULT res = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&vertexBuffer));
		assert(res == S_OK);

		D3D12_HEAP_PROPERTIES heapPropsUpLoad;
		heapPropsUpLoad.Type = D3D12_HEAP_TYPE_UPLOAD;

		ComPtr<ID3D12Resource> vertexBufferUploadHeap;
		res = device->CreateCommittedResource(&heapPropsUpLoad, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBufferUploadHeap));
		assert(res == S_OK);

		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = verts;
		vertexData.RowPitch = 42 * sizeof(vert);
		vertexData.SlicePitch = vertexData.RowPitch;

		D3D12_RESOURCE_BARRIER transition;
		transition.Transition.pResource = vertexBuffer.Get();
		transition.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		transition.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;

		UpdateSubresources<1>(commandList.Get(), vertexBuffer.Get(), vertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);
		commandList->ResourceBarrier(1, &transition);

		vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = sizeof(vert);
		vertexBufferView.SizeInBytes = 42 * sizeof(vert);
	}
	//Index Buffer

	{
		D3D12_RESOURCE_DESC resourceDesc;
		resourceDesc.Width = 80 * 3 * sizeof(unsigned int);

		HRESULT res = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&indexBuffer));
		assert(res == S_OK);

		D3D12_HEAP_PROPERTIES heapPropsUpLoad;
		heapPropsUpLoad.Type = D3D12_HEAP_TYPE_UPLOAD;

		ComPtr<ID3D12Resource> indexBufferUploadHeap;
		res = device->CreateCommittedResource(&heapPropsUpLoad, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBufferUploadHeap));
		assert(res == S_OK);

		D3D12_SUBRESOURCE_DATA indexData = {};
		indexData.pData = verts;
		indexData.RowPitch = 80 * 3 * sizeof(unsigned int);
		indexData.SlicePitch = indexData.RowPitch;

		D3D12_RESOURCE_BARRIER transition;
		transition.Transition.pResource = indexBuffer.Get();
		transition.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		transition.Transition.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER;

		UpdateSubresources<1>(commandList.Get(), indexBuffer.Get(), indexBufferUploadHeap.Get(), 0, 0, 1, &indexData);
		commandList->ResourceBarrier(1, &transition);

		indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		indexBufferView.SizeInBytes = 80 * 3 * sizeof(unsigned int);
	}

	numVerts = 42;
	numIndices = 80 * 3;
}