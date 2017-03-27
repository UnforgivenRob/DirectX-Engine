#include <assert.h>
#include "GridModel.h"
#include "GraphicsEngine.h"
#include "d3dx12.h"

struct vert
{
	float X;
	float Y;
	float Z;
	float s;
	float t;
	float nx;
	float ny;
	float nz;
};

GridModel::GridModel(Model_ID id, int width, int length, int step)
	: Primitive( id ), width( width ), length( length ), step( step )
{
	initModel();
}

GridModel::~GridModel()
{
}

void GridModel::initModel()
{
	int numLong = length / step;
	int numWide = width / step;

	numVerts = 3 * ( numLong + numWide + 2 );
	numIndices = 3 * ( numLong + numWide + 2 );
	stride = 8 * sizeof(float);

	float* verts = new float[numVerts * stride];
	unsigned int* indices = new unsigned int[numIndices];
	float halfWidth = width /2.0f;
	float halfLength = length / 2.0f;

	int cnt = 0;
	for( int i = 0; i <= numLong; ++i )
	{ 
		verts[ cnt ] = 0.0f - halfWidth + (float)( i * step );
		verts[ cnt + 1 ] = 0.0f - halfLength;
		verts[ cnt + 2 ] = 0.0f;
		verts[ cnt + 3 ] = 0.0f;
		verts[ cnt + 4 ] = 0.0f;
		verts[ cnt + 5 ] = 0.0f;
		verts[ cnt + 6 ] = 0.0f;
		verts[ cnt + 7 ] = 0.0f;

		verts[ cnt + 8 ] = 0.0f - halfWidth + (float)( i * step );
		verts[ cnt + 9 ] = 00.0f + halfLength;
		verts[ cnt + 10] = 0.0f;
		verts[ cnt + 11] = 0.0f;
		verts[ cnt + 12] = 0.0f;
		verts[ cnt + 13] = 0.0f;
		verts[ cnt + 14] = 0.0f;
		verts[ cnt + 15] = 0.0f;

		verts[ cnt + 16] = 0.0f - halfWidth + (float)( i * step );
		verts[ cnt + 17] = 0.0f + halfLength;
		verts[ cnt + 18] = 0.0f;
		verts[ cnt + 19] = 0.0f;
		verts[ cnt + 20] = 0.0f;
		verts[ cnt + 21] = 0.0f;
		verts[ cnt + 22] = 0.0f;
		verts[ cnt + 23] = 0.0f;
		cnt += 8 * 3;
	}

	for( int i = 0; i <= numWide; ++i )
	{
		verts[ cnt ] = 0.0f - halfWidth;
		verts[ cnt + 1 ] = 0.0f - halfLength + (float)( i * step );
		verts[ cnt + 2 ] = 0.0f;
		verts[ cnt + 3 ] = 0.0f;
		verts[ cnt + 4 ] = 0.0f;
		verts[ cnt + 5 ] = 0.0f;
		verts[ cnt + 6 ] = 0.0f;
		verts[ cnt + 7 ] = 0.0f;

		verts[ cnt + 8 ] = 0.0f + halfWidth;
		verts[ cnt + 9 ] = 0.0f - halfLength + (float)( i * step );
		verts[ cnt + 10] = 0.0f;
		verts[ cnt + 11] = 0.0f;
		verts[ cnt + 12] = 0.0f;
		verts[ cnt + 13] = 0.0f;
		verts[ cnt + 14] = 0.0f;
		verts[ cnt + 15] = 0.0f;

		verts[ cnt + 16] = 0.0f + halfWidth;
		verts[ cnt + 17] = 0.0f - halfLength + (float)( i * step );
		verts[ cnt + 18] = 0.0f;
		verts[ cnt + 19] = 0.0f;
		verts[ cnt + 20] = 0.0f;
		verts[ cnt + 21] = 0.0f;
		verts[ cnt + 22] = 0.0f;
		verts[ cnt + 23] = 0.0f;
		cnt += 8 * 3;
	}

	for( unsigned int i = 0; i < numIndices; ++i )
	{
		indices[ i] = i;
	}

	ComPtr<ID3D12Device> device = GraphicsEngine::getDevice();
	ComPtr<ID3D12GraphicsCommandList> commandList = GraphicsEngine::getCommandList();

	D3D12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	{
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(numVerts * sizeof(vert));

		HRESULT res = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&vertexBuffer));
		assert(res == S_OK);

		D3D12_HEAP_PROPERTIES heapPropsUpLoad = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		ComPtr<ID3D12Resource> vertexBufferUploadHeap;
		res = device->CreateCommittedResource(&heapPropsUpLoad, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBufferUploadHeap));
		assert(res == S_OK);

		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = verts;
		vertexData.RowPitch = numVerts * sizeof(vert);
		vertexData.SlicePitch = vertexData.RowPitch;

		D3D12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		UpdateSubresources<1>(commandList.Get(), vertexBuffer.Get(), vertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);
		commandList->ResourceBarrier(1, &transition);

		vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = sizeof(vert);
		vertexBufferView.SizeInBytes = numVerts * sizeof(vert);
	}
	//Index Buffer

	{
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(numIndices * sizeof(unsigned int));

		HRESULT res = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&indexBuffer));
		assert(res == S_OK);

		D3D12_HEAP_PROPERTIES heapPropsUpLoad = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		ComPtr<ID3D12Resource> indexBufferUploadHeap;
		res = device->CreateCommittedResource(&heapPropsUpLoad, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBufferUploadHeap));
		assert(res == S_OK);

		D3D12_SUBRESOURCE_DATA indexData = {};
		indexData.pData = verts;
		indexData.RowPitch = numIndices * sizeof(unsigned int);
		indexData.SlicePitch = indexData.RowPitch;

		D3D12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);

		UpdateSubresources<1>(commandList.Get(), indexBuffer.Get(), indexBufferUploadHeap.Get(), 0, 0, 1, &indexData);
		commandList->ResourceBarrier(1, &transition);

		indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		indexBufferView.SizeInBytes = numIndices * sizeof(unsigned int);
	}
}