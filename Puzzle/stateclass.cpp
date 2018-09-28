////////////////////////////////////////////////////////////////////////////////
// Filename: stateclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stateclass.h"


StateClass::StateClass()
{
	m_topLeft = nullptr;
}


StateClass::StateClass(const StateClass& other)
{
}


StateClass::~StateClass()
{
}


bool StateClass::Initialize(char* filename)
{
	bool result;


	// Load the level description from file.
	result = LoadLevel(filename);
	if (!result)
	{
		return false;
	}

	// Set the random seed for user piece colors.
	srand(time(NULL));

	// Set default starting values for our current piece and next piece color.
	m_activePiece.color = GenerateColor();
	m_nextColor = GenerateColor();
	m_currentPosition = 0.5f * (float)(m_height % 2);

	return true;
}


void StateClass::Shutdown()
{
	std::function<void(SpaceType*)> traverseDown, traverseRight;


	// Lambda function to recursively traverse down left column of board.
	traverseDown = [&traverseDown, &traverseRight](SpaceType* start)
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
	traverseRight = [&traverseRight](SpaceType* start)
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
	traverseDown(m_topLeft);

	return;
}


bool StateClass::LoadLevel(char* filename)
{
	SpaceType *traverseDown, *traverseRight;
	std::string line;
	std::ifstream fileReader;
	char colorKey;
	XMFLOAT4 colorValues;
	unsigned int colorCount;
	bool result;


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

	// Assume a default Alpha value of 1.0 for all colors.
	colorValues.w = 1.0f;

	// Read in color keys and HSV values, then store in color map.
	for (unsigned int i = 0; i < colorCount; i++)
	{
		fileReader >> colorKey >> colorValues.x >> colorValues.y >> colorValues.z;

		m_colors[colorKey] = colorValues;
	}

	// Read in the next line from the file, which should determine the dimensions.
	// NOTE: This could be more safely defined. 
	fileReader >> line >> m_maxWidth >> m_height >> std::ws;
	if (line.compare("dimensions") != 0)
	{
		return false;
	}

	// Initialize the board and get the number of piece containers created.
	result = AllocateBoard();
	if (!result)
	{
		return false;
	}

	// Set a pointer to the top of the left column on the board for board traversal.
	traverseDown = m_topLeft;

	while (!fileReader.eof())
	{
		// Get a string of color keys from the file.
		getline(fileReader, line);

		// Set a row traversal pointer to start at the left side of the board for the given row.
		traverseRight = traverseDown;

		for (char color : line)
		{
			// If traverseRight points to a null piece container, we have an error and need to fail out.
			if (!traverseRight)
			{
				return false;
			}

			// Set the piece container in traverseRight's color to the one matching the color key from line.
			traverseRight->color = color;

			// Move traverseRight to the right.
			traverseRight = traverseRight->rightNeighbor;
		}

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


std::size_t StateClass::GetSize()
{
	return m_size;
}


std::size_t StateClass::GetMaxWidth()
{
	return m_maxWidth;
}


std::size_t StateClass::GetHeight()
{
	return m_height;
}


XMFLOAT4 StateClass::GetColorHSVA(char color)
{
	return m_colors[color];
}


XMFLOAT4 StateClass::GetCurrentColor()
{
	return m_colors[m_activePiece.color];
}


char StateClass::GetNextColor()
{
	return m_nextColor;
}


bool StateClass::IsColorValid(char color)
{
	return m_colors.find(color) != m_colors.end();
}


float StateClass::GetCurrentPosition()
{
	return m_currentPosition;
}


StateClass::SpaceType* StateClass::GetTopLeft()
{
	return m_topLeft;
}


bool StateClass::MoveLeft()
{
	// Check to see if we're currently at the leftmost position.
	if (m_currentPosition < 1.0f)
	{
		return false;
	}

	// We aren't at the leftmost position, so update our active piece's entry points.
	m_activePiece.upperRightNeighbor = m_activePiece.upperLeftNeighbor;
	m_activePiece.upperLeftNeighbor = m_activePiece.upperLeftNeighbor->leftNeighbor;

	// Decrement our current position.
	m_currentPosition -= 1.0f;

	return true;
}


bool StateClass::MoveRight()
{
	// Check to see if we're currently at the rightmost position.
	if (m_currentPosition >= (float)(m_maxWidth - (m_height % 2) - 1))
	{
		return false;
	}

	// We aren't at the rightmost position, so update our active piece's entry points.
	m_activePiece.upperLeftNeighbor = m_activePiece.upperRightNeighbor;
	m_activePiece.upperRightNeighbor = m_activePiece.upperRightNeighbor->rightNeighbor;

	// Decrement our current position.
	m_currentPosition += 1.0f;

	return true;
}


void StateClass::ShootPiece(bool travelingLeft)
{
	SpaceType* endPoint;


	// Fire the piece to the left or right.
	if (travelingLeft)
	{
		endPoint = TravelLeft(&m_activePiece);
	}
	else
	{
		endPoint = TravelRight(&m_activePiece);
	}

	// Change the color of the endPoint piece to match the one we shot.
	endPoint->color = m_activePiece.color;

	// Update active piece's color.
	m_activePiece.color = m_nextColor;

	// Determine next piece color.
	m_nextColor = GenerateColor();

	return;
}

char StateClass::GenerateColor()
{
	auto randomKey = m_colors.begin();
	std::advance(randomKey, (rand() % m_colors.size()));
	return randomKey->first;
}


StateClass::SpaceType* StateClass::TravelLeft(SpaceType* entryPoint)
{
	// If upper left neighbor is null, check to see if there is an upper right neighbor
	if (!entryPoint->upperLeftNeighbor)
	{
		// If there is not an upper right neighbor, we've hit the ceiling; return position
		if (!entryPoint->upperRightNeighbor)
		{
			return entryPoint;
		}

		// Else, we've hit the left wall; ricochet by calling TravelRight
		return TravelRight(entryPoint);
	}

	// Else if upper left neighbor is an empty space, keep moving
	else if (entryPoint->upperLeftNeighbor->color == '_')
	{
		return TravelLeft(entryPoint->upperLeftNeighbor);
	}

	// Else, we've hit an occupied space; return position
	return entryPoint;
}


StateClass::SpaceType* StateClass::TravelRight(SpaceType* entryPoint)
{
	// If upper right neighbor is null, check to see if there is an upper left neighbor
	if (!entryPoint->upperRightNeighbor)
	{
		// If there is not an upper left neighbor, we've hit the ceiling; return position
		if (!entryPoint->upperLeftNeighbor)
		{
			return entryPoint;
		}

		// Else, we've hit the right wall; ricochet by calling TravelLeft
		return TravelLeft(entryPoint);
	}

	// Else if upper right neighbor is an empty space, keep moving
	else if (entryPoint->upperRightNeighbor->color == '_')
	{
		return TravelRight(entryPoint->upperRightNeighbor);
	}

	// Else, we've hit an occupied space; return position
	return entryPoint;
}


bool StateClass::AllocateBoard()
{
	SpaceType* traverseRight, *traverseDown;
	unsigned int rowWidth;


	// Fail if board dimensions too small.
	if (m_maxWidth < 2 || m_height == 0)
	{
		return false;
	}

	// Start counting number of spaces created.
	m_size = 0;

	// Create the first space in the top left corner of the board.
	m_topLeft = new SpaceType;
	if (!m_topLeft)
	{
		return false;
	}

	++m_size;

	// Set traverseDown to the first space of the board.
	traverseDown = m_topLeft;

	// Iterate down left side of board.
	for (unsigned int i = 0; i < m_height; i++)
	{
		// Set traverseRight to the far left position in this row.
		traverseRight = traverseDown;

		// Determine the width of the current row (maxRowWidth if an even row, maxRowWidth-1 if odd).
		rowWidth = (unsigned int) m_maxWidth - (i % 2);

		// Iterate over current row.
		for (unsigned int j = 0; j < rowWidth; j++)
		{
			// Set the color of the current space to empty.
			traverseRight->color = '_';

			// Assuming traverseRight isn't the first space in a row, link it to its upper neighbors.
			if (i > 0 && j > 0)
			{
				// Assign traverseRight's upper left neighbor and back again.
				traverseRight->upperLeftNeighbor = traverseRight->leftNeighbor->upperRightNeighbor;
				traverseRight->upperLeftNeighbor->lowerRightNeighbor = traverseRight;

				// If there is an upper right neighbor available to assign, assign it to traverseRight and back again.
				if (traverseRight->upperLeftNeighbor->rightNeighbor)
				{
					traverseRight->upperRightNeighbor = traverseRight->upperLeftNeighbor->rightNeighbor;
					traverseRight->upperRightNeighbor->lowerLeftNeighbor = traverseRight;
				}
			}

			// If we aren't at the end of the current row, create a new space.
			if (j < rowWidth - 1)
			{
				// Create a new space to the right of traverseRight and link the two.
				traverseRight->rightNeighbor = new SpaceType;
				if (!traverseRight->rightNeighbor)
				{
					return false;
				}
				traverseRight->rightNeighbor->leftNeighbor = traverseRight;

				// Move traverseRight to the next space.
				traverseRight = traverseRight->rightNeighbor;

				++m_size;
			}
		}

		// If we aren't on the last row, we want to make a new one.
		if (i < m_height - 1)
		{
			// We are moving from an odd numbered row to an even numbered row.
			if (i % 2)
			{
				// Create a new space to the lower left of traverseDown and link the two.
				traverseDown->lowerLeftNeighbor = new SpaceType;
				if (!traverseDown->lowerLeftNeighbor)
				{
					return false;
				}
				traverseDown->lowerLeftNeighbor->upperRightNeighbor = traverseDown;

				// Move traverseDown down to the next row.
				traverseDown = traverseDown->lowerLeftNeighbor;

				++m_size;
			}
			// We are moving from an even numbered row to an odd numbered row.
			else
			{
				// Create a new space to the lower right of traverseDown and link the two.
				traverseDown->lowerRightNeighbor = new SpaceType;
				if (!traverseDown->lowerRightNeighbor)
				{
					return false;
				}
				traverseDown->lowerRightNeighbor->upperLeftNeighbor = traverseDown;

				// Link that new space to traverseDown's right neighbor.
				traverseDown->rightNeighbor->lowerLeftNeighbor = traverseDown->lowerRightNeighbor;
				traverseDown->lowerRightNeighbor->upperRightNeighbor = traverseDown->rightNeighbor;

				// Move traverseDown down to next row.
				traverseDown = traverseDown->lowerRightNeighbor;

				++m_size;
			}
		}

		// We are on the last row; let's establish entry points into the bottom of the board.
		else
		{
			// The bottom row is an even numbered row; it would only have one entry point, and the other would be null.
			if (i % 2)
			{
				m_activePiece.upperLeftNeighbor = nullptr;
				m_activePiece.upperRightNeighbor = traverseDown;
			}
			// The bottom row is an odd numbered row; entry points would be the two bottom left pieces.
			else
			{
				m_activePiece.upperLeftNeighbor = traverseDown;
				m_activePiece.upperRightNeighbor = traverseDown->rightNeighbor;
			}
		}
	}

	return true;
}
