#include "Primitive.h"
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

Primitive::Primitive( Model_ID id )
	: Model( id )
{
	initModel();
}
		
Primitive::~Primitive()
{
}

void Primitive::initModel() 
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

	ID3D11Device* device = GraphicsEngine::getDevice();

	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage           = D3D11_USAGE_IMMUTABLE;
	vertexDesc.ByteWidth       = 8 * sizeof( vert );
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
	indexDesc.ByteWidth       = 6 * 2 * 3 * sizeof( unsigned int );
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