#include "Material.h"
#include "Game.h"
#include "Shader.h"
#include "GraphicsEngine.h"

Material::Material( Material_ID id, Shader* inShader )
	: id( id ), shader( inShader ), device( GraphicsEngine::getDevice() ), context( GraphicsEngine::getContext() )
{
}

Material::~Material(void)
{
}

Shader* Material::activate()
{
	context->RSSetState( rastState );
	return shader;
}
