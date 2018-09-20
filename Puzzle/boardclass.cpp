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
	InstanceType* instances;
	int vertexCount, indexCount, instanceCount;


	// Load level in from file and get number of piece containers.
	instanceCount = LoadLevel("../Puzzle/data/level.txt");
	if (instanceCount == 0)
	{
		return false;
	}

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
	ShutdownBoard();

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


int BoardClass::InitializeBoard(unsigned int maxRowWidth, unsigned int rows)
{
	PieceType* rowTraverse, * columnTraverse;
	unsigned int rowWidth, pieceCount;


	// Fail if board dimensions too small.
	if (maxRowWidth < 2 || rows == 0)
	{
		return 0;
	}

	// Start counting number of pieces created.
	pieceCount = 0;

	// Create the first piece container in the top left corner of the board.
	m_board = new PieceType;
	if (!m_board)
	{
		return 0;
	}

	pieceCount++;

	// Set columnTraverse to the first piece container of the board.
	columnTraverse = m_board;

	// Iterate down left side of board.
	for (unsigned int i = 0; i < rows; i++)
	{
		// Set rowTraverse to the far left position in this row.
		rowTraverse = columnTraverse;

		// Determine the width of the current row (maxRowWidth if an even row, maxRowWidth-1 if odd).
		rowWidth = maxRowWidth - (i % 2);

		// Iterate over current row.
		for (unsigned int j = 0; j < rowWidth; j++)
		{
			// Set the color of the current piece container to empty.
			rowTraverse->color = '_';

			// Assuming rowTraverse isn't the first piece container in a row, link it to its upper neighbors.
			if (i > 0 && j > 0)
			{
				// Assign rowTraverse's upper left neighbor and back again.
				rowTraverse->upperLeftNeighbor = rowTraverse->leftNeighbor->upperRightNeighbor;
				rowTraverse->upperLeftNeighbor->lowerRightNeighbor = rowTraverse;

				// If there is an upper right neighbor available to assign, assign it to rowTraverse and back again.
				if (rowTraverse->upperLeftNeighbor->rightNeighbor) 
				{
					rowTraverse->upperRightNeighbor = rowTraverse->upperLeftNeighbor->rightNeighbor;
					rowTraverse->upperRightNeighbor->lowerLeftNeighbor = rowTraverse;
				}
			}

			// If we aren't at the end of the current row, create a new piece container.
			if (j < rowWidth - 1)
			{
				// Create a new piece container to the right of rowTraverse and link the two.
				rowTraverse->rightNeighbor = new PieceType;
				if (!rowTraverse->rightNeighbor)
				{
					return 0;
				}
				rowTraverse->rightNeighbor->leftNeighbor = rowTraverse;

				// Move rowTraverse to the next piece container.
				rowTraverse = rowTraverse->rightNeighbor;

				pieceCount++;
			}
		}

		// If we aren't on the last row, we want to make a new one.
		if (i < rows - 1)
		{
			// We are moving from an odd numbered row to an even numbered row.
			if (i % 2)
			{
				// Create a new piece container to the lower left of columnTraverse and link the two.
				columnTraverse->lowerLeftNeighbor = new PieceType;
				if (!columnTraverse->lowerLeftNeighbor)
				{
					return 0;
				}
				columnTraverse->lowerLeftNeighbor->upperRightNeighbor = columnTraverse;

				// Move columnTraverse down to the next row.
				columnTraverse = columnTraverse->lowerLeftNeighbor;

				pieceCount++;
			}
			// We are moving from an even numbered row to an odd numbered row.
			else
			{
				// Create a new piece container to the lower right of columnTraverse and link the two.
				columnTraverse->lowerRightNeighbor = new PieceType;
				if (!columnTraverse->lowerRightNeighbor)
				{
					return 0;
				}
				columnTraverse->lowerRightNeighbor->upperLeftNeighbor = columnTraverse;

				// Link that new piece container to columnTraverse's right neighbor.
				columnTraverse->rightNeighbor->lowerLeftNeighbor = columnTraverse->lowerRightNeighbor;
				columnTraverse->lowerRightNeighbor->upperRightNeighbor = columnTraverse->rightNeighbor;

				// Move columnTraverse down to next row.
				columnTraverse = columnTraverse->lowerRightNeighbor;

				pieceCount++;
			}
		}
	}

	return pieceCount;
}


