#include "Game.h"
#include "Time.h"
#include <stdio.h>
#include <assert.h>
#include "ShaderManager.h"
#include "ModelManager.h"
#include "MaterialManager.h"
#include "GraphicsObject.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"

Game::Game( HINSTANCE hInstance )
	: Engine( hInstance )
{
	bgColor = Vect( 0.3f, 0.35f, 0.3f, 1.0f );
	FullTimer.tic();
	intervalTimer.tic();
}

Game::~Game()
{
}

void Game::Initialize()
{
	Device = GraphicsEngine::getDevice();
	Context = GraphicsEngine::getContext();
}

void Game::LoadContent()
{
	ShaderManager::create( Shader_ID::Base, "Base", false );

	MaterialManager::createBaseMat( Material_ID::Base_Solid, ShaderManager::get( Shader_ID::Base ) );

	ModelManager::createCube( Model_ID::Cube_Model );
	ModelManager::createGrid( Model_ID::Grid_Model, 100, 100, 1 );

	GraphicsObject* go1 = new GraphicsObject( Model_ID::Cube_Model, Material_ID::Base_Solid );
	GameObjectManager::create( GameObject_ID::Cube, go1 );
	GameObjectManager::get( GameObject_ID::Cube )->setStaticScale( Matrix( SCALE, .1f, .1f, .1f ) );
	GameObjectManager::get( GameObject_ID::Cube )->setStaticTrans( Matrix( TRANS, .5f, .5f, .5f ) );
}

void Game::Update()
{
	intervalTimer.toc();
	intervalTimer.tic();

	Time current = FullTimer.toc();
	
	Matrix id = Matrix( IDENTITY );
	GameObjectManager::get( GameObject_ID::Cube )->update( current, id, id );
}

void Game::Draw()
{
	Matrix id = Matrix( IDENTITY );
	GameObjectManager::get( GameObject_ID::Cube )->draw( id );
}

void Game::ClearBuffers()
{
	GraphicsEngine::ClearBuffers( bgColor );
}

void Game::UnloadContent()
{
	MaterialManager::clear();
	ShaderManager::clear();
	ModelManager::clear();
	GameObjectManager::clear();
}
