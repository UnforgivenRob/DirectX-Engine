#include "GraphicsObject.h"
#include "MaterialManager.h"
#include "ModelManager.h"
#include "Shader.h"
#include "ConstantBuffers.h"
#include "GraphicsEngine.h"

GraphicsObject::GraphicsObject( Model_ID modelID, Material_ID material )
	: model_Mat( IDENTITY ), view_Mat( IDENTITY )
{
	this->material = MaterialManager::get(material);
	this->model = ModelManager::get(modelID);
}

GraphicsObject::~GraphicsObject()
{
}

void GraphicsObject::update( Matrix& inModel_Mat, Matrix& inView_Mat )
{
	this->model_Mat = inModel_Mat;
	this->view_Mat = inView_Mat;
}

void GraphicsObject::draw( Matrix& proj_Mat )
{
	Shader* shade = material->activate();
	ID3D11DeviceContext* Context = GraphicsEngine::getContext();
	Context->VSSetShader( shade->getVertexShader(), nullptr, 0 );
	Context->PSSetShader( shade->getPixelShader(), nullptr, 0 );

	baseBuffer buf;
	buf.model = model_Mat;
	buf.proj = proj_Mat;
	buf.view = view_Mat;

	ID3D11Buffer* cBuf = shade->getConstBuffer();
	Context->UpdateSubresource( cBuf, 0, nullptr, &buf, 0, 0 );
	Context->VSSetConstantBuffers(0, 1, &cBuf );

	Context->IASetVertexBuffers( 0, 1, model->getVertexBuffer(), model->getStride(), model->getOffset() ); 
	Context->IASetIndexBuffer( model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0 ); 

	Context->IASetInputLayout( shade->getVertexLayout() );
    Context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	Context->DrawIndexed( 36, 0, 0 ); 

	HRESULT res = GraphicsEngine::getSwapChain()->Present ( 0, 0 );
	assert( res == S_OK );
}

Model* GraphicsObject::getModel()
{
	return this->model;
}

void GraphicsObject::setModel( Model_ID modID )
{
	this->model = ModelManager::get( modID );
}

Material* GraphicsObject::getMaterial()
{
	return this->material;
}

Matrix& GraphicsObject::getModelView()
{
	return this->model_Mat;
}