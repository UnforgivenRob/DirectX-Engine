#include "GraphicsObject.h"
#include "MaterialManager.h"
#include "ModelManager.h"
#include "Shader.h"
#include "ConstantBuffers.h"
#include "GraphicsEngine.h"

GraphicsObject::GraphicsObject( Model_ID modelID, Material_ID material )
	: model_Mat( IDENTITY ), view_Mat( IDENTITY )
{
	this->material = MaterialManager::get(material);
	this->model = ModelManager::get(modelID);
}

GraphicsObject::~GraphicsObject()
{
}

void GraphicsObject::update( Matrix& inModel_Mat, Matrix& inView_Mat )
{
	this->model_Mat = inModel_Mat;
	this->view_Mat = inView_Mat;
}

Model* GraphicsObject::getModel()
{
	return this->model;
}

void GraphicsObject::setModel( Model_ID modID )
{
	this->model = ModelManager::get( modID );
}

Material* GraphicsObject::getMaterial()
{
	return this->material;
}

Matrix& GraphicsObject::getModelView()
{
	return this->model_Mat;
}

unsigned int GraphicsObject::getID()
{
	return id;
}
