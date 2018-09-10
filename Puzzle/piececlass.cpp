////////////////////////////////////////////////////////////////////////////////
// Filename: piececlass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "piececlass.h"



PieceClass::PieceClass() : DrawableInterface()
{
	m_Texture = nullptr;
}


PieceClass::PieceClass(const PieceClass& other) : DrawableInterface(other)
{
}


PieceClass::~PieceClass()
{
}


bool PieceClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;
	VertexType* vertices;
	unsigned long* indices;
	int vertexCount, indexCount;


	// Load the texture for our piece.
	result = LoadTexture(device, deviceContext, "../Puzzle/data/piece.tga");
	if (!result)
	{
		return false;
	}

	// Set the number of vertices in the vertex array.
	vertexCount = 4;

	// Set the number of indices in the index array.
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
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);	// Bottom left.
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);		// Top left.
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);		// Bottom right.
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[3].position = XMFLOAT3(1.0f, 1.0f, 0.0f);		// Top right.
	vertices[3].texture = XMFLOAT2(1.0f, 0.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top left.
	indices[2] = 2;  // Bottom right.

	indices[3] = 2;  // Bottom right.
	indices[4] = 1;  // Top left.
	indices[5] = 3;  // Top right.

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


void PieceClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}

	return;
}


void PieceClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


ID3D11ShaderResourceView* PieceClass::GetTexture()
{
	return m_Texture->GetTexture();
}



bool PieceClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
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
