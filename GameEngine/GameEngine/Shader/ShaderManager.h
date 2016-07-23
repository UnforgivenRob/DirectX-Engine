#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <FileSystem.h>
#include "ManagerList.h"
#include "Shader.h"

class ShaderManager
{
public:
	static void Activate();
	static void create(Shader_ID id, const char* name, int cb_Size, bool bIsCompute);
	static void add(Shader* inShader, Shader_ID id);
	static void remove(Shader_ID id);
	static void clear();
	static Shader* get(Shader_ID id);
	static void Deactivate();
private:
	ShaderManager();
	~ShaderManager();

	static ShaderManager instance;
	static ManagerList* manager;
};

#endif
