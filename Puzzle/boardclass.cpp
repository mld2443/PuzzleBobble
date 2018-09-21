////////////////////////////////////////////////////////////////////////////////
// Filename: boardclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "boardclass.h"


BoardClass::BoardClass() : DrawableInterface()
{
	m_boardState = nullptr;
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
	InstanceType* instances;
	int vertexCount, indexCount, instanceCount;


	// Create the board state object.
	m_boardState = new BoardStateClass;
	if (!m_boardState)
	{
		return false;
	}

	// Load level in from file and get number of piece containers.
	result = LoadLevel("../Puzzle/data/level.txt");
	if (!result)
	{
		return false;
	}

	// Set the number of vertices, indices, and instances in the arrays.
	vertexCount = 7;
	indexCount = 18;
	instanceCount = m_boardState->size();

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

	// Create the instance array.
	instances = new InstanceType[instanceCount];
	if (!instances)
	{
		return false;
	}

	// Create piece geometry.
	CreateGeometry(vertices, indices);

	// Load the instance array with data.
	instances[0].position =	XMFLOAT3(-1.5f, -1.5f, 5.0f);
	instances[0].color =	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	instances[1].position =	XMFLOAT3(-1.5f, 1.5f, 5.0f);
	instances[1].color =	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	instances[2].position =	XMFLOAT3(1.5f, -1.5f, 5.0f);
	instances[2].color =	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	instances[3].position =	XMFLOAT3(1.5f, 1.5f, 5.0f);
	instances[3].color =	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

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

	// Initialize the instance buffer.
	result = InitializeInstanceBuffer(device, instances, instanceCount);
	if (!result)
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	delete[] indices;
	delete[] instances;

	vertices = nullptr;
	indices = nullptr;
	instances = nullptr;

	return true;
}


void BoardClass::Shutdown()
{
	// Clear and shutdown the level.
	if (m_boardState)
	{
		m_boardState->Shutdown();
		delete m_boardState;
		m_boardState = nullptr;
	}

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void BoardClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderWithInstanceBuffer(deviceContext);

	return;
}


bool BoardClass::LoadLevel(char* filename)
{
	unsigned int colorCount;
	char colorKey;
	XMFLOAT4 colorValues;
	std::ifstream fileReader;
	std::string line;


	// Open file for reading.
	fileReader = std::ifstream(filename);
	if (!fileReader)
	{
		return false;
	}

	// Read in the first word of the line, which should state the number of colors.
	// NOTE: This could be more safely defined. 
	fileReader >> line >> colorCount;
	if (line.compare("colors") != 0)
	{
		return false;
	}

	// Read in color keys and RGBA values, then store in color map.
	for (unsigned int i = 0; i < colorCount; i++)
	{
		fileReader >> colorKey >> colorValues.x >> colorValues.y >> colorValues.z >> colorValues.w;

		m_colors[colorKey] = colorValues;
	}

	// Add a blank color to color map for empty spaces.
	m_colors['_'] = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	// The board state will use the rest of the file to load in the starting state.
	m_boardState->Initialize(fileReader);

	// Close our file now that we're done with it.
	fileReader.close();

	return true;
}


void BoardClass::CreateGeometry(VertexType* vertices, unsigned long* indices)
{
	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);			// Center.
	vertices[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);			// Top.
	vertices[1].color = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);

	vertices[2].position = XMFLOAT3(SQRT075, 0.5f, 0.0f);		// Top Right.
	vertices[2].color = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);

	vertices[3].position = XMFLOAT3(SQRT075, -0.5f, 0.0f);		// Bottom right.
	vertices[3].color = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);

	vertices[4].position = XMFLOAT3(0.0f, -1.0f, 0.0f);		// Bottom.
	vertices[4].color = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);

	vertices[5].position = XMFLOAT3(-SQRT075, -0.5f, 0.0f);	// Bottom left.
	vertices[5].color = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	vertices[6].position = XMFLOAT3(-SQRT075, 0.5f, 0.0f);		// Top left.
	vertices[6].color = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Center.
	indices[1] = 1;  // Top.
	indices[2] = 2;  // Top right.

	indices[3] = 0;  // Center.
	indices[4] = 2;  // Top right.
	indices[5] = 3;  // Bottom right.

	indices[6] = 0;  // Center.
	indices[7] = 3;  // Bottom right.
	indices[8] = 4;  // Bottom.

	indices[9] = 0;  // Center.
	indices[10] = 4;  // Bottom.
	indices[11] = 5;  // Bottom left.

	indices[12] = 0;  // Center.
	indices[13] = 5;  // Bottom left.
	indices[14] = 6;  // Top left.

	indices[15] = 0;  // Center.
	indices[16] = 6;  // Top left.
	indices[17] = 1;  // Top.
}
