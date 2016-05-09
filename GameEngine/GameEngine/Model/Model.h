#ifndef MODEL_H
#define MODEL_H
#include <d3d11.h>

class Game;

class Model
{
public:
	Model( Game* inGame );
	Model( const char* filename );
	~Model();

	ID3D11Buffer** getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
	unsigned int* getStride();
	unsigned int* getOffset();

protected:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	unsigned int stride;
	unsigned int offset;
private:
	virtual void initModel( const char* fileName );
	
	//prevent from being called
	Model( Model& inModel );
	Model& operator = ( Model& inModel );
};

#endif

