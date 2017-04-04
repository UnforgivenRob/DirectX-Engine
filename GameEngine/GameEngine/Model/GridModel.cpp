#include <assert.h>
#include "GridModel.h"
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

	ID3D11Device* device = GraphicsEngine::getDevice();

	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage           = D3D11_USAGE_IMMUTABLE;
	vertexDesc.ByteWidth       = numVerts * sizeof( vert );
	vertexDesc.BindFlags       = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags  = 0;
	vertexDesc.MiscFlags       = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = verts;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT res = device->CreateBuffer( &vertexDesc, &InitData, &vertexBuffer );
	assert( res == S_OK );

	D3D11_BUFFER_DESC indexDesc;
	indexDesc.Usage           = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth       = numIndices * sizeof( unsigned int );
	indexDesc.BindFlags       = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags  = 0;
	indexDesc.MiscFlags       = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	res = device->CreateBuffer( &indexDesc, &indexData, &indexBuffer );
	assert( res == S_OK );

	GraphicsEngine::getContext()->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset ); 
	GraphicsEngine::getContext()->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R32_UINT, 0 ); 
}