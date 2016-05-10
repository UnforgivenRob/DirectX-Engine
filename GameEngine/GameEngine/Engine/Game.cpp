#include "Game.h"
#include "Time.h"
#include <stdio.h>
#include <assert.h>
#include "Shader.h"
#include "Model.h"

Shader* shade = 0;
Model* model = 0;

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
}

void Game::LoadContent()
{
	//D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	//};

	// Create the input layout
	/*D3d11_
    D3DX11_PASS_DESC passDesc;
    mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	Device->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, 
	passDesc.IAInputSignatureSize, &mInputLayout));*/
	shade = new Shader( Shader_ID::Base, "Base", this );
	model = new Model( this );

}

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
}

void Game::Draw()
{
	Context->VSSetShader( shade->getVertexShader(), nullptr, 0 );
	Context->PSSetShader( shade->getPixelShader(), nullptr, 0 );

	Context->IASetVertexBuffers( 0, 1, model->getVertexBuffer(), model->getStride(), model->getOffset() ); 
	Context->IASetIndexBuffer( model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0 ); 

	Context->IASetInputLayout(shade->getVertexLayout());
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Context->DrawIndexed( 3, 0, 0 ); 

	HRESULT res = SwapChain->Present(0, 0);
	assert( res == S_OK );
}

void Game::ClearBuffers()
{
	assert( Context );
	assert( SwapChain );

	Context->ClearRenderTargetView(RenderTargetView, &bgColor[x] );
	Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	

}

void Game::UnloadContent()
{
	delete shade;
	delete model;
}
