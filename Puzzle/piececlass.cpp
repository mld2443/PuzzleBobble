////////////////////////////////////////////////////////////////////////////////
// Filename: piececlass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "piececlass.h"


PieceClass::PieceClass()
{
}


PieceClass::PieceClass(const PieceClass&)
{
}


PieceClass::~PieceClass()
{
}


void PieceClass::Initialize(PieceClass::PieceColor color)
{
	m_Color = color;
}


PieceClass::PieceColor PieceClass::GetPieceColor()
{
	return m_Color;
}