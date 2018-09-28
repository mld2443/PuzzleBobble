////////////////////////////////////////////////////////////////////////////////
// Filename: drawableinterface.cpp
////////////////////////////////////////////////////////////////////////////////
#include "drawableinterface.h"


DrawableInterface::DrawableInterface()
{
	m_Texture = nullptr;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_instanceBuffer = nullptr;
	m_instanceCount = 0;
}


DrawableInterface::DrawableInterface(const DrawableInterface&)
{
}


DrawableInterface::~DrawableInterface()
{
}


int DrawableInterface::GetVertexCount()
{
	return m_vertexCount;
}


int DrawableInterface::GetIndexCount()
{
	return m_indexCount;
}


int DrawableInterface::GetInstanceCount()
{
	return m_instanceCount;
}


ID3D11ShaderResourceView* DrawableInterface::GetTexture()
{
	return m_Texture->GetTexture();
}



bool DrawableInterface::isInstanced()
{
	return m_instanceCount > 0;
}


bool DrawableInterface::InitializeVertexBuffer(ID3D11Device* device, VertexType* vertices, int vertexCount)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	HRESULT result;


	// Set the number of vertices in the vertex array.
	m_vertexCount = vertexCount;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage =				D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth =			sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags =			D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags =		0;
	vertexBufferDesc.MiscFlags =			0;
	vertexBufferDesc.StructureByteStride =	0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem =			vertices;
	vertexData.SysMemPitch =		0;
	vertexData.SysMemSlicePitch =	0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


bool DrawableInterface::InitializeIndexBuffer(ID3D11Device* device, unsigned long* indices, int indexCount)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;


	// Set the number of indices in the index array.
	m_indexCount = indexCount;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage =					D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth =				sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags =				D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags =		0;
	indexBufferDesc.MiscFlags =				0;
	indexBufferDesc.StructureByteStride =	0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem =				indices;
	indexData.SysMemPitch =			0;
	indexData.SysMemSlicePitch =	0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


bool DrawableInterface::InitializeInstanceBuffer(ID3D11Device* device, InstanceType* instances, int instanceCount)
{
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;
	HRESULT result;


	// Set the number of instances in the array.
	m_instanceCount = instanceCount;

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage =					D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth =				sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags =				D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags =			D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags =				0;
	instanceBufferDesc.StructureByteStride =	0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem =			instances;
	instanceData.SysMemPitch =		0;
	instanceData.SysMemSlicePitch =	0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


bool DrawableInterface::UpdateInstanceBuffer(ID3D11DeviceContext* deviceContext, InstanceType* instances, int instanceCount)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	InstanceType* instancePtr;


	// Lock the vertex buffer.
	result = deviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	instancePtr = (InstanceType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(instancePtr, (void*)instances, (sizeof(InstanceType) * instanceCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_instanceBuffer, 0);

	return true;
}


void DrawableInterface::ShutdownBuffers()
{
	// Release the instance buffer.
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = nullptr;
	}

	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	return;
}


void DrawableInterface::RenderWithInstanceBuffer(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];


	// Set the buffer strides.
	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


void DrawableInterface::RenderWithoutInstanceBuffer(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool DrawableInterface::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}


void DrawableInterface::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}

	return;
}
