#include "Model.h"
#include "MathEngine.h"
#include "GraphicsEngine.h"

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

Model::Model( Model_ID inID )
	: id( inID ), vertexBuffer( 0 ), indexBuffer( 0 ), stride( sizeof(vert) ), offset( 0 )
{
}

Model::~Model()
{
}

ComPtr<ID3D12Resource> Model::getVertexBuffer()
{
	return vertexBuffer;
}

ComPtr<ID3D12Resource> Model::getIndexBuffer()
{
	return indexBuffer;
}

D3D12_VERTEX_BUFFER_VIEW * Model::getVertexBufferView()
{
	return &vertexBufferView;
}

D3D12_INDEX_BUFFER_VIEW * Model::getIndexBufferView()
{
	return &indexBufferView;
}

unsigned int* Model::getStride()
{
	return &stride;
}

unsigned int* Model::getOffset()
{
	return &offset;
}

unsigned int Model::getNumIndices()
{
	return numIndices;
}

unsigned int Model:: getNumVertices()
{
	return numVerts;
}