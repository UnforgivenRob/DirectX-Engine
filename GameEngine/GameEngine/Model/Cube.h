#ifndef CUBE_H
#define CUBE_H

#include "Primitive.h"

class Cube : public Primitive
{
public:
	Cube( Model_ID id );
		
	~Cube();

private:
	virtual void initModel() override;
	
	//prevent from being called
	Cube();
	Cube( Cube& inModel );
	Cube& operator= ( Cube& inModel );
};

#endif