////////////////////////////////////////////////////////////////////////////////
// Filename: boardclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <functional>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "drawableinterface.h"


///////////////
// CONSTANTS //
///////////////
#define SQRT075 0.86602540378f


////////////////////////////////////////////////////////////////////////////////
// Class name: BoardClass
////////////////////////////////////////////////////////////////////////////////
class BoardClass : public DrawableInterface
{
private:
	struct PieceType
	{
		char color =					'_';
		PieceType* upperLeftNeighbor =	nullptr;
		PieceType* upperRightNeighbor =	nullptr;
		PieceType* rightNeighbor =		nullptr;
		PieceType* lowerRightNeighbor =	nullptr;
		PieceType* lowerLeftNeighbor =	nullptr;
		PieceType* leftNeighbor =		nullptr;
	};

public:
	BoardClass();
	BoardClass(const BoardClass&);
	~BoardClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*) override;
	void Shutdown() override;
	void Render(ID3D11DeviceContext*) override;

private:
	void CreateGeometry(VertexType*, unsigned long*);
	int InitializeBoard(unsigned int, unsigned int);
	void ShutdownBoard();
	int LoadLevel(char*);

private:
	std::map<char, XMFLOAT4>	m_colors;
	PieceType*					m_board;
};
