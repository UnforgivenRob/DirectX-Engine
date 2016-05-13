#ifndef MATERIAL_H
#define MATERIAL_H
#include <d3d11.h>
class Game;
class Shader;

class Material
{
public:
	Material();
	~Material(void);

	virtual Shader* activate();

private:
	ID3D11RasterizerState* rastState;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
};

#endif

