#include "ShaderManager.h"

ManagerList* ShaderManager::manager = 0;
ShaderManager ShaderManager::instance = ShaderManager();

ShaderManager::ShaderManager()
{
	if(manager != 0)
	{
		delete manager;
	}
	this->manager = new ManagerList();
}

ShaderManager::~ShaderManager()
{
	delete manager;
	manager = 0;
}

void ShaderManager::Activate()
{
	if(instance.manager == 0)
	{
		manager = new ManagerList();
	}
}

void ShaderManager::Deactivate()
{
	delete manager;
	manager = 0;
}
void ShaderManager::create(Shader_ID id, const char* name, bool bIsCompute)
{
	bIsCompute;
	ShaderManager::add(new Shader( id, name), id);
}

void ShaderManager::add(Shader* inShader, Shader_ID id)
{
	instance.manager->add((Node*)inShader, id);
}

void ShaderManager::remove(Shader_ID id)
{
	instance.manager->remove(id);
}

void ShaderManager::clear()
{
	instance.manager->clear();
}

Shader* ShaderManager::get(Shader_ID id)
{
	return (Shader*)instance.manager->get(id);
}