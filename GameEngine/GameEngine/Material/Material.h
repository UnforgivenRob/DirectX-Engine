#ifndef MATERIAL_H
#define MATERIAL_H
#include "Node.h"
#include <d3d12.h>
#include "wrl/client.h"

using Microsoft::WRL::ComPtr;

class Game;
class Shader;

enum Material_ID
{
	Base_Solid,
	Base_Wire,
	Solid_Solid,
	Solid_Wire,
};

class Material : public Node
{
public:
	Material( Material_ID id, Shader* inShader );
	~Material(void);

	virtual Shader* activate();

	ComPtr<ID3D12PipelineState> getPSO();

protected:
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12Device> device;
	Shader* shader;
	Material_ID id;

private:
	Material();
};

#endif

