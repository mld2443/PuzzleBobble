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
	result = InitializeVertexBuffer(device, vertices.data(), (int) vertices.size());
	if (!result)
	{
		return false;
	}

	// Initialize the index buffer.
	result = InitializeIndexBuffer(device, indices.data(), (int) indices.size());
	if (!result)
	{
		return false;
	}

	// Load the texture for pieces.
	result = LoadTexture(device, deviceContext, PIECEPATH);
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


bool BoardClass::InitializeInstances(ID3D11Device* device, StateClass* state)
{
	std::vector<InstanceType> instances;
	unsigned int numFreePieces;
	bool result;


	numFreePieces = 2;
	m_maxPieces = state->GetHeight() * state->GetMaxWidth() - (state->GetHeight() / 2) + numFreePieces;

	// Calculate instance positions for drawing.
	result = CalculateInstancePositions(instances, state);
	if (!result)
	{
		return false;
	}

	// Finally initialize the instance buffer.
	result = InitializeInstanceBuffer(device, instances.data(), m_maxPieces);
	if (!result)
	{
		return false;
	}

	return true;
}


bool BoardClass::UpdateInstances(ID3D11DeviceContext* deviceContext, StateClass* state)
{
	std::vector<InstanceType> instances;
	bool result;


	// Calculate instance positions for drawing.
	result = CalculateInstancePositions(instances, state);
	if (!result)
	{
		return false;
	}

	// Update instance buffer with new instance positions.
	result = UpdateInstanceBuffer(deviceContext, instances.data(), instances.size());
	if (!result)
	{
		return false;
	}


	return true;
}


bool BoardClass::CalculateInstancePositions(std::vector<InstanceType>& instances, StateClass* state)
{
	float boardWidth, boardHeight, positionX, positionY, stepX, stepY;
	StateClass::SpaceType *traverseDown, *traverseRight;
	InstanceType tempInstance;


	// Set the stepping distance.
	stepX = 2.1f * PIECEWIDTH;
	stepY = 0.5f * stepX * SQRT3;

	// Calculate the width between centers of the farthest apart spaces.
	boardWidth = (float)(state->GetMaxWidth() - 1) * stepX;
	boardHeight = (float)(state->GetHeight()) * stepY;

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
				if (!state->IsColorValid(traverseRight->color))
				{
					return false;
				}

				// If the Space pointer's color is not blank, create an instance in its position.
				tempInstance.position =	XMFLOAT3(positionX, positionY, 0.0f);
				tempInstance.HSVA =		state->GetColorHSVA(traverseRight->color);
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

	// Set up a piece a the bottom of the board for the player to position.
	positionX = -0.5f * boardWidth + (stepX * state->GetCurrentPosition());
	positionY = -0.5f * boardHeight;

	// Add this piece to the instance vector.
	tempInstance.position =	XMFLOAT3(positionX, positionY, 0.0f);
	tempInstance.HSVA =		state->GetCurrentColor();
	instances.push_back(tempInstance);

	// Create template for an empty space.
	tempInstance.position =	XMFLOAT3(0.0f, 0.0f, 0.0f);
	tempInstance.HSVA =		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	// Fill rest of instance vector with empty spaces.
	for (unsigned int i = instances.size(); i < m_maxPieces; i++)
	{
		instances.push_back(tempInstance);
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
