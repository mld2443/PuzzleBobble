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


////////////////////////////////////////////////////////////////////////////////
// Class name: BoardStateClass
////////////////////////////////////////////////////////////////////////////////
class BoardStateClass
{
public:
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
	BoardStateClass();
	BoardStateClass(const BoardStateClass&);
	~BoardStateClass();

	bool Initialize(char*);
	void Shutdown();

	bool LoadLevel(char *);

	std::size_t GetSize();
	std::size_t GetMaxWidth();
	std::size_t GetHeight();

	SpaceType* GetTopLeft();

private:
	bool AllocateBoard();

private:
	std::size_t	m_size, m_maxWidth, m_height;
	SpaceType*	m_topLeft;
};
