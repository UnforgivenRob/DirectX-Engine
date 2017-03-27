#include "Sphere.h"
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

Sphere::Sphere( Model_ID id )
	: Primitive( id )
{
	initModel();
}
		
Sphere::~Sphere()
{
}

void Sphere::initModel() 
{
	vert verts[2 + 20 * 10]; 

	verts[0].x = 0.0f;
	verts[0].y = 1.0f;
	verts[0].z = 0.0f;
	verts[0].s = 0.0f;
	verts[0].t = 0.0f;
	verts[0].nx = 0.0f;
	verts[0].ny = 0.0f;
	verts[0].nz = 0.0f;

	for( int j = 1; j < 11; ++j )
	{
		for( int i = 1; i < 21; ++i )
		{
			verts[(j - 1) * 20 + i ].x = cosf( ( (float)( i - 1 ) / 20.0f ) * MATH_2PI ) * sinf( ( (float)j / 11.0f ) * MATH_PI );
			verts[(j - 1) * 20 + i ].y = cosf( ( (float)( j )  / 11.0f ) * MATH_PI );
			verts[(j - 1) * 20 + i ].z = sinf( ( (float)( i - 1 ) / 20.0f ) * MATH_2PI ) * sinf( ( (float)j / 11.0f ) * MATH_PI );
			verts[(j - 1) * 20 + i ].s = 0.0f;
			verts[(j - 1) * 20 + i ].t = 0.0f;
			verts[(j - 1) * 20 + i ].nx = 0.0f;
			verts[(j - 1) * 20 + i ].ny = 0.0f;
			verts[(j - 1) * 20 + i ].nz = 0.0f;
		}
	}

	verts[201].x = 0.0f;
	verts[201].y = -1.0f;
	verts[201].z = 0.0f;
	verts[201].s = 0.0f;
	verts[201].t = 0.0f;
	verts[201].nx = 0.0f;
	verts[201].ny = 0.0f;
	verts[201].nz = 0.0f;

	unsigned int indices[ 3 * (20 + 20 + 10 * 40 ) ];

	for( int i = 0; i < 20; ++i )
	{
		indices[ i * 3 ] = 0;
		indices[ i * 3 + 1 ] = i + 2;
		indices[ i * 3 + 2 ] = i + 1;
	}

	indices[58] = 1;
	for( int j = 0; j < 9; ++j )
	{
		int curIndex = j * 120 + 60;
		int curVert = j * 20;
		for( int i = 0; i < 20; ++i )
		{
			indices[ curIndex + ( i * 6 ) ]		=  curVert + i + 2;
			indices[ curIndex + ( i * 6 ) + 1 ] = curVert + 20 + i + 1;
			indices[ curIndex + ( i * 6 ) + 2 ] = curVert + i + 1;

			indices[ curIndex + ( i * 6 ) + 3 ] = curVert + i + 2;
			indices[ curIndex + ( i * 6 ) + 4 ] = curVert + 20 + i + 2;
			indices[ curIndex + ( i * 6 ) + 5 ] = curVert + 20 + i + 1;
		}
		indices[ curIndex + 120 - 2 ] = curVert + 21;
		indices[ curIndex + 120 - 3 ] = curVert + 1;
		indices[ curIndex + 120 - 6 ] = curVert + 1;
	}

	int vertIndex = 180;
	for( int i = 20 + ( 10 * 40 ); i < 20 + 20 + ( 10 * 40 ); ++i )
	{
		indices[ i * 3 ] = vertIndex + 2;
		indices[ i * 3 + 1 ] = 201; 
		indices[ i * 3 + 2 ] = vertIndex + 1;

		vertIndex++;
	}

	indices[3 * (20 + 20 + 10* 40) - 3 ] = 181;

	ComPtr<ID3D12Device> device = GraphicsEngine::getDevice();

	ComPtr<ID3D12GraphicsCommandList> commandList = GraphicsEngine::getCommandList();

	D3D12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	{
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer( (2 + (20 * 10)) * sizeof(vert));

		HRESULT res = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&vertexBuffer));
		assert(res == S_OK);

		D3D12_HEAP_PROPERTIES heapPropsUpLoad = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		ComPtr<ID3D12Resource> vertexBufferUploadHeap;
		res = device->CreateCommittedResource(&heapPropsUpLoad, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBufferUploadHeap));
		assert(res == S_OK);

		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = verts;
		vertexData.RowPitch = (2 + (20 * 10)) * sizeof(vert);
		vertexData.SlicePitch = vertexData.RowPitch;

		D3D12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		UpdateSubresources<1>(commandList.Get(), vertexBuffer.Get(), vertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);
		commandList->ResourceBarrier(1, &transition);

		vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = sizeof(vert);
		vertexBufferView.SizeInBytes = (2 + (20 * 10)) * sizeof(vert);
	}
	//Index Buffer

	{
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((3 * (20 + 20 + (10 * 40))) * sizeof(unsigned int));

		HRESULT res = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&indexBuffer));
		assert(res == S_OK);

		D3D12_HEAP_PROPERTIES heapPropsUpLoad = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		ComPtr<ID3D12Resource> indexBufferUploadHeap;
		res = device->CreateCommittedResource(&heapPropsUpLoad, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBufferUploadHeap));
		assert(res == S_OK);

		D3D12_SUBRESOURCE_DATA indexData = {};
		indexData.pData = verts;
		indexData.RowPitch = (3 * (20 + 20 + (10 * 40))) * sizeof(unsigned int);
		indexData.SlicePitch = indexData.RowPitch;

		D3D12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);

		UpdateSubresources<1>(commandList.Get(), indexBuffer.Get(), indexBufferUploadHeap.Get(), 0, 0, 1, &indexData);
		commandList->ResourceBarrier(1, &transition);

		indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		indexBufferView.SizeInBytes = (3 * (20 + 20 + (10 * 40))) * sizeof(unsigned int);
	}
	
	numVerts = 2 + ( 20 * 10 );
	numIndices = 3 * ( 20 + 20 + ( 10 * 40 ) );

}