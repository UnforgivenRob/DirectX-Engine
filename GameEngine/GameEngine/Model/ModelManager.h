#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "ManagerList.h"
#include "Model.h"

class ModelManager
{
public:
	static void Activate();
	
	static void create( Model_ID id, const char* fileName );
	static void createCube( Model_ID id );
	static void createGrid( Model_ID id, int width, int length, int step );
	

	static void add(Model* inText, Model_ID id);
	static void remove(Model_ID id);
	static void clear();
	static Model* get(Model_ID id);
	static void Deactivate();
private:
	ModelManager();
	~ModelManager();
	static ModelManager instance;
	static ManagerList*	manager;
};

#endif