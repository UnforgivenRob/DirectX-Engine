#include "MaterialManager.h"
//#include "TextMat.h"
#include "BaseMat.h"
//#include "WireframeMat.h"
//#include "PointLightMat.h"

ManagerList* MaterialManager::manager = 0;
MaterialManager MaterialManager::instance = MaterialManager();

MaterialManager::MaterialManager()
{
	if(manager != 0)
	{
		delete manager;
	}
	this->manager = new ManagerList();
}

MaterialManager::~MaterialManager()
{
	delete manager;
	manager = 0;
}

void MaterialManager::Activate()
{
	if(instance.manager == 0)
	{
		manager = new ManagerList();
	}
}

void MaterialManager::Deactivate()
{
	delete manager;
	manager = 0;
}

void MaterialManager::createBaseMat(Material_ID id, Shader* shader)
{
	MaterialManager::add(new BaseMat(id, shader), id);
}

void MaterialManager::createWireframeMat(Material_ID id, Shader* shader)
{
	id, shader;
	//MaterialManager::add(new WireframeMat(id, shader), id);
}

void MaterialManager::createTextMat(Material_ID id, Shader* shader)
{
	id, shader;
	//MaterialManager::add(new TextMat(id, shader), id);
}

void MaterialManager::createLightMat(Material_ID id, Shader* shader)
{
	id, shader;
	//MaterialManager::add(new PointLightMat(id, shader), id);
}

void MaterialManager::add(Material* inText, Material_ID id)
{
	instance.manager->add((Node*)inText, id);
}

void MaterialManager::remove(Material_ID id)
{
	instance.manager->remove(id);
}

void MaterialManager::clear()
{
	instance.manager->clear();
}

Material* MaterialManager::get(Material_ID id)
{
	return (Material*)instance.manager->get(id);
}