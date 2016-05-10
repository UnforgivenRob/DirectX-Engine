#include "Model.h"
#include "MathEngine.h"
#include "Game.h"

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

Model::Model( Game* inGame )
	: vertexBuffer( 0 ), indexBuffer( 0 ), stride( sizeof(vert) ), offset( 0 )
{
	vert verts[] = 
	{
		{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{ -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
	//	{ 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
	//	{ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
	//	{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
	//	{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
	};

	unsigned int indices[] = {  0, 2, 1,
								0, 3, 2,
								//0, 4, 3,
								//3, 4, 7,
								//0, 1, 4, 
								//1, 5, 4,
								//4, 5, 6,
								//4, 6, 7,
								//3, 6, 2,
								//3, 7, 6,
								//1, 2, 5,
								//2, 6, 5
							};

	ID3D11Device* device = inGame->getDevice();

	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage           = D3D11_USAGE_IMMUTABLE;
	vertexDesc.ByteWidth       = 4 * sizeof( vert );
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
	indexDesc.ByteWidth       = /*6 * */ 2 * 3 * sizeof( unsigned int );
	indexDesc.BindFlags       = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags  = 0;
	indexDesc.MiscFlags       = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	res = device->CreateBuffer( &indexDesc, &indexData, &indexBuffer );
	assert( res == S_OK );

	inGame->getContext()->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset ); 
	inGame->getContext()->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R32_UINT, 0 ); 
}

Model::Model( const char* filename )
{
	initModel( filename);
}

Model::~Model()
{
}

void Model::initModel( const char* filename )
{
	filename;
}

ID3D11Buffer** Model::getVertexBuffer()
{
	return &vertexBuffer;
}

ID3D11Buffer* Model::getIndexBuffer()
{
	return indexBuffer;
}

unsigned int* Model::getStride()
{
	return &stride;
}

unsigned int* Model::getOffset()
{
	return &offset;
}