////////////////////////////////////////////////////////////////////////////////
// Filename: resourcesclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")



//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d2d1_3.h>
#include <d2d1_1helper.h>
#include <dwrite_3.h>
#include <DirectXMath.h>

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: ResourcesClass
////////////////////////////////////////////////////////////////////////////////
class ResourcesClass
{
public:
	ResourcesClass();
	ResourcesClass(const ResourcesClass&);
	~ResourcesClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDirect3DDevice();
	ID2D1Device4* GetDirect2DDevice();
	ID3D11DeviceContext* GetDirect3DDeviceContext();
	ID2D1DeviceContext4* GetDirect2DDeviceContext();
	IDWriteFactory* GetDirectWriteFactory();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

private:
	bool InitializeDirect3D(int, int, bool, HWND, bool, float, float);
	bool InitializeDirect2D();
	bool InitializeDirectWrite();

	void ShutdownDirect3D();
	void ShutdownDirect2D();
	void ShutdownDirectWrite();

private:
	bool						m_vsync_enabled;
	int							m_videoCardMemory;
	char						m_videoCardDescription[128];
	IDXGISwapChain*				m_swapChain;

	// Direct3D
	ID3D11Device*				m_direct3DDevice;
	ID3D11DeviceContext*		m_direct3DDeviceContext;
	ID3D11RenderTargetView*		m_renderTargetView;
	ID3D11Texture2D*			m_depthStencilBuffer;
	ID3D11DepthStencilState*	m_depthStencilState;
	ID3D11DepthStencilView*		m_depthStencilView;
	ID3D11RasterizerState*		m_rasterState;

	// Direct2D
	ID2D1Device4*			m_direct2DDevice;
	ID2D1DeviceContext4*	m_direct2DDeviceContext;
	ID2D1Bitmap1*			m_bitmap;

	// DirectWrite
	IDWriteFactory*	m_directWriteFactory;

	XMMATRIX	m_projectionMatrix;
	XMMATRIX	m_worldMatrix;
	XMMATRIX	m_orthoMatrix;
};
