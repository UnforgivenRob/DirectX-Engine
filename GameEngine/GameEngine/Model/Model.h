#ifndef MODEL_H
#define MODEL_H
#include <d3d12.h>
#include <wrl/client.h>
#include "Node.h"

using Microsoft::WRL::ComPtr;

enum Model_ID
{
	Cube_Model,
	Grid_Model,
	Cylinder_Model,
	Sphere_Model,
};

class Model: public Node
{
public:

	Model( Model_ID id );
	virtual ~Model();

	ComPtr<ID3D12Resource> getVertexBuffer();
	ComPtr<ID3D12Resource> getIndexBuffer();
	unsigned int* getStride();
	unsigned int* getOffset();
	unsigned int getNumIndices();
	unsigned int getNumVertices();

protected:
	ComPtr<ID3D12Resource> vertexBuffer;
	ComPtr<ID3D12Resource> indexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	unsigned int numVerts;
	unsigned int numIndices;
	unsigned int stride;
	unsigned int offset;
	Model_ID id;
private:
	
	//prevent from being called
	Model();
	Model( Model& inModel );
	Model& operator = ( Model& inModel );
};

#endif

