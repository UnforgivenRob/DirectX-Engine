#ifndef MATERIAL_H
#define MATERIAL_H
#include "Node.h"
#include <d3d11.h>
class Game;
class Shader;

enum Material_ID
{
	Base_Solid,
	Base_Wire,

};

class Material : public Node
{
public:
	Material( Material_ID id, Shader* inShader );
	~Material(void);

	virtual Shader* activate();

protected:
	ID3D11RasterizerState* rastState;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	Shader* shader;
	Material_ID id;

private:
	Material();
};

#endif

