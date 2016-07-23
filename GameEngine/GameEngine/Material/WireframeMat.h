#ifndef WIREFRAMEMAT_H
#define WIREFRAMEMAT_H

#include "Material.h"

class WireframeMat : public Material
{
public:
	WireframeMat( Material_ID id, Shader* inShader );
	~WireframeMat();
};

#endif