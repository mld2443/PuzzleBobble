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


bool TriangleClass::Initialize(ID3D11Device* device)
{
	bool result;
	VertexType* vertices;
	unsigned long* indices;
	int vertexCount, indexCount;


	// Set the number of vertices in the vertex array.
	vertexCount = 3;

	// Set the number of indices in the index array.
	indexCount = 3;

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
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device, vertices, vertexCount, indices, indexCount);
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


void TriangleClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void TriangleClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}