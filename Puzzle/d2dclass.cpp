////////////////////////////////////////////////////////////////////////////////
// Filename: d2dclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "d2dclass.h"


D2DClass::D2DClass()
{
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_dwriteFactory = nullptr;
}


D2DClass::D2DClass(const D2DClass&)
{
}


D2DClass::~D2DClass()
{
}


bool D2DClass::Initialize(ID3D11Device* d3dDevice)
{
	HRESULT result;
	ID2D1Factory5* factory;
	IDXGIDevice *dxgiDevice;


	// Create D2DFactory to get our device.
	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (FAILED(result))
	{
		return false;
	}

	// Retrieve a pointer to our device as a DXGI device.
	result = d3dDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
	if (FAILED(result))
	{
		return false;
	}

	// Store a reference to our device.
	result = factory->CreateDevice(dxgiDevice, &m_device);
	if (FAILED(result))
	{
		return false;
	}

	// Release the factory.
	factory->Release();
	factory = nullptr;

	// Release the DXGI device.
	dxgiDevice->Release();
	dxgiDevice = nullptr;

	// Create a device context to draw with.
	result = m_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// Create a DirectWrite Factory to create layouts and formats with.
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory4), (IUnknown **)&m_dwriteFactory);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void D2DClass::Shutdown()
{
	if (m_dwriteFactory)
	{
		m_dwriteFactory->Release();
		m_dwriteFactory = nullptr;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	return;
}


ID2D1Device4* D2DClass::GetDevice()
{
	return m_device;
}


ID2D1DeviceContext4* D2DClass::GetDeviceContext()
{
	return m_deviceContext;
}


IDWriteFactory* D2DClass::GetDwriteFactory()
{
	return m_dwriteFactory;
}

