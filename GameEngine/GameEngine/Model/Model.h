#ifndef MODEL_H
#define MODEL_H
#include <d3d11.h>
#include "Node.h"

enum Model_ID
{
	Cube_Model,

};

class Model: public Node
{
public:

	Model( Model_ID id );
	virtual ~Model();

	ID3D11Buffer** getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
	unsigned int* getStride();
	unsigned int* getOffset();

protected:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
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

