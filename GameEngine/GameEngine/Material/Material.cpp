#include "Material.h"
#include "Game.h"
#include "Shader.h"

Material::Material( Game* inGame )
	: device( inGame->getDevice() ), context( inGame->getContext() )
{
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FrontCounterClockwise = TRUE;

	device->CreateRasterizerState( &rastDesc, &rastState );
}

Material::~Material(void)
{
}

Shader* Material::activate()
{
	context->RSSetState( rastState );
	return 0;
}
