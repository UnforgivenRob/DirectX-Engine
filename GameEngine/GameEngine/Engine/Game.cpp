#include "Game.h"
#include "Time.h"
#include <stdio.h>
#include <assert.h>
#include "ShaderManager.h"
#include "ModelManager.h"
#include "MaterialManager.h"
#include "ConstantBuffers.h"
#include "GraphicsEngine.h"

Game::Game( HINSTANCE hInstance )
	: Engine( hInstance )
{
	bgColor = Vect( 1.0f, 1.0f, 1.0f, 1.0f );
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
}

Matrix Rot = Matrix (IDENTITY );

void Game::Update()
{
	intervalTimer.toc();
	intervalTimer.tic();

	static int cnt = 0;
	static Time prev = Time( TIME_ZERO );
	static Time dif  = Time( TIME_ZERO );
	Time current = FullTimer.toc();
	dif += current - prev;
	prev = current;

	if( dif >=  Time( TIME_ONE_SECOND ) )
	{
		dif = Time ( TIME_ZERO );
		printf( "Tic%d\n", cnt );
		cnt++;
	}

	bgColor[x] = 1.0f / (float)cnt;
	bgColor[y] = 1.0f / (float)cnt;
	bgColor[z] = 1.0f / (float)cnt;

	Rot *= Matrix(ROT_XYZ, .0005f, .0005f, .0005f );
}

void Game::Draw()
{
	Shader* shade = MaterialManager::get( Material_ID::Base_Solid )->activate();
	Context->VSSetShader( shade->getVertexShader(), nullptr, 0 );
	Context->PSSetShader( shade->getPixelShader(), nullptr, 0 );

	baseBuffer buf;
	buf.model = Matrix( SCALE, .1f, .1f, .1f )  * Rot * Matrix( TRANS, .5f, .5f, .5f );
	buf.proj = Matrix( IDENTITY );
	buf.view = Matrix( IDENTITY );

	ID3D11Buffer* cBuf = shade->getConstBuffer();
	Context->UpdateSubresource( cBuf, 0, nullptr, &buf, 0, 0 );
	Context->VSSetConstantBuffers(0, 1, &cBuf );

	Model* model = ModelManager::get( Model_ID::Cube_Model );
	Context->IASetVertexBuffers( 0, 1, model->getVertexBuffer(), model->getStride(), model->getOffset() ); 
	Context->IASetIndexBuffer( model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0 ); 

	Context->IASetInputLayout( shade->getVertexLayout() );
    Context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	Context->DrawIndexed( 36, 0, 0 ); 

	HRESULT res = GraphicsEngine::getSwapChain()->Present ( 0, 0 );
	assert( res == S_OK );
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
}