void BoardClass::ShutdownBoard()
{
	std::function<void(PieceType*)> traverseDown, traverseRight;
	

	// Lambda function to recursively traverse down left column of board.
	traverseDown = [&traverseDown, &traverseRight](PieceType* start)
	{
		// First see if there is a left branch to traverse down.
		if (start->lowerLeftNeighbor)
		{
			traverseDown(start->lowerLeftNeighbor);
		}
		// Else check to see if there is a right branch to traverse down.
		else if (start->lowerRightNeighbor)
		{
			traverseDown(start->lowerRightNeighbor);
		}
		
		// Traverse and delete the current row.
		traverseRight(start);

		return;
	};

	// Lambda function to recursively traverse right through a row of board and recursively delete piece containers.
	traverseRight = [&traverseRight](PieceType* start)
	{
		// Traverse right if possible.
		if (start->rightNeighbor)
		{
			traverseRight(start->rightNeighbor);
		}
		
		// Delete piece container.
		delete start;

		return;
	};

	// Begin traversing board and deleting piece containers.
	traverseDown(m_board);
	
	return;
}


int BoardClass::LoadLevel(char* filename)
{
	unsigned int colorCount, maxRowWidth, numRows, pieceCount;
	char colorKey;
	XMFLOAT4 colorValues;
	PieceType* rowTraverse, *columnTraverse;
	std::ifstream fileReader;
	std::string token;


	// Open level file for reading.
	fileReader = std::ifstream(filename);
	if (!fileReader)
	{
		return 0;
	}

	// Read in the first token in the line, which should be the word "colors."
	fileReader >> token;
	if (token.compare("colors") != 0)
	{
		return 0;
	}

	// Read in the number of colors we will define for pieces.
	fileReader >> colorCount;

	// Read in color keys and RGBA values, then store in color map.
	for (unsigned int i = 0; i < colorCount; i++) 
	{
		fileReader >> colorKey >> colorValues.x >> colorValues.y >> colorValues.z >> colorValues.w;

		m_colors[colorKey] = colorValues;
	}

	// Add a blank color to color map for empty spaces.
	m_colors['_'] = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	// Read in the next token from the file, which should be the word "dimensions."
	fileReader >> token;
	if (token.compare("dimensions") != 0)
	{
		return 0;
	}

	// Read in the maximum row width and number of rows for the board.
	fileReader >> maxRowWidth >> numRows >> std::ws;

	// Initialize the board and get the number of piece containers created.
	pieceCount = InitializeBoard(maxRowWidth, numRows);
	if (pieceCount == 0)
	{
		return 0;
	}

	// Set a pointer to the top of the left column on the board for board traversal.
	columnTraverse = m_board;

	while (!fileReader.eof())
	{
		// Get a string of color keys from the file.
		getline(fileReader, token);

		// Set a row traversal pointer to start at the left side of the board for the given row.
		rowTraverse = columnTraverse;

		for (char color : token)
		{
			// If rowTraverse points to a null piece container, we have an error and need to fail out.
			if (!rowTraverse) 
			{
				return 0;
			}

			// Set the piece container in rowTraverse's color to the one matching the color key from token.
			rowTraverse->color = color;

			// Move rowTraverse to the right.
			rowTraverse = rowTraverse->rightNeighbor;
		}

		// Move columnTraverse down a row, either to the left if moving from an even row to an odd row, or right for the opposite.
		if (columnTraverse->lowerLeftNeighbor)
		{
			columnTraverse = columnTraverse->lowerLeftNeighbor;
		}
		else
		{
			columnTraverse = columnTraverse->lowerRightNeighbor;
		}
	}

	return pieceCount;
}
