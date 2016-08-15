#ifndef CYLINDER_H
#define CYLINDER_H

#include "Primitive.h"

class Cylinder : public Primitive
{
public:
	Cylinder( Model_ID id );
		
	~Cylinder();

private:
	virtual void initModel() override;
	
	//prevent from being called
	Cylinder();
	Cylinder( Cylinder& inModel );
	Cylinder& operator= ( Cylinder& inModel );
};

#endif