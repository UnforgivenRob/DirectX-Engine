#ifndef BASEMAT_H
#define BASEMAT_H

#include "Material.h"

class BaseMat : public Material
{
public:
	BaseMat( Material_ID id, Shader* inShader );
	~BaseMat();
};

#endif