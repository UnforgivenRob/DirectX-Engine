#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "ManagerList.h"
#include "Material.h"

class MaterialManager
{
public:
	
	static void Activate();
	static void createBaseMat(Material_ID id, Shader* shader);
	static void createWireframeMat(Material_ID id, Shader* shader);
	static void createTextMat(Material_ID id, Shader* shader);
	static void createLightMat(Material_ID id, Shader* shader);

	static void add(Material* inText, Material_ID id);
	static void remove(Material_ID id);
	static void clear();
	static Material* get(Material_ID id);
	static void Deactivate();
private:
	MaterialManager();
	~MaterialManager();
	
	static MaterialManager instance;
	static ManagerList*		  manager;

};

#endif // ! MaterialMANAGER_H
