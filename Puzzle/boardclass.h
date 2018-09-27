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
#include <vector>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "drawableinterface.h"
#include "boardstateclass.h"


///////////////
// CONSTANTS //
///////////////
#define PIECEWIDTH 0.5f
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

	bool InitializeLevel(ID3D11Device*, char*, StateClass*);

private:
	bool LoadColors(char*);
	bool LoadInstances(std::vector<InstanceType>&, StateClass*);
	void CreateGeometry(std::vector<VertexType>&, std::vector<unsigned long>&);

private:
	std::map<char, XMFLOAT3>	m_colors;
};
