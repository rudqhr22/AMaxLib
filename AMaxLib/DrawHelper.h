#pragma once
#include "ADxShape.h"
#include "ACamera.h"
#include "AHeroObj.h"

class DrawHelper : public ASingleton< DrawHelper >
{
private:
	friend class ASingleton<DrawHelper>;
public:
	//ALineShape * m_pLine;
	ALineShape m_pLine;
	//ComPtr<ID3D11DeviceContext>				m_pImmediateContext;

public:
	void	DrawInit();
	//void	DrawAseLine(AAseObj* pObj, ACamera* pCamera, ID3D11DeviceContext* pContext);

	void	BoxLine(A_Box* pObj, ID3D11DeviceContext* pContext, ACamera* pCamera );


public:
	DrawHelper();
	virtual ~DrawHelper();
};
#define I_DRAW DrawHelper::GetInstance()

