#ifndef SPHERE_H
#define SPHERE_H

#include "Primitive.h"

class Sphere : public Primitive
{
public:
	Sphere( Model_ID id );
		
	~Sphere();

private:
	virtual void initModel() override;
	
	//prevent from being called
	Sphere();
	Sphere( Sphere& inModel );
	Sphere& operator= ( Sphere& inModel );
};

#endif