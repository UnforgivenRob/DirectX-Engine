#include "GraphicsObject_Solid.h"
#include "GraphicsEngine.h"
#include "Shader.h"
#include "ConstantBuffers.h"

GraphicsObject_Solid::GraphicsObject_Solid( Model_ID modelID, Material_ID material, Vect& color )
	: GraphicsObject( modelID, material ), color( color )
{
}

GraphicsObject_Solid::~GraphicsObject_Solid()
{
}

void GraphicsObject_Solid::draw( Matrix& proj_Mat )
{
	Shader* shade = material->activate();
	ComPtr<ID3D12CommandList> commandList = GraphicsEngine::getCommandList();
	Context->VSSetShader( shade->getVertexShader(), nullptr, 0 );
	Context->PSSetShader( shade->getPixelShader(), nullptr, 0 );

	solidBuffer buf;
	buf.model = model_Mat;
	buf.proj = proj_Mat;
	buf.view = view_Mat;
	buf.color = color;

	ID3D11Buffer* cBuf = shade->getConstBuffer();
	Context->UpdateSubresource( cBuf, 0, nullptr, &buf, 0, 0 );
	Context->VSSetConstantBuffers(0, 1, &cBuf );

	Context->IASetVertexBuffers( 0, 1, model->getVertexBuffer(), model->getStride(), model->getOffset() ); 
	Context->IASetIndexBuffer( model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0 ); 

	Context->IASetInputLayout( shade->getVertexLayout() );
    Context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	Context->DrawIndexed( model->getNumIndices(), 0, 0 ); 
}