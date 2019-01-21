#pragma once
#include "ACamera.h"
#include "AModel.h"
class AFPSCamera :	public ACamera
{
private:
	AModel * m_pShape;

	
public:
	void SetCameraTarget(AModel* pTarget);
	bool	Frame();


public:
	AFPSCamera();
	virtual ~AFPSCamera();
};

