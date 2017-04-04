#include "GraphicsObject_Base.h"
#include "GraphicsEngine.h"
#include "Shader.h"
#include "ConstantBuffers.h"
#include "d3dx12.h"

GraphicsObject_Base::GraphicsObject_Base( Model_ID modelID, Material_ID material )
	: GraphicsObject( modelID, material )
{
}

GraphicsObject_Base::~GraphicsObject_Base()
{
}

void GraphicsObject_Base::draw( Matrix& proj_Mat )
{
	proj_Mat;
	Shader* shade = material->activate();
	
	ComPtr<ID3D12GraphicsCommandList> commandList = GraphicsEngine::getCommandList();
	
	BaseBuffer* buf = shade->getBuffer();
	buf->model = model_Mat;
	buf->proj = proj_Mat;
	buf->view = view_Mat;

	UINT8* cBuf = shade->getCBStart();
	memcpy(cBuf, &buf, sizeof(BaseBuffer));

	commandList->SetPipelineState(material->getPSO().Get());

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetIndexBuffer(model->getIndexBufferView());
	commandList->IASetVertexBuffers(0, 1, model->getVertexBufferView());
	commandList->DrawIndexedInstanced(model->getNumIndices(), 1, 0, 0, 0);

	//Shader* shade = material->activate();
	//ID3D11DeviceContext* Context = GraphicsEngine::getContext();
	//Context->VSSetShader( shade->getVertexShader(), nullptr, 0 );
	//Context->PSSetShader( shade->getPixelShader(), nullptr, 0 );


	//Context->UpdateSubresource( cBuf, 0, nullptr, &buf, 0, 0 );
	//Context->VSSetConstantBuffers(0, 1, &cBuf );

	//Context->IASetVertexBuffers( 0, 1, model->getVertexBuffer(), model->getStride(), model->getOffset() ); 
	//Context->IASetIndexBuffer( model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0 ); 

	//Context->IASetInputLayout( shade->getVertexLayout() );
 //   Context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	//Context->DrawIndexed( model->getNumIndices(), 0, 0 );*/ 
}