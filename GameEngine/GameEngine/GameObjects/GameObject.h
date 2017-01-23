#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "MathEngine.h"
#include "Node.h"
#include "GraphicsObject.h"
#include "Time.h"
#include "GOData.h"

enum GameObject_ID
{
	Grid,
	Cube,
	Cylinder,
	Sphere,
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

	//overload operator new and delete to ensure aligned 16
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		return _mm_free(p);
	}
	
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
