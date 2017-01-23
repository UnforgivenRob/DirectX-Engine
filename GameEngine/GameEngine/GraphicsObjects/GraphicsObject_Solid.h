#ifndef GRAPHICSOBJECT_SOLID_H
#define GRAPHICSOBJECT_SOLID_H

#include "GraphicsObject.h"

class GraphicsObject_Solid : public GraphicsObject
{
public:
	GraphicsObject_Solid( Model_ID modelID, Material_ID material, Vect& color );
	virtual ~GraphicsObject_Solid();

	virtual void draw( Matrix& proj_Mat ) override;

	//overload operator new and delete to ensure aligned 16
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		return _mm_free(p);
	}

private:
	GraphicsObject_Solid();

protected:
	Vect color;
};

#endif