////////////////////////////////////////////////////////////////////////////////
// Filename: boardstateclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <string>
#include <functional>
#include <iterator>


////////////////////////////////////////////////////////////////////////////////
// Class name: BoardStateClass
////////////////////////////////////////////////////////////////////////////////
class BoardStateClass
{
private:
	struct SpaceType
	{
		char color =					'_';
		SpaceType* upperLeftNeighbor =	nullptr;
		SpaceType* upperRightNeighbor =	nullptr;
		SpaceType* rightNeighbor =		nullptr;
		SpaceType* lowerRightNeighbor =	nullptr;
		SpaceType* lowerLeftNeighbor =	nullptr;
		SpaceType* leftNeighbor =		nullptr;
	};

public:
	class PieceIterator : public std::iterator<std::input_iterator_tag, SpaceType>
	{
	public:
		PieceIterator();
		PieceIterator(SpaceType*);
		PieceIterator(const PieceIterator&);

		PieceIterator& operator++();
		PieceIterator operator++(int);

		bool operator==(const PieceIterator&) const;
		bool operator!=(const PieceIterator&) const;

		SpaceType& operator*();

		void NextRow();

	private:
		SpaceType *row, *pos;
	};

public:
	BoardStateClass();
	BoardStateClass(const BoardStateClass&);
	~BoardStateClass();

	bool Initialize(std::ifstream&);
	void Shutdown();

	std::size_t size();

	PieceIterator begin();
	PieceIterator end();

private:
	bool AllocateBoard(unsigned int, unsigned int);

private:
	std::size_t	m_size;
	SpaceType*	m_topLeft;
};
