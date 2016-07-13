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
#include "Camera.h"

Camera* cam1 = 0;

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
	cam1 = new Camera( Camera1, Vect( 100.0f, 0.0f, 0.0f ), Vect ( 0.0f, 0.0f, 0.0f ), Vect( 0.0f, 1.0f, 0.0f ) );
	cam1->setFrustumData( 50.0f, 800.0f/600.0f, 0.1f, 10000.0f );
	ShaderManager::create( Shader_ID::Base, "Base", false );

	MaterialManager::createBaseMat( Material_ID::Base_Solid, ShaderManager::get( Shader_ID::Base ) );

	ModelManager::createCube( Model_ID::Cube_Model );
	ModelManager::createGrid( Model_ID::Grid_Model, 100, 100, 1 );

	GraphicsObject* go1 = new GraphicsObject( Model_ID::Cube_Model, Material_ID::Base_Solid );
	GameObjectManager::create( GameObject_ID::Cube, go1 );
	GameObjectManager::get( GameObject_ID::Cube )->setStaticScale( Matrix( SCALE, 10.0f, 10.0f, 10.0f ) );
	GameObjectManager::get( GameObject_ID::Cube )->setStaticTrans( Matrix( TRANS, .5f, .5f, .5f ) );
}

void Game::Update()
{
	intervalTimer.toc();
	intervalTimer.tic();

	Time current = FullTimer.toc();
	
	Matrix id = Matrix( IDENTITY );
	cam1->update( current );

	GameObjectManager::get( GameObject_ID::Cube )->update( current, id, cam1->getViewMat() );
}

void Game::Draw()
{
	//Matrix id = Matrix( IDENTITY );
	GameObjectManager::get( GameObject_ID::Cube )->draw( cam1->getProjMat() );
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

//keyboard input handling functings
void Game::OnKeyDown( WPARAM keyState ) 
{ 
	float trans = 9.0f;//.9f;
	switch ( keyState )
	{
	case VK_UP:
		cam1->translateZ(-trans);
		break;
	case VK_DOWN:
		cam1->translateZ(trans);
		break;
	case VK_LEFT:
		cam1->translateX(-trans);
		break;
	case VK_RIGHT:
		cam1->translateX(trans);
		break;

	default:
		break;
	} 
}
void Game::OnKeyUp( WPARAM keyState ) 
{ 
	switch ( keyState )
	{
	case VK_UP:
		break;
	case VK_DOWN:
		break;
	case VK_LEFT:
		break;
	case VK_RIGHT:
		break;
	default:
		break;
	} 
}
