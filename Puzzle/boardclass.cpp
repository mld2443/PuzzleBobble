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
	std::vector<VertexType> vertices;
	std::vector<unsigned long> indices;
	std::vector<InstanceType> instances;


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

	// Create piece geometry.
	CreateGeometry(vertices, indices);

	// Load the instance array with data.
	LoadInstances(instances);

	// Initialize the vertex buffer.
	result = InitializeVertexBuffer(device, vertices.data(), vertices.size());
	if (!result)
	{
		return false;
	}

	// Initialize the index buffer.
	result = InitializeIndexBuffer(device, indices.data(), indices.size());
	if (!result)
	{
		return false;
	}

	// Initialize the instance buffer.
	result = InitializeInstanceBuffer(device, instances.data(), instances.size());
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, "../Puzzle/data/piece.tga");
	if (!result)
	{
		return false;
	}

	return true;
}


void BoardClass::Shutdown()
{
	// Release our texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Clear and shutdown the level.
	if (m_boardState)
	{
		m_boardState->Shutdown();
		delete m_boardState;
		m_boardState = nullptr;
	}

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
	XMFLOAT3 colorValues;
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
		fileReader >> colorKey >> colorValues.x >> colorValues.y >> colorValues.z;

		m_colors[colorKey] = colorValues;
	}

	// The board state will use the rest of the file to load in the starting state.
	m_boardState->Initialize(fileReader);

	// Close our file now that we're done with it.
	fileReader.close();

	return true;
}




void BoardClass::LoadInstances(std::vector<InstanceType>& instances)
{
	float boardWidth, boardHeight, positionX, positionY, stepX, stepY;
	BoardStateClass::SpaceType *traverseDown, *traverseRight;
	InstanceType tempInstance;


	// Set the stepping distance.
	stepX = 2.1f;
	stepY = 0.5f * stepX * SQRT3;

	// Calculate the width between centers of the farthest apart spaces.
	boardWidth = (float)(m_boardState->GetMaxWidth() - 1) * stepX;
	boardHeight = (float)(m_boardState->GetHeight() - 1) * stepY;

	// Offset the starting position so the whole board appears centered.
	positionX = -0.5f * boardWidth;
	positionY = 0.5f * boardHeight;

	// Start the traversal of our board.
	traverseDown = m_boardState->GetTopLeft();

	while (traverseDown)
	{
		// Set a row traversal pointer to start at the left side of the board for the given row.
		traverseRight = traverseDown;

		while (traverseRight)
		{
			if (traverseRight->color != '_')
			{
				tempInstance.position =	XMFLOAT3(positionX, positionY, 0.0f);
				tempInstance.HSV =		m_colors[traverseRight->color];
				instances.push_back(tempInstance);
			}

			// Move our Space pointer right one space.
			traverseRight = traverseRight->rightNeighbor;

			// Move our prospective position coordinates right one space.
			positionX += stepX;
		}

		// Move our prospective position coordinates down to the next row.
		positionX -= stepX * ((float)m_boardState->GetMaxWidth() - 0.5f);
		positionY -= stepY;

		// Move traverseDown down a row, either to the left if moving from an even row to an odd row, or right for the opposite.
		if (traverseDown->lowerLeftNeighbor)
		{
			traverseDown = traverseDown->lowerLeftNeighbor;
		}
		else
		{
			traverseDown = traverseDown->lowerRightNeighbor;
		}
	}

	return;
}


void BoardClass::CreateGeometry(std::vector<VertexType>& vertices, std::vector<unsigned long>& indices)
{
	VertexType vertex;


	// Load the vertex array with data.
	vertex.position =	XMFLOAT3(-1.0f, 1.0f, 0.0f);	// Top left.
	vertex.tex =		XMFLOAT2(0.0f, 0.0f);
	vertices.push_back(vertex);

	vertex.position =	XMFLOAT3(1.0f, 1.0f, 0.0f);		// Top right.
	vertex.tex =		XMFLOAT2(1.0f, 0.0f);
	vertices.push_back(vertex);

	vertex.position =	XMFLOAT3(1.0f, -1.0f, 0.0f);	// Bottom right.
	vertex.tex =		XMFLOAT2(1.0f, 1.0f);
	vertices.push_back(vertex);

	vertex.position =	XMFLOAT3(-1.0f, -1.0f, 0.0f);	// Bottom left.
	vertex.tex =		XMFLOAT2(0.0f, 1.0f);
	vertices.push_back(vertex);

	// Load the index array with data.
	indices.push_back(0);	// Top left.
	indices.push_back(1);	// Top right.
	indices.push_back(2);	// Bottom right.

	indices.push_back(0);	// Top left.
	indices.push_back(2);	// Bottom right.
	indices.push_back(3);	// Bottom left.
}
