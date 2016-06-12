#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Model.h"

class Primitive : public Model
{
public:
	Primitive( Model_ID id );
		
	~Primitive();

private:
	virtual void initModel();
	
	//prevent from being called
	Primitive();
	Primitive( Primitive& inModel );
	Primitive& operator= ( Primitive& inModel );
};

#endif