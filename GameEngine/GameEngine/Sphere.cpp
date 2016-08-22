#include "Sphere.h"
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

	ID3D11Device* device = GraphicsEngine::getDevice();

	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage           = D3D11_USAGE_IMMUTABLE;
	vertexDesc.ByteWidth       = ( 2 + (20 * 10) ) * sizeof( vert );
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
	indexDesc.ByteWidth       =  ( 3 * ( 20 + 20 + ( 10 * 40 ) ) ) * sizeof( unsigned int );
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
	numVerts = 2 + ( 20 * 10 );
	numIndices = 3 * ( 20 + 20 + ( 10 * 40 ) );

}