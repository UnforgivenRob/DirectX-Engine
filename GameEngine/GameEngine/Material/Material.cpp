#include "Material.h"
#include "Game.h"
#include "Shader.h"
#include "GraphicsEngine.h"

Material::Material( Material_ID id, Shader* inShader )
	: id( id ), shader( inShader ), device( GraphicsEngine::getDevice() )
{
}

Material::~Material(void)
{
}

Shader* Material::activate()
{
	
	return shader;
}

ComPtr<ID3D12PipelineState> Material::getPSO()
{
	return pipelineState;
}
