#include "Game.h"
#include "Time.h"
#include <stdio.h>
#include <assert.h>
#include "CameraManager.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include "MaterialManager.h"
#include "GraphicsObject.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "InputManager.h"

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

	InputManager::Activate();
	CameraManager::Activate();
	ShaderManager::Activate();
	MaterialManager::Activate();
	ModelManager::Activate();
	GameObjectManager::Activate();
}

void Game::LoadContent()
{
	CameraManager::create( Camera1, Vect( 100.0f, 0.0f, 0.0f ), Vect ( 0.0f, 0.0f, 0.0f ), Vect( 0.0f, 1.0f, 0.0f ) );
	CameraManager::get( Camera1 )->setFrustumData( 50.0f, 800.0f/600.0f, 0.1f, 10000.0f );
	CameraManager::setActive( Camera1 );

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

	CheckInput();

	Matrix id = Matrix( IDENTITY );
	CameraManager::update( current );

	GameObjectManager::get( GameObject_ID::Cube )->update( current, id, CameraManager::getActive()->getViewMat() );
}

void Game::Draw()
{
	//Matrix id = Matrix( IDENTITY );
	GameObjectManager::get( GameObject_ID::Cube )->draw( CameraManager::getActive()->getProjMat() );
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
	CameraManager::clear();

	MaterialManager::Deactivate();
	ShaderManager::Deactivate();
	ModelManager::Deactivate();
	GameObjectManager::Deactivate();
	CameraManager::Deactivate();
	InputManager::Deactivate();
}

void Game::CheckInput()
{
	float trans = .2f;//9.0f;
	float rot = .04f * MATH_PI_180;
	Camera* cam1 = CameraManager::getActive();
	Keyboard* keyboard = InputManager::getKeyboard();

	if(keyboard->isPressed(Keyboard_Key::KEY_W))
		{
			cam1->translateZ(-trans);
		}

		if(keyboard->isPressed(Keyboard_Key::KEY_S))
		{
			cam1->translateZ(trans);
		}
		//along x(right) axis
		if(keyboard->isPressed(Keyboard_Key::KEY_D))
		{
			cam1->translateX(trans);
		}

		if(keyboard->isPressed(Keyboard_Key::KEY_A))
		{
			cam1->translateX(-trans);
		}

		//along y(up) axis
		if(keyboard->isPressed(Keyboard_Key::KEY_E))
		{
			cam1->translateY(trans);
		}

		if(keyboard->isPressed(Keyboard_Key::KEY_Q))
		{
			cam1->translateY(-trans);
		}

		//camera rotations
		//pitching
		if(keyboard->isPressed(Keyboard_Key::KEY_ARROW_UP))
		{
			cam1->pitch(-rot);
		}

		if(keyboard->isPressed(Keyboard_Key::KEY_ARROW_DOWN))
		{
			cam1->pitch(rot);
		}

		//yawing
		if(keyboard->isPressed(Keyboard_Key::KEY_ARROW_LEFT))
		{
			cam1->yaw(-rot);
		}

		if(keyboard->isPressed(Keyboard_Key::KEY_ARROW_RIGHT))
		{
			cam1->yaw(rot);
		}

		//rolling
		if(keyboard->isPressed(Keyboard_Key::KEY_Z))
		{
			cam1->roll(-rot);
		}

		if(keyboard->isPressed(Keyboard_Key::KEY_X))
		{
			cam1->roll(rot);
		}
}


//keyboard input handling functings
void Game::OnKeyDown( WPARAM keyState ) 
{ 
	keyState;
	/*switch ( keyState )
	{
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		break;
	default:
		break;
	} */
}
void Game::OnKeyUp( WPARAM keyState ) 
{ 
	keyState;
	//switch ( keyState )
	//{
	//case VK_UP:
	//case VK_DOWN:
	//case VK_LEFT:
	//case VK_RIGHT:
	//	break;
	//default:
	//	break;
	//} 
}
