#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "MathEngine.h"
#include "Node.h"
#include "GraphicsObject.h"
#include "Time.h"
#include "GOData.h"

enum GameObject_ID
{
	Cube,
};

class GameObject : public Node
{
public:
	GameObject( GameObject_ID id, GraphicsObject* go );
	virtual ~GameObject();

	virtual void update(Time currentTime, Matrix& inWorld_Mat, Matrix& view_Mat);// = 0;
	virtual void draw(Matrix& proj_Mat);// = 0;

	void setStaticRot(const Matrix& inMat);
	void setStaticScale(const Matrix& inMat);
	void setStaticTrans(const Matrix& inMat);
	
	GOData* getGOData();
	GraphicsObject* getGraphicsObject();
	
	Matrix world_Mat;

private:
	GameObject();

protected:
	GameObject_ID id;
	GraphicsObject* graphicsObj;

	//basic transformations
	GOData* data;
};

#endif