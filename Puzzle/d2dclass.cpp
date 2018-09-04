////////////////////////////////////////////////////////////////////////////////
// Filename: d2dclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "d2dclass.h"


D2DClass::D2DClass()
{
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
	d3dDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);

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

	return true;
}


void D2DClass::Shutdown()
{
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