////////////////////////////////////////////////////////////////////////////////
// Filename: boardclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <map>
#include <fstream>
#include <string>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "drawableinterface.h"
#include "boardstateclass.h"


///////////////
// CONSTANTS //
///////////////
#define SQRT075 0.86602540378f
#define SQRT3 1.73205080757f


////////////////////////////////////////////////////////////////////////////////
// Class name: BoardClass
////////////////////////////////////////////////////////////////////////////////
class BoardClass : public DrawableInterface
{
public:
	BoardClass();
	BoardClass(const BoardClass&);
	~BoardClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*) override;
	void Shutdown() override;
	void Render(ID3D11DeviceContext*) override;

private:
	bool LoadLevel(char*);
	void LoadInstances(InstanceType*);
	void CreateGeometry(VertexType*, unsigned long*);

private:
	std::map<char, XMFLOAT4>	m_colors;
	BoardStateClass*			m_boardState;
};
