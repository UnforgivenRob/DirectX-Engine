#ifndef GRAPHICSOBJECT_BASE_H
#define GRAPHICSOBJECT_BASE_H

#include "GraphicsObject.h"

class GraphicsObject_Base : public GraphicsObject
{
public:
	GraphicsObject_Base( Model_ID modelID, Material_ID material );
	virtual ~GraphicsObject_Base();

	virtual void draw( Matrix& proj_Mat ) override;

private:
	GraphicsObject_Base();

protected:
};

#endif