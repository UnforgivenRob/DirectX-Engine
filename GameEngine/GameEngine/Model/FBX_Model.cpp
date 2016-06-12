#include "FBX_Model.h"
#include "GraphicsEngine.h"

FBX_Model::FBX_Model( Model_ID id, const char* fileName  )
	: Model( id )
{
	initModel( fileName );
}
		
FBX_Model::~FBX_Model()
{
}

void FBX_Model::initModel( const char* fileName ) 
{
	fileName;
}