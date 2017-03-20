#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H
#include <d3d12.h>
#include <dxgi1_5.h>
#include <wrl/client.h>
#include "MathEngine.h"

using Microsoft::WRL::ComPtr;

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

class GraphicsEngine
{
public:
	static void Activate( HINSTANCE mApp, const char* caption, bool bEnable4xMsaa, int clientWindth, int clientHeight );
	static void OnResize();
	static void setViewPort( const int posX, const int posY, const int width, const int height );
	static void Deactivate();
	static void BufferSwap( Vect& bgColor );
	static ComPtr<ID3D12Device> getDevice();
	static ComPtr<IDXGISwapChain3> getSwapChain();
	static ComPtr<ID3D12RootSignature> getRootSignature();
	static ComPtr<ID3D12GraphicsCommandList> getCommandList();
	static ComPtr<ID3D12CommandQueue> getCommandQueue();
	static ComPtr<ID3D12CommandAllocator> getCurrentCommandAllocator();
	static ComPtr<ID3D12Resource> getCurrentRenderTarget();
	static D3D12_VIEWPORT* getViewport();
	static D3D12_RECT* getScissorRect();
	static UINT getCurrentFrameIndex();
	static ComPtr<ID3D12DescriptorHeap> getRTVHeap();
	static UINT getRTVHeapSize();

private:
	GraphicsEngine();
	~GraphicsEngine();

	static GraphicsEngine engine;

	static const UINT FrameCount = 2;
	
	HWND	  mMainWnd;
	UINT	  msaaQuality;

	D3D12_VIEWPORT Viewport;
	D3D12_RECT scissorRect;

	ComPtr<IDXGISwapChain3> swapChain;
	ComPtr<ID3D12Device> Device;
	ComPtr<ID3D12Resource> renderTargets[FrameCount];
	ComPtr<ID3D12Resource> depthStencil;
	ComPtr<ID3D12CommandAllocator> commandAllocator[FrameCount];
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap;
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	UINT descriptorSize;

	ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	UINT frameIndex;
	HANDLE fenceEvent;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceValues[FrameCount];

	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;
};

#endif