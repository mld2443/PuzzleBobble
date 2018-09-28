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
#include <map>
#include <stdlib.h>
#include <time.h>
#include <directxmath.h>

using namespace DirectX;

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

	XMFLOAT4 GetColorHSVA(char);
	XMFLOAT4 GetCurrentColor();
	char GetNextColor();
	bool IsColorValid(char);


	float GetCurrentPosition();

	SpaceType* GetTopLeft();

	bool MoveLeft();
	bool MoveRight();

	void ShootPiece(bool);

private:
	char GenerateColor();

	SpaceType* TravelLeft(SpaceType*);
	SpaceType* TravelRight(SpaceType*);

	bool AllocateBoard();

private:
	std::size_t	m_size, m_maxWidth, m_height;
	SpaceType*	m_topLeft, m_activePiece;

	std::map<char, XMFLOAT4>	m_colors;

	char	m_nextColor;
	float	m_currentPosition;
};
