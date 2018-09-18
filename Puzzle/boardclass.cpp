////////////////////////////////////////////////////////////////////////////////
// Filename: boardclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "boardclass.h"



BoardClass::BoardClass() : DrawableInterface()
{
	m_instanceBuffer = nullptr;
}


BoardClass::BoardClass(const BoardClass& other) : DrawableInterface(other)
{
}


BoardClass::~BoardClass()
{
}


bool BoardClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;
	VertexType* vertices;
	unsigned long* indices;
	int vertexCount, indexCount;


	// Set the number of vertices in the vertex array.
	vertexCount = 7;

	// Set the number of indices in the index array.
	indexCount = 18;

	// Create the vertex array.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);	// Center.
	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.5f, SQRT075, 0.0f);	// Top right.
	vertices[1].color = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, 0.0f, 0.0f);		// Right.
	vertices[2].color = XMFLOAT4(0.6f, 0.0f, 0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(0.5f, -SQRT075, 0.0f);	// Bottom right.
	vertices[3].color = XMFLOAT4(0.4f, 0.0f, 0.0f, 1.0f);

	vertices[4].position = XMFLOAT3(-0.5f, -SQRT075, 0.0f);	// Bottom left.
	vertices[4].color = XMFLOAT4(0.2f, 0.0f, 0.0f, 1.0f);

	vertices[5].position = XMFLOAT3(-1.0f, 0.0f, 0.0f);		// Left.
	vertices[5].color = XMFLOAT4(0.4f, 0.0f, 0.0f, 1.0f);

	vertices[6].position = XMFLOAT3(-0.5f, SQRT075, 0.0f);	// Top left.
	vertices[6].color = XMFLOAT4(0.6f, 0.0f, 0.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Center.
	indices[1] = 1;  // Top Right.
	indices[2] = 2;  // Right.

	indices[3] = 0;  // Center.
	indices[4] = 2;  // Right.
	indices[5] = 3;  // Bottom right.

	indices[6] = 0;  // Center.
	indices[7] = 3;  // Bottom right.
	indices[8] = 4;  // Bottom left.

	indices[9] = 0;  // Center.
	indices[10] = 4;  // Bottom left.
	indices[11] = 5;  // Left.

	indices[12] = 0;  // Center.
	indices[13] = 5;  // Left.
	indices[14] = 6;  // Top left.

	indices[15] = 0;  // Center.
	indices[16] = 6;  // Top left.
	indices[17] = 1;  // Top right.

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device, vertices, vertexCount, indices, indexCount);
	if (!result)
	{
		return false;
	}

	// Initialize the instance buffer.
	result = InitializeInstanceBuffer(device);
	if (!result)
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}


void BoardClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Shutdown the instance buffer.
	ShutdownInstanceBuffer();

	return;
}


void BoardClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderAllBuffers(deviceContext);

	return;
}


int BoardClass::GetInstanceCount()
{
	return m_instanceCount;
}


bool BoardClass::InitializeInstanceBuffer(ID3D11Device* device)
{
	InstanceType* instances;
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;
	HRESULT result;


	// Set the number of instances in the array.
	m_instanceCount = 4;

	// Create the instance array.
	instances = new InstanceType[m_instanceCount];
	if (!instances)
	{
		return false;
	}

	// Load the instance array with data.
	instances[0].position = XMFLOAT3(-1.5f, -1.5f, 5.0f);
	instances[1].position = XMFLOAT3(-1.5f, 1.5f, 5.0f);
	instances[2].position = XMFLOAT3(1.5f, -1.5f, 5.0f);
	instances[3].position = XMFLOAT3(1.5f, 1.5f, 5.0f);

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the instance array now that the instance buffer has been created and loaded.
	delete[] instances;
	instances = 0;

	return true;
}


void BoardClass::ShutdownInstanceBuffer()
{
	// Release the instance buffer.
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = nullptr;
	}

	return;
}


void BoardClass::RenderAllBuffers(ID3D11DeviceContext* deviceContext)
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
	bufferPointers[0] = GetVertexBuffer();
	bufferPointers[1] = m_instanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}