////////////////////////////////////////////////////////////////////////////////
// Filename: instanceshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "instanceshaderclass.h"


InstanceShaderClass::InstanceShaderClass() : ShaderInterface()
{
}


InstanceShaderClass::InstanceShaderClass(const InstanceShaderClass& other) : ShaderInterface(other)
{
}


InstanceShaderClass::~InstanceShaderClass()
{
}


bool InstanceShaderClass::Initialize(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements;
	bool result;


	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName =			"POSITION";
	polygonLayout[0].SemanticIndex =		0;
	polygonLayout[0].Format =				DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot =			0;
	polygonLayout[0].AlignedByteOffset =	0;
	polygonLayout[0].InputSlotClass =		D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate =	0;

	polygonLayout[1].SemanticName =			"COLOR";
	polygonLayout[1].SemanticIndex =		0;
	polygonLayout[1].Format =				DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot =			0;
	polygonLayout[1].AlignedByteOffset =	D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass =		D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate =	0;

	polygonLayout[2].SemanticName =			"POSITION";
	polygonLayout[2].SemanticIndex =		1;
	polygonLayout[2].Format =				DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot =			1;
	polygonLayout[2].AlignedByteOffset =	0;
	polygonLayout[2].InputSlotClass =		D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[2].InstanceDataStepRate =	1;

	polygonLayout[3].SemanticName =			"COLOR";
	polygonLayout[3].SemanticIndex =		1;
	polygonLayout[3].Format =				DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[3].InputSlot =			1;
	polygonLayout[3].AlignedByteOffset =	D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass =		D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[3].InstanceDataStepRate =	1;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, g_instancevs, sizeof(g_instancevs), g_instanceps, sizeof(g_instanceps), polygonLayout, numElements);
	if (!result)
	{
		return false;
	}

	return true;
}


void InstanceShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}


void InstanceShaderClass::DrawShader(ID3D11DeviceContext* deviceContext, int indexCountPerInstance, int instanceCount)
{
	// Render the geometry.
	deviceContext->DrawIndexedInstanced(indexCountPerInstance, instanceCount, 0, 0, 0);

	return;
}
