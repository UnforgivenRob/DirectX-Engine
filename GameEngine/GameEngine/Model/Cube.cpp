#include "Cube.h"
#include "GraphicsEngine.h"
#include "d3dx12.h"

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

Cube::Cube( Model_ID id )
	: Primitive( id )
{
	initModel();
}
		
Cube::~Cube()
{
}

void Cube::initModel() 
{
	vert verts[] = 
	{
		{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
	};

	unsigned int indices[] = {  0, 2, 1,
								0, 3, 2,
								0, 4, 3,
								3, 4, 7,
								0, 1, 4, 
								1, 5, 4,
								4, 5, 6,
								4, 6, 7,
								3, 6, 2,
								3, 7, 6,
								1, 2, 5,
								2, 6, 5
							};

	ComPtr<ID3D12Device> device = GraphicsEngine::getDevice();
	ComPtr<ID3D12GraphicsCommandList> commandList = GraphicsEngine::getCommandList();

	D3D12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	{
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(8 * sizeof(vert));

		HRESULT res = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&vertexBuffer));
		assert(res == S_OK);

		D3D12_HEAP_PROPERTIES heapPropsUpLoad = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		ComPtr<ID3D12Resource> vertexBufferUploadHeap;
		res = device->CreateCommittedResource(&heapPropsUpLoad, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBufferUploadHeap));
		assert(res == S_OK);

		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = verts;
		vertexData.RowPitch = 8 * sizeof(vert);
		vertexData.SlicePitch = vertexData.RowPitch;

		D3D12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition( vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		UpdateSubresources<1>(commandList.Get(), vertexBuffer.Get(), vertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);
		commandList->ResourceBarrier(1, &transition);

		vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = sizeof(vert);
		vertexBufferView.SizeInBytes = 8 * sizeof(vert);
	}
	//Index Buffer

	{
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer( 36 * sizeof(unsigned int) );

		HRESULT res = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&indexBuffer));
		assert(res == S_OK);

		D3D12_HEAP_PROPERTIES heapPropsUpLoad = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		ComPtr<ID3D12Resource> indexBufferUploadHeap;
		res = device->CreateCommittedResource(&heapPropsUpLoad, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBufferUploadHeap));
		assert(res == S_OK);

		D3D12_SUBRESOURCE_DATA indexData = {};
		indexData.pData = verts;
		indexData.RowPitch = 36 * sizeof(unsigned int);
		indexData.SlicePitch = indexData.RowPitch;
		
		D3D12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition( indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);

		UpdateSubresources<1>(commandList.Get(), indexBuffer.Get(), indexBufferUploadHeap.Get(), 0, 0, 1, &indexData);
		commandList->ResourceBarrier(1, &transition);

		indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		indexBufferView.SizeInBytes = 36 * sizeof(unsigned int);
	}
	numVerts = 8;
	numIndices = 36;
}