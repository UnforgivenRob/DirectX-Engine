#ifndef FBX_MODEL_H
#define FBX_MODEL_H

#include "Model.h"

class FBX_Model : public Model
{
public:
	FBX_Model( Model_ID id, const char* fileName  );
	
	//FBXModel(Model_ID id, Vert_Final_Skinned* vertexBuffer, int* indexBuffer, VBO_header& vboHead, Vect& center, float radius);
	
	~FBX_Model();

private:
	void initModel( const char* fileName );
	
	//void initModel(Vert_Final_Skinned* vertexBuffer, int* indexBuffer, VBO_header& vboHead, Vect& center, float radius);
	
	//prevent from being called
	FBX_Model();
	FBX_Model( FBX_Model& inModel );
	FBX_Model& operator= ( FBX_Model& inModel );
};

#endif