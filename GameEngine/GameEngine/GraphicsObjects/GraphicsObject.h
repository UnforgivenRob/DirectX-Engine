#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include "Model.h"
#include "Material.h"
#include "MathEngine.h"

class GraphicsObject : public Node
{
public:
	GraphicsObject( Model_ID modelID, Material_ID material );
	virtual ~GraphicsObject();

	void update( Matrix& inModel_Mat, Matrix& inView_Mat );
	virtual void draw( Matrix& proj_Mat ) = 0;
	Model* getModel();
	void setModel( Model_ID modID );
	Material* getMaterial();
	Matrix& getModelView();
	unsigned int getID();

private:
	GraphicsObject();

protected:
	Material* material;
	Model* model; 

	Matrix model_Mat;
	Matrix view_Mat;
};

#endif