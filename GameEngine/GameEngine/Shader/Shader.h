#ifndef SHADER_H
#define SHADER_H
#include <d3d11.h>
#include "Node.h"

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

	ID3D11VertexShader* getVertexShader();
	ID3D11PixelShader* getPixelShader();
	ID3D11InputLayout* getVertexLayout();

	ID3D11Buffer* getConstBuffer();

private:
	Shader(void);
	Shader& operator= ( const Shader& );
	Shader( const Shader& );

	void CompileVertexShader( char* vsName );
	void CompilePixelShader( char* psName );

	Shader_ID id;

	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D11InputLayout* vertexLayout;
	ID3D11Buffer* constBuff;

	ID3D11Device* const Device;
	ID3D11DeviceContext* const Context;
};

#endif

