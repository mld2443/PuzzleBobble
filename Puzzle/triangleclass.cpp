////////////////////////////////////////////////////////////////////////////////
// Filename: triangleclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "triangleclass.h"


TriangleClass::TriangleClass() : DrawableInterface()
{
}


TriangleClass::TriangleClass(const TriangleClass& other) : DrawableInterface(other)
{
}


TriangleClass::~TriangleClass()
{
}


bool TriangleClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;
	VertexType* vertices;
	unsigned long* indices;
	int vertexCount, indexCount;


	// Set the number of vertices, indices, and instances in the arrays.
	vertexCount = 7;
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
	vertices[0].position =	XMFLOAT3(-1.0f, -1.0f, 0.0f);	// Left.
	vertices[0].color =		XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);

	vertices[1].position =	XMFLOAT3(0.0f, 1.0f, 0.0f);		// Top.
	vertices[1].color =		XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);

	vertices[2].position =	XMFLOAT3(1.0f, -1.0f, 0.0f);	// Right.
	vertices[2].color =		XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Left.
	indices[1] = 1;  // Top.
	indices[2] = 2;  // Right.

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

	return true;
}


void TriangleClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void TriangleClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderWithoutInstanceBuffer(deviceContext);

	return;
}
