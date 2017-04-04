#include "BaseMat.h"
#include "GraphicsEngine.h"

BaseMat::BaseMat( Material_ID id,  Shader* inShader )
	: Material( id, inShader )
{
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FrontCounterClockwise = TRUE;

	device->CreateRasterizerState( &rastDesc, &rastState );
}

BaseMat::~BaseMat(void)
{
}