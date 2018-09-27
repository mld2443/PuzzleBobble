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
	std::vector<VertexType> vertices;
	std::vector<unsigned long> indices;


	// Create piece geometry.
	CreateGeometry(vertices, indices);

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

	// Load the texture for pieces.
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

	return;
}


void BoardClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderWithInstanceBuffer(deviceContext);

	return;
}


bool BoardClass::InitializeLevel(ID3D11Device* device, char* filename, StateClass* state)
{
	bool result;
	std::vector<InstanceType> instances;


	// Load level in from file and get number of piece containers.
	result = LoadColors(filename);
	if (!result)
	{
		return false;
	}

	// Load the instance array with data.
	result = LoadInstances(instances, state);
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

	return true;
}


bool BoardClass::LoadColors(char* filename)
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

	// Read in color keys and HSV values, then store in color map.
	for (unsigned int i = 0; i < colorCount; i++)
	{
		fileReader >> colorKey >> colorValues.x >> colorValues.y >> colorValues.z;

		m_colors[colorKey] = colorValues;
	}

	// Close our file now that we're done with it.
	fileReader.close();

	return true;
}


bool BoardClass::LoadInstances(std::vector<InstanceType>& instances, StateClass* state)
{
	float boardWidth, boardHeight, positionX, positionY, stepX, stepY;
	StateClass::SpaceType *traverseDown, *traverseRight;
	InstanceType tempInstance;


	// Set the stepping distance.
	stepX = 2.1f * PIECEWIDTH;
	stepY = 0.5f * stepX * SQRT3;

	// Calculate the width between centers of the farthest apart spaces.
	boardWidth = (float)(state->GetMaxWidth() - 1) * stepX;
	boardHeight = (float)(state->GetHeight() - 1) * stepY;

	// Offset the starting position so the whole board appears centered.
	positionX = -0.5f * boardWidth;
	positionY = 0.5f * boardHeight;

	// Start the traversal of our board.
	traverseDown = state->GetTopLeft();

	while (traverseDown)
	{
		// Set a row traversal pointer to start at the left side of the board for the given row.
		traverseRight = traverseDown;

		while (traverseRight)
		{
			if (traverseRight->color != '_')
			{
				// Check to make sure the Space pointer's color is in the board's color map.
				if (m_colors.find(traverseRight->color) == m_colors.end())
				{
					return false;
				}

				// If the Space pointer's color is not blank, create an instance in its position.
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
		positionX -= stepX * ((float)state->GetMaxWidth() - 0.5f);
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

	return true;
}


void BoardClass::CreateGeometry(std::vector<VertexType>& vertices, std::vector<unsigned long>& indices)
{
	VertexType vertex;


	// Load the vertex array with data.
	vertex.position =	XMFLOAT3(-PIECEWIDTH, PIECEWIDTH, 0.0f);	// Top left.
	vertex.tex =		XMFLOAT2(0.0f, 0.0f);
	vertices.push_back(vertex);

	vertex.position =	XMFLOAT3(PIECEWIDTH, PIECEWIDTH, 0.0f);		// Top right.
	vertex.tex =		XMFLOAT2(1.0f, 0.0f);
	vertices.push_back(vertex);

	vertex.position =	XMFLOAT3(PIECEWIDTH, -PIECEWIDTH, 0.0f);	// Bottom right.
	vertex.tex =		XMFLOAT2(1.0f, 1.0f);
	vertices.push_back(vertex);

	vertex.position =	XMFLOAT3(-PIECEWIDTH, -PIECEWIDTH, 0.0f);	// Bottom left.
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
