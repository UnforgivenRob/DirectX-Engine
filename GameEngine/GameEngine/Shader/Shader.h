#ifndef SHADER_H
#define SHADER_H
#include <d3d12.h>
#include <wrl/client.h>
#include "Node.h"

using Microsoft::WRL::ComPtr;

enum Shader_ID
{
	Base,
	Solid,
};

class Shader : Node
{
public:
	Shader( Shader_ID id, const char* inName, int cb_Size );
	~Shader();

	D3D12_SHADER_BYTECODE& getVS();
	D3D12_SHADER_BYTECODE& getPS();
	D3D12_INPUT_ELEMENT_DESC* getLayout();
	unsigned int getLayoutCnt();

	//ID3D11VertexShader* getVertexShader();
	//ID3D11PixelShader* getPixelShader();
	//ID3D11InputLayout* getVertexLayout();

	//ID3D11Buffer* getConstBuffer();

private:
	Shader(void);
	Shader& operator= ( const Shader& );
	Shader( const Shader& );

	void CompileVertexShader( char* vsName, ComPtr<ID3DBlob>& Vs );
	void CompilePixelShader( char* psName, ComPtr<ID3DBlob>& Ps );

	Shader_ID id;

	D3D12_SHADER_BYTECODE VS;
	D3D12_SHADER_BYTECODE PS;

	D3D12_INPUT_ELEMENT_DESC* layout;
	unsigned int layoutCnt;
	//ComPtr<ID3D12PipelineState> pipelineState;

	//ID3D11VertexShader* vs;
	//ID3D11PixelShader* ps;
	//ID3D11InputLayout* vertexLayout;
	//ID3D11Buffer* constBuff;

	//ID3D11Device* const Device;
	//ID3D11DeviceContext* const Context;
	ComPtr<ID3D12Device> Device;

	ComPtr<ID3DBlob> vs;
	ComPtr<ID3DBlob> ps;
};

#endif

