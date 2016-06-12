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