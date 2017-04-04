#include "Cylinder.h"
#include "GraphicsEngine.h"
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

	ID3D11Device* device = GraphicsEngine::getDevice();

	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage           = D3D11_USAGE_IMMUTABLE;
	vertexDesc.ByteWidth       = 42 * sizeof( vert );
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
	indexDesc.ByteWidth       = 80 * 3 * sizeof( unsigned int );
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
	numVerts = 42;
	numIndices = 80 * 3;
}