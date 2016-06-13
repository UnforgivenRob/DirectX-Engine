#ifndef  GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include "ManagerList.h"
#include "GameObject.h"

class GameObjectManager
{
public:

	static void Activate();

	static void create( GameObject_ID id, GraphicsObject* graphObj );

	static void add( GameObject* inGO, GameObject_ID id );
	static void remove( GameObject_ID id );
	static void clear();
	static GameObject* get( GameObject_ID id );
	static void Deactivate();
private:
	GameObjectManager();
	~GameObjectManager();

	static GameObjectManager instance;
	static ManagerList* manager;
};

#endif