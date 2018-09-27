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
	return LoadLevel(filename);
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
	bool result;


	// Open file for reading.
	fileReader = std::ifstream(filename);
	if (!fileReader)
	{
		return false;
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


StateClass::SpaceType* StateClass::GetTopLeft()
{
	return m_topLeft;
}


bool StateClass::AllocateBoard()
{
	SpaceType* rowTraverse, *columnTraverse;
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
	columnTraverse = m_topLeft;

	// Iterate down left side of board.
	for (unsigned int i = 0; i < m_height; i++)
	{
		// Set traverseRight to the far left position in this row.
		rowTraverse = columnTraverse;

		// Determine the width of the current row (maxRowWidth if an even row, maxRowWidth-1 if odd).
		rowWidth = m_maxWidth - (i % 2);

		// Iterate over current row.
		for (unsigned int j = 0; j < rowWidth; j++)
		{
			// Set the color of the current space to empty.
			rowTraverse->color = '_';

			// Assuming traverseRight isn't the first space in a row, link it to its upper neighbors.
			if (i > 0 && j > 0)
			{
				// Assign traverseRight's upper left neighbor and back again.
				rowTraverse->upperLeftNeighbor = rowTraverse->leftNeighbor->upperRightNeighbor;
				rowTraverse->upperLeftNeighbor->lowerRightNeighbor = rowTraverse;

				// If there is an upper right neighbor available to assign, assign it to traverseRight and back again.
				if (rowTraverse->upperLeftNeighbor->rightNeighbor)
				{
					rowTraverse->upperRightNeighbor = rowTraverse->upperLeftNeighbor->rightNeighbor;
					rowTraverse->upperRightNeighbor->lowerLeftNeighbor = rowTraverse;
				}
			}

			// If we aren't at the end of the current row, create a new space.
			if (j < rowWidth - 1)
			{
				// Create a new space to the right of traverseRight and link the two.
				rowTraverse->rightNeighbor = new SpaceType;
				if (!rowTraverse->rightNeighbor)
				{
					return false;
				}
				rowTraverse->rightNeighbor->leftNeighbor = rowTraverse;

				// Move traverseRight to the next space.
				rowTraverse = rowTraverse->rightNeighbor;

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
				columnTraverse->lowerLeftNeighbor = new SpaceType;
				if (!columnTraverse->lowerLeftNeighbor)
				{
					return false;
				}
				columnTraverse->lowerLeftNeighbor->upperRightNeighbor = columnTraverse;

				// Move traverseDown down to the next row.
				columnTraverse = columnTraverse->lowerLeftNeighbor;

				++m_size;
			}
			// We are moving from an even numbered row to an odd numbered row.
			else
			{
				// Create a new space to the lower right of traverseDown and link the two.
				columnTraverse->lowerRightNeighbor = new SpaceType;
				if (!columnTraverse->lowerRightNeighbor)
				{
					return false;
				}
				columnTraverse->lowerRightNeighbor->upperLeftNeighbor = columnTraverse;

				// Link that new space to traverseDown's right neighbor.
				columnTraverse->rightNeighbor->lowerLeftNeighbor = columnTraverse->lowerRightNeighbor;
				columnTraverse->lowerRightNeighbor->upperRightNeighbor = columnTraverse->rightNeighbor;

				// Move traverseDown down to next row.
				columnTraverse = columnTraverse->lowerRightNeighbor;

				++m_size;
			}
		}
	}

	return true;
}
