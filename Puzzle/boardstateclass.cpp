////////////////////////////////////////////////////////////////////////////////
// Filename: boardstateclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "boardstateclass.h"


BoardStateClass::PieceIterator::PieceIterator() : row(nullptr), pos(nullptr)
{
}


BoardStateClass::PieceIterator::PieceIterator(SpaceType* space) : row(space), pos(space)
{
}


BoardStateClass::PieceIterator::PieceIterator(const PieceIterator& other) : row(other.row), pos(other.pos)
{
}


BoardStateClass::PieceIterator& BoardStateClass::PieceIterator::operator++()
{
	if (pos->rightNeighbor)
		pos = pos->rightNeighbor;
	else
		NextRow();
	return *this;
}


BoardStateClass::PieceIterator BoardStateClass::PieceIterator::operator++(int)
{
	PieceIterator old(*this);
	operator++();
	return old;
}


bool BoardStateClass::PieceIterator::operator==(const PieceIterator& rhs) const
{
	return pos == rhs.pos;
}


bool BoardStateClass::PieceIterator::operator!=(const PieceIterator& rhs) const
{
	return pos != rhs.pos;
}


BoardStateClass::SpaceType& BoardStateClass::PieceIterator::operator*()
{
	return *pos;
}


void BoardStateClass::PieceIterator::NextRow()
{
	if (row->lowerLeftNeighbor)
	{
		row = row->lowerLeftNeighbor;
	}
	else
	{
		row = row->lowerRightNeighbor;
	}
	pos = row;
}


BoardStateClass::BoardStateClass()
{
	m_topLeft = nullptr;
}


BoardStateClass::BoardStateClass(const BoardStateClass& other)
{
}


BoardStateClass::~BoardStateClass()
{
}


bool BoardStateClass::Initialize(std::ifstream& fileReader)
{
	SpaceType* rowTraverse, *columnTraverse;
	std::string line;
	unsigned int maxRowWidth, numRows;
	bool result;


	// Read in the next line from the file, which should determine the dimensions.
	// NOTE: This could be more safely defined. 
	fileReader >> line >> maxRowWidth >> numRows >> std::ws;
	if (line.compare("dimensions") != 0)
	{
		return false;
	}

	// Initialize the board and get the number of piece containers created.
	result = AllocateBoard(maxRowWidth, numRows);
	if (!result)
	{
		return false;
	}

	// Set a pointer to the top of the left column on the board for board traversal.
	columnTraverse = m_topLeft;

	while (!fileReader.eof())
	{
		// Get a string of color keys from the file.
		getline(fileReader, line);

		// Set a row traversal pointer to start at the left side of the board for the given row.
		rowTraverse = columnTraverse;

		for (char color : line)
		{
			// If rowTraverse points to a null piece container, we have an error and need to fail out.
			if (!rowTraverse)
			{
				return false;
			}

			// Set the piece container in rowTraverse's color to the one matching the color key from line.
			// NOTE: We aren't checking to see if color is defined in the color map.
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

	return true;
}


void BoardStateClass::Shutdown()
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


std::size_t BoardStateClass::size()
{
	return m_size;
}


BoardStateClass::PieceIterator BoardStateClass::begin()
{
	return PieceIterator(m_topLeft);
}


BoardStateClass::PieceIterator BoardStateClass::end()
{
	return PieceIterator();
}


bool BoardStateClass::AllocateBoard(unsigned int maxRowWidth, unsigned int rows)
{
	SpaceType* rowTraverse, *columnTraverse;
	unsigned int rowWidth;


	// Fail if board dimensions too small.
	if (maxRowWidth < 2 || rows == 0)
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

	// Set columnTraverse to the first space of the board.
	columnTraverse = m_topLeft;

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
			// Set the color of the current space to empty.
			rowTraverse->color = '_';

			// Assuming rowTraverse isn't the first space in a row, link it to its upper neighbors.
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

			// If we aren't at the end of the current row, create a new space.
			if (j < rowWidth - 1)
			{
				// Create a new space to the right of rowTraverse and link the two.
				rowTraverse->rightNeighbor = new SpaceType;
				if (!rowTraverse->rightNeighbor)
				{
					return false;
				}
				rowTraverse->rightNeighbor->leftNeighbor = rowTraverse;

				// Move rowTraverse to the next space.
				rowTraverse = rowTraverse->rightNeighbor;

				++m_size;
			}
		}

		// If we aren't on the last row, we want to make a new one.
		if (i < rows - 1)
		{
			// We are moving from an odd numbered row to an even numbered row.
			if (i % 2)
			{
				// Create a new space to the lower left of columnTraverse and link the two.
				columnTraverse->lowerLeftNeighbor = new SpaceType;
				if (!columnTraverse->lowerLeftNeighbor)
				{
					return false;
				}
				columnTraverse->lowerLeftNeighbor->upperRightNeighbor = columnTraverse;

				// Move columnTraverse down to the next row.
				columnTraverse = columnTraverse->lowerLeftNeighbor;

				++m_size;
			}
			// We are moving from an even numbered row to an odd numbered row.
			else
			{
				// Create a new space to the lower right of columnTraverse and link the two.
				columnTraverse->lowerRightNeighbor = new SpaceType;
				if (!columnTraverse->lowerRightNeighbor)
				{
					return false;
				}
				columnTraverse->lowerRightNeighbor->upperLeftNeighbor = columnTraverse;

				// Link that new space to columnTraverse's right neighbor.
				columnTraverse->rightNeighbor->lowerLeftNeighbor = columnTraverse->lowerRightNeighbor;
				columnTraverse->lowerRightNeighbor->upperRightNeighbor = columnTraverse->rightNeighbor;

				// Move columnTraverse down to next row.
				columnTraverse = columnTraverse->lowerRightNeighbor;

				++m_size;
			}
		}
	}

	return true;
}
