#include "Game.h"
#include "Time.h"
#include <stdio.h>
#include <assert.h>
#include "CameraManager.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include "MaterialManager.h"
#include "GraphicsObject_Solid.h"
#include "GraphicsObject_Base.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "InputManager.h"
#include "ConstantBuffers.h"
#include <dos.h>

Game::Game( HINSTANCE hInstance )
	: Engine( hInstance )
{
	bgColor = Vect( 0.2f, 0.2f, 0.2f, 1.0f );
	
	FullTimer.tic();
	intervalTimer.tic();
}

Game::~Game()
{
	File::close(fh);
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
	CameraManager::create( Camera1, Vect( 0.0f, 50.0f, 100.0f ), Vect ( 0.0f, 10.0f, 0.0f ), Vect( 0.0f, 1.0f, 0.0f ) );
	CameraManager::get( Camera1 )->setFrustumData( 50.0f, 800.0f/600.0f, 0.1f, 10000.0f );
	CameraManager::setActive( Camera1 );

	ShaderManager::create( Shader_ID::Base, "Base", sizeof( baseBuffer ), false );
	ShaderManager::create( Shader_ID::Solid, "Solid", sizeof( solidBuffer ), false );

	MaterialManager::createBaseMat( Material_ID::Base_Solid, ShaderManager::get( Shader_ID::Base ) );
	MaterialManager::createWireframeMat( Material_ID::Solid_Wire, ShaderManager::get( Shader_ID::Solid ) );

	ModelManager::createCube( Model_ID::Cube_Model );
	ModelManager::createGrid( Model_ID::Grid_Model, 100, 100, 1 );
	ModelManager::createCylinder( Model_ID::Cylinder_Model );
	ModelManager::createSphere( Model_ID::Sphere_Model );

	GraphicsObject* go1 = new GraphicsObject_Base( Model_ID::Cube_Model, Material_ID::Base_Solid );
	GameObjectManager::create( GameObject_ID::Cube, go1 );
	GameObjectManager::get( GameObject_ID::Cube )->setStaticScale( Matrix( SCALE, 10.0f, 10.0f, 10.0f ) );
	GameObjectManager::get( GameObject_ID::Cube )->setStaticTrans( Matrix( TRANS, 50.0f, 30.0f, .5f ) );

	GraphicsObject* go2 = new GraphicsObject_Base( Model_ID::Cylinder_Model, Material_ID::Base_Solid );
	GameObjectManager::create( GameObject_ID::Cylinder, go2 );
	GameObjectManager::get( GameObject_ID::Cylinder )->setStaticScale( Matrix( SCALE, 10.0f, 10.0f, 10.0f ) );
	GameObjectManager::get( GameObject_ID::Cylinder )->setStaticTrans( Matrix( TRANS, 0.0f, 30.0f, .5f ) );

	GraphicsObject* go3 = new GraphicsObject_Base( Model_ID::Sphere_Model, Material_ID::Base_Solid );
	GameObjectManager::create( GameObject_ID::Sphere, go3 );
	GameObjectManager::get( GameObject_ID::Sphere )->setStaticScale( Matrix( SCALE, 10.0f, 10.0f, 10.0f ) );
	GameObjectManager::get( GameObject_ID::Sphere )->setStaticTrans( Matrix( TRANS, -50.0f, 30.0f, .5f ) );

	Vect color( 0.0f, 0.0f, 1.0f );
	GraphicsObject* go4 = new GraphicsObject_Solid( Model_ID::Grid_Model, Material_ID::Solid_Wire, color );
	GameObjectManager::create( GameObject_ID::Grid, go4 );
	GameObjectManager::get( GameObject_ID::Grid )->setStaticRot( Matrix( ROT_XYZ, 90.0f * MATH_PI_180, 90.0f * MATH_PI_180, 0.0f * MATH_PI_180 ) );
	GameObjectManager::get( GameObject_ID::Grid )->setStaticScale( Matrix( SCALE, 10.0f, 10.0f, 10.0f ) );
	GameObjectManager::get( GameObject_ID::Grid )->setStaticTrans( Matrix( TRANS, .5f, .5f, .5f ) );
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
	GameObjectManager::get( GameObject_ID::Cylinder )->update( current, id, CameraManager::getActive()->getViewMat() );
	GameObjectManager::get( GameObject_ID::Sphere )->update( current, id, CameraManager::getActive()->getViewMat() );
	GameObjectManager::get( GameObject_ID::Grid )->update( current, id, CameraManager::getActive()->getViewMat() );
}

void Game::Draw()
{
	//Matrix id = Matrix( IDENTITY );
	GameObjectManager::get( GameObject_ID::Cube )->draw( CameraManager::getActive()->getProjMat() );
	GameObjectManager::get( GameObject_ID::Cylinder )->draw( CameraManager::getActive()->getProjMat() );
	GameObjectManager::get( GameObject_ID::Sphere )->draw( CameraManager::getActive()->getProjMat() );
	GameObjectManager::get( GameObject_ID::Grid )->draw( CameraManager::getActive()->getProjMat() );
}

void Game::ClearBuffers()
{
	HRESULT res = GraphicsEngine::getSwapChain()->Present ( 0, 0 );
	assert( res == S_OK );
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
	float trans = .04f;//9.0f;
	float rot = .016f * MATH_PI_180;
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
