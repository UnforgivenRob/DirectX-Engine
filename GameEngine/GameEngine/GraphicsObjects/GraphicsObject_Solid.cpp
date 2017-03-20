#include "GraphicsObject_Solid.h"
#include "GraphicsEngine.h"
#include "Shader.h"
#include "ConstantBuffers.h"
#include "d3dx12.h"
GraphicsObject_Solid::GraphicsObject_Solid( Model_ID modelID, Material_ID material, Vect& color )
	: GraphicsObject( modelID, material ), color( color )
{
}

GraphicsObject_Solid::~GraphicsObject_Solid()
{
}

void GraphicsObject_Solid::draw( Matrix& proj_Mat )
{
	proj_Mat;
	Shader* shade = material->activate();
	shade;
	ComPtr<ID3D12GraphicsCommandList> commandList = GraphicsEngine::getCommandList();
	ComPtr<ID3D12CommandAllocator> commandAllocator = GraphicsEngine::getCurrentCommandAllocator();

	HRESULT res = commandAllocator->Reset();
	assert(res == S_OK);

	res = commandList->Reset(commandAllocator.Get(), material->getPSO().Get());
	assert(res == S_OK);

	commandList->SetGraphicsRootSignature(GraphicsEngine::getRootSignature().Get());
	commandList->RSSetViewports(1, GraphicsEngine::getViewport());
	commandList->RSSetScissorRects(1, GraphicsEngine::getScissorRect());

	CD3DX12_RESOURCE_BARRIER trans = CD3DX12_RESOURCE_BARRIER::Transition(GraphicsEngine::getCurrentRenderTarget().Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	commandList->ResourceBarrier(1, &trans);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(GraphicsEngine::getRTVHeap()->GetCPUDescriptorHandleForHeapStart(), GraphicsEngine::getCurrentFrameIndex(), GraphicsEngine::getRTVHeapSize());
	commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetIndexBuffer(model->getIndexBufferView());
	commandList->IASetVertexBuffers(0, 1, model->getVertexBufferView());
	commandList->DrawIndexedInstanced(model->getNumIndices(), 1, 0, 0, 0);
	
	trans = CD3DX12_RESOURCE_BARRIER::Transition(GraphicsEngine::getCurrentRenderTarget().Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	commandList->ResourceBarrier(1, &trans);

	res = commandList->Close();
	assert(res == S_OK);
	/*solidBuffer buf;
	buf.model = model_Mat;
	buf.proj = proj_Mat;
	buf.view = view_Mat;
	buf.color = color;

	ID3D11Buffer* cBuf = shade->getConstBuffer();
	Context->UpdateSubresource( cBuf, 0, nullptr, &buf, 0, 0 );
	Context->VSSetConstantBuffers(0, 1, &cBuf );*/

	//Context->IASetVertexBuffers( 0, 1, model->getVertexBuffer(), model->getStride(), model->getOffset() ); 
	//Context->IASetIndexBuffer( model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0 ); 

	//Context->IASetInputLayout( shade->getVertexLayout() );
 //   Context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	//Context->DrawIndexed( model->getNumIndices(), 0, 0 ); 
}