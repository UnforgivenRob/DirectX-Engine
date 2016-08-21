#include "ModelManager.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "GridModel.h"
#include "FBX_Model.h"

ManagerList* ModelManager::manager = 0;
ModelManager ModelManager::instance = ModelManager();

ModelManager::ModelManager()
{
	if(manager != 0)
	{
		delete manager;
	}
	this->manager = new ManagerList();
}

ModelManager::~ModelManager()
{
	delete manager;
	manager = 0;
}

void ModelManager::Activate()
{
	if(instance.manager == 0)
	{
		manager = new ManagerList();
	}
}

void ModelManager::Deactivate()
{
	delete manager;
	manager = 0;
}

void ModelManager::create( Model_ID id, const char* fileName )
{
	ModelManager::add( new FBX_Model( id, fileName ), id );
}

void ModelManager::createCube( Model_ID id )
{
	ModelManager::add( new Cube( id ), id );
}

void ModelManager::createGrid( Model_ID id, int width, int length, int step )
{
	ModelManager::add( new GridModel( id, width, length, step ), id );
}

void ModelManager::createCylinder( Model_ID id )
{
	ModelManager::add( new Cylinder( id ), id );
}

void ModelManager::createSphere( Model_ID id )
{
	ModelManager::add( new Sphere( id ), id );
}

void ModelManager::add(Model* inText, Model_ID id)
{
	instance.manager->add((Node*)inText, id);
}

void ModelManager::remove(Model_ID id)
{
	instance.manager->remove(id);
}

void ModelManager::clear()
{
	instance.manager->clear();
}

Model* ModelManager::get(Model_ID id)
{
	return (Model*)instance.manager->get(id);
}