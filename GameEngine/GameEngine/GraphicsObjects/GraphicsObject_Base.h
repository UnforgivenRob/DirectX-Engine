#ifndef GRAPHICSOBJECT_BASE_H
#define GRAPHICSOBJECT_BASE_H

#include "GraphicsObject.h"

class GraphicsObject_Base : public GraphicsObject
{
public:
	GraphicsObject_Base( Model_ID modelID, Material_ID material );
	virtual ~GraphicsObject_Base();

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
	GraphicsObject_Base();

protected:
};

#endif