#include "WireframeMat.h"
#include "GraphicsEngine.h"

WireframeMat::WireframeMat( Material_ID id,  Shader* inShader )
	: Material( id, inShader )
{
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FrontCounterClockwise = TRUE;

	device->CreateRasterizerState( &rastDesc, &rastState );
}

WireframeMat::~WireframeMat(void)
{
}