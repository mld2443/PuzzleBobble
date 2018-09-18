////////////////////////////////////////////////////////////////////////////////
// Filename: boardclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "boardclass.h"



BoardClass::BoardClass() : DrawableInterface()
{
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
	vertexCount = 6;

	// Set the number of indices in the index array.
	indexCount = 12;

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
	vertices[0].position = XMFLOAT3(-0.5f, SQRT075, 0.0f);	// Top left.
	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.5f, SQRT075, 0.0f);	// Top right.
	vertices[1].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, 0.0f, 0.0f);		// Right.
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(0.5f, -SQRT075, 0.0f);	// Bottom right.
	vertices[3].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);

	vertices[4].position = XMFLOAT3(-0.5f, -SQRT075, 0.0f);	// Bottom left.
	vertices[4].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[5].position = XMFLOAT3(-1.0f, 0.0f, 0.0f);		// Left.
	vertices[5].color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 5;  // Left.
	indices[1] = 0;  // Top left.
	indices[2] = 4;  // Bottom left.

	indices[3] = 4;  // Bottom left.
	indices[4] = 0;  // Top left.
	indices[5] = 3;  // Bottom right.

	indices[6] = 0;  // Top left.
	indices[7] = 1;  // Top right.
	indices[8] = 3;  // Bottom right.

	indices[9] = 1;  // Top right.
	indices[10] = 2;  // Right.
	indices[11] = 3;  // Bottom right.

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


void BoardClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void BoardClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}
