////////////////////////////////////////////////////////////////////////////////
// Filename: stateclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <string>
#include <functional>


////////////////////////////////////////////////////////////////////////////////
// Class name: StateClass
////////////////////////////////////////////////////////////////////////////////
class StateClass
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
	StateClass();
	StateClass(const StateClass&);
	~StateClass();

	bool Initialize(char*);
	void Shutdown();

	bool LoadLevel(char *);

	std::size_t GetSize();
	std::size_t GetMaxWidth();
	std::size_t GetHeight();
	char GetCurrentColor();
	char GetNextColor();
	float GetCurrentPosition();

	SpaceType* GetTopLeft();

	bool moveLeft();
	bool moveRight();

private:
	bool AllocateBoard();

private:
	std::size_t	m_size, m_maxWidth, m_height;
	SpaceType*	m_topLeft;

	char	m_currentColor, m_nextColor;
	float	m_currentPosition;
};
