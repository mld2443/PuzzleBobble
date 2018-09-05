////////////////////////////////////////////////////////////////////////////////
// Filename: resourcesclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "resourcesclass.h"


ResourcesClass::ResourcesClass()
{
	m_swapChain = nullptr;
	m_direct3DDevice = nullptr;
	m_direct3DDeviceContext = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_rasterState = nullptr;

	m_direct2DDevice = nullptr;
	m_direct2DDeviceContext = nullptr;
	m_dxgiSurface = nullptr;
	m_bitmap = nullptr;

	m_directWriteFactory = nullptr;
}


ResourcesClass::ResourcesClass(const ResourcesClass& other)
{
}


ResourcesClass::~ResourcesClass()
{
}


bool ResourcesClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
	float screenDepth, float screenNear)
{
	bool result;
	

	// Initialize all direct3D resources.
	result = InitializeDirect3D(screenWidth, screenHeight, vsync, hwnd, fullscreen, screenDepth, screenNear);
	if (!result)
	{
		return false;
	}

	// Initialize all direct2D resources.
	result = InitializeDirect2D();
	if (!result)
	{
		return false;
	}

	// Initialize all directWrite resources.
	result = InitializeDirectWrite();
	if (!result)
	{
		return false;
	}

	return true;
}


void ResourcesClass::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	ShutdownDirectWrite();

	ShutdownDirect2D();

	ShutdownDirect3D();

	return;
}


void ResourcesClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_direct3DDeviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	m_direct3DDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void ResourcesClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}

	return;
}


ID3D11Device* ResourcesClass::GetDirect3DDevice()
{
	return m_direct3DDevice;
}


ID3D11DeviceContext* ResourcesClass::GetDirect3DDeviceContext()
{
	return m_direct3DDeviceContext;
}


ID2D1Device4* ResourcesClass::GetDirect2DDevice()
{
	return m_direct2DDevice;
}


ID2D1DeviceContext4*  ResourcesClass::GetDirect2DDeviceContext()
{
	return m_direct2DDeviceContext;
}


IDWriteFactory*  ResourcesClass::GetDirectWriteFactory()
{
	return m_directWriteFactory;
}


void ResourcesClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void ResourcesClass::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}


void ResourcesClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}


void ResourcesClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}


bool ResourcesClass::InitializeDirect3D(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
	float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	UINT creationFlags;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;


	// Store the vsync setting.
	m_vsync_enabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator =   displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = nullptr;

	// Release the adapter.
	adapter->Release();
	adapter = nullptr;

	// Release the factory.
	factory->Release();
	factory = nullptr;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width =  screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator =   numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator =   0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count =   1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// This flag adds support for surfaces with a different color channel ordering
	// than the API default. It is required for compatibility with Direct2D.
	creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	// Set the feature level to a version of DirectX 11.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_direct3DDevice, NULL, &m_direct3DDeviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = m_direct3DDevice->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = nullptr;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width =				 screenWidth;
	depthBufferDesc.Height =			 screenHeight;
	depthBufferDesc.MipLevels =			 1;
	depthBufferDesc.ArraySize =			 1;
	depthBufferDesc.Format =			 DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count =	 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage =				 D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags =			 D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags =	 0;
	depthBufferDesc.MiscFlags =			 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_direct3DDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable =	  true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc =	  D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable =	true;
	depthStencilDesc.StencilReadMask =	0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp =		D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp =		D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc =		D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp =	   D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp =	   D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc =		   D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_direct3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	m_direct3DDeviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format =			  DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension =	  D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_direct3DDevice->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_direct3DDeviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode =			   D3D11_CULL_BACK;
	rasterDesc.DepthBias =			   0;
	rasterDesc.DepthBiasClamp =		   0.0f;
	rasterDesc.DepthClipEnable =	   true;
	rasterDesc.FillMode =			   D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable =	   false;
	rasterDesc.ScissorEnable =		   false;
	rasterDesc.SlopeScaledDepthBias =  0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_direct3DDevice->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	m_direct3DDeviceContext->RSSetState(m_rasterState);

	// Setup the viewport for rendering.
	viewport.Width =	(float)screenWidth;
	viewport.Height =	(float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_direct3DDeviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}


bool ResourcesClass::InitializeDirect2D()
{
	HRESULT result;
	ID2D1Factory5* factory;
	D2D1_FACTORY_OPTIONS options;
	FLOAT dpiX, dpiY;
	IDXGIDevice *dxgiDevice;


	// Set the default options for the D2DFactory.
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
	// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	// Create D2DFactory to get our device.
	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &factory);
	if (FAILED(result))
	{
		return false;
	}

	// Retrieve a pointer to our device as a DXGI device.
	result = m_direct3DDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
	if (FAILED(result))
	{
		return false;
	}

	// Store a reference to our device.
	result = factory->CreateDevice(dxgiDevice, &m_direct2DDevice);
	if (FAILED(result))
	{
		return false;
	}

	// Create a device context to draw with.
	result = m_direct2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_direct2DDeviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// Get the back buffer from the swap chain to use as a DXGI Surface.
	result = m_swapChain->GetBuffer(0, __uuidof(m_dxgiSurface), (void**)&m_dxgiSurface);
	if (FAILED(result))
	{
		return false;
	}

	// Retrieve the device context DPI to match the text DPI with.
	factory->GetDesktopDpi(&dpiX, &dpiY);

	// Define Properties for our bitmap object.
	//FIXME: currently unused.
	D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, pixelFormat, dpiX, dpiY);

	// Create a bitmap to draw text on using the DXGI Surface backbuffer.
	result = m_direct2DDeviceContext->CreateBitmapFromDxgiSurface(m_dxgiSurface, nullptr, &m_bitmap);
	if (FAILED(result))
	{
		return false;
	}

	// Set our bitmap as the drawing target for our device context.
	m_direct2DDeviceContext->SetTarget(m_bitmap);

	// Release the factory.
	factory->Release();
	factory = nullptr;

	// Release the DXGI device.
	dxgiDevice->Release();
	dxgiDevice = nullptr;

	return true;
}


bool ResourcesClass::InitializeDirectWrite()
{
	bool result;

	// Create a DirectWrite Factory to create layouts and formats with.
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory4), (IUnknown **)&m_directWriteFactory);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void ResourcesClass::ShutdownDirect3D()
{
	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = nullptr;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_direct3DDeviceContext)
	{
		m_direct3DDeviceContext->Release();
		m_direct3DDeviceContext = nullptr;
	}

	if (m_direct3DDevice)
	{
		m_direct3DDevice->Release();
		m_direct3DDevice = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}

	return;
}


void ResourcesClass::ShutdownDirect2D()
{
	if (m_bitmap)
	{
		m_bitmap->Release();
		m_bitmap = nullptr;
	}

	if (m_dxgiSurface)
	{
		m_dxgiSurface->Release();
		m_dxgiSurface = nullptr;
	}

	if (m_direct2DDeviceContext)
	{
		m_direct2DDeviceContext->Release();
		m_direct2DDeviceContext = nullptr;
	}

	if (m_direct2DDevice)
	{
		m_direct2DDevice->Release();
		m_direct2DDevice = nullptr;
	}

	return;
}


void ResourcesClass::ShutdownDirectWrite()
{
	if (m_directWriteFactory)
	{
		m_directWriteFactory->Release();
		m_directWriteFactory = nullptr;
	}

	return;
}
