////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureshaderclass.h"


TextureShaderClass::TextureShaderClass() : ShaderInterface()
{
}


TextureShaderClass::TextureShaderClass(const TextureShaderClass& other) : ShaderInterface(other)
{

}


TextureShaderClass::~TextureShaderClass()
{
}


bool TextureShaderClass::Initialize(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
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

	polygonLayout[1].SemanticName =			"TEXCOORD";
	polygonLayout[1].SemanticIndex =		0;
	polygonLayout[1].Format =				DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot =			0;
	polygonLayout[1].AlignedByteOffset =	D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass =		D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate =	0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, g_texturevs, sizeof(g_texturevs), g_textureps, sizeof(g_textureps), polygonLayout, numElements);
	if (!result)
	{
		return false;
	}

	return true;
}


void TextureShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}


void TextureShaderClass::DrawShader(ID3D11DeviceContext* deviceContext, int indexCount, int)
{
	// Render the geometry.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
