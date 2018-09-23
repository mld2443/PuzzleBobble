////////////////////////////////////////////////////////////////////////////////
// Filename: shaderinterface.cpp
////////////////////////////////////////////////////////////////////////////////
#include "shaderinterface.h"


ShaderInterface::ShaderInterface()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_matrixBuffer = nullptr;
}


ShaderInterface::ShaderInterface(const ShaderInterface& other)
{
}


ShaderInterface::~ShaderInterface()
{
}


bool ShaderInterface::Render(ID3D11DeviceContext* deviceContext, int indexCountPerInstance, int instanceCount, 
							 XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

	// Now draw the prepared buffers with the shader.
	DrawShader(deviceContext, indexCountPerInstance, instanceCount);

	return true;
}


bool ShaderInterface::InitializeShader(ID3D11Device* device,
									   const BYTE* vertexByteCode, SIZE_T vertexCodeSize,
									   const BYTE* pixelByteCode, SIZE_T pixelCodeSize,
									   D3D11_INPUT_ELEMENT_DESC* polygonLayout, unsigned int numElements)
{
	HRESULT result;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// Create the vertex shader from the header file provided by compiling VertexShader.hlsl.
	result = device->CreateVertexShader(vertexByteCode, vertexCodeSize, nullptr, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the header file provided by compiling PixelShader.hlsl.
	result = device->CreatePixelShader(pixelByteCode, pixelCodeSize, nullptr, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexByteCode, vertexCodeSize, &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage =				D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth =			sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags =			D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags =		D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags =			0;
	matrixBufferDesc.StructureByteStride =	0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void ShaderInterface::ShutdownShader()
{
	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}

	return;
}


bool ShaderInterface::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;


	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finally set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}
