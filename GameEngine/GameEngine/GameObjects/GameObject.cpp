#include "GameObject.h"

GameObject::GameObject()
	: Node()
{
}

GameObject::GameObject( GameObject_ID id, GraphicsObject* go )
	: id( id ), graphicsObj( go ), Node()
{
	//set up data
	data = new GOData();
}

GameObject::~GameObject()
{
	delete data;
	delete graphicsObj;
}

void GameObject::update(Time currentTime, Matrix& inWorld_Mat, Matrix& view_Mat)
{
	currentTime;
	data->Rotation *= Matrix(ROT_XYZ, .0005f, .0005f, .0005f );
	world_Mat =  inWorld_Mat * data->Scale * data->Rotation * data->Translation;
	Matrix id = Matrix( IDENTITY );
	graphicsObj->update( world_Mat, view_Mat );
}

void GameObject::draw(Matrix& proj_Mat)
{
	graphicsObj->draw( proj_Mat );
}

void GameObject::setStaticRot( const Matrix& inMat )
{
	this->data->Rotation = inMat;
}

void GameObject::setStaticScale( const Matrix& inMat )
{
	this->data->Scale = inMat;
}

void GameObject::setStaticTrans( const Matrix& inMat )
{
	this->data->Translation = inMat;
}

GOData* GameObject::getGOData()
{
	return this->data;
}

GraphicsObject* GameObject::getGraphicsObject()
{
	return graphicsObj;
}