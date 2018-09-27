////////////////////////////////////////////////////////////////////////////////
// Filename: quadclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "quadclass.h"


QuadClass::QuadClass() : DrawableInterface()
{
}


QuadClass::QuadClass(const QuadClass& other) : DrawableInterface(other)
{
}


QuadClass::~QuadClass()
{
}


bool QuadClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;
	VertexType* vertices;
	unsigned long* indices;
	int vertexCount, indexCount;


	// Set the number of vertices, indices, and instances in the arrays.
	vertexCount = 4;
	indexCount = 6;

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
	vertices[0].position =	XMFLOAT3(-10.0f, 10.0f, 0.1f);	// Top left.
	vertices[0].tex =		XMFLOAT2(0.0f, 0.0f);

	vertices[1].position =	XMFLOAT3(10.0f, 10.0f, 0.1f);	// Top right.
	vertices[1].tex =		XMFLOAT2(1.0f, 0.0f);

	vertices[2].position =	XMFLOAT3(10.0f, -10.0f, 0.1f);	// Bottom right.
	vertices[2].tex =		XMFLOAT2(1.0f, 1.0f);

	vertices[3].position =	XMFLOAT3(-10.0f, -10.0f, 0.1f);	// Bottom left.
	vertices[3].tex =		XMFLOAT2(0.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Top left.
	indices[1] = 1;  // Top right.
	indices[2] = 2;  // Bottom right.

	indices[3] = 0;  // Top left.
	indices[4] = 2;  // Bottom right.
	indices[5] = 3;  // Bottom left.

	// Initialize the vertex buffer.
	result = InitializeVertexBuffer(device, vertices, vertexCount);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeIndexBuffer(device, indices, indexCount);
	if (!result)
	{
		return false;
	}


	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	delete[] indices;

	vertices = nullptr;
	indices = nullptr;

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, BACKGROUNDPATH);
	if (!result)
	{
		return false;
	}

	return true;
}


void QuadClass::Shutdown()
{
	// Release our texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void QuadClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderWithoutInstanceBuffer(deviceContext);

	return;
}
