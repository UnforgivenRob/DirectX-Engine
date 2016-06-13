#include "GameObjectManager.h"

ManagerList* GameObjectManager::manager = 0;
GameObjectManager GameObjectManager::instance = GameObjectManager();

GameObjectManager::GameObjectManager()
{
	if(manager != 0)
	{
		delete manager;
	}
	this->manager = new ManagerList();
}

GameObjectManager::~GameObjectManager()
{
	delete manager;
	manager = 0;
}

void GameObjectManager::Activate()
{
	if(instance.manager == 0)
	{
		manager = new ManagerList();
	}
}

void GameObjectManager::Deactivate()
{
	delete manager;
	manager = 0;
}

void GameObjectManager::create(GameObject_ID id, GraphicsObject* graphObj)
{
	GameObjectManager::add(new GameObject(id, graphObj), id);
}


void GameObjectManager::add(GameObject* inGO, GameObject_ID id)
{
	instance.manager->add((Node*)inGO, id);
}

void GameObjectManager::remove(GameObject_ID id)
{
	instance.manager->remove(id);
}

void GameObjectManager::clear()
{
	instance.manager->clear();
}

GameObject* GameObjectManager::get(GameObject_ID id)
{
	return (GameObject*)instance.manager->get(id);
}