#ifndef GRAPHICSOBJECT_SOLID_H
#define GRAPHICSOBJECT_SOLID_H

#include "GraphicsObject.h"

class GraphicsObject_Solid : public GraphicsObject
{
public:
	GraphicsObject_Solid( Model_ID modelID, Material_ID material, Vect& color );
	virtual ~GraphicsObject_Solid();

	virtual void draw( Matrix& proj_Mat ) override;

private:
	GraphicsObject_Solid();

protected:
	Vect color;
};

#endif