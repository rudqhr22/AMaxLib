#pragma once
#include <math.h>


class APOINT
{
public:
	float m_iX;
	float m_iY;

public:
	APOINT():m_iX(0),m_iY(0) {};
	APOINT(float x, float y) 	{
	m_iX = x;
	m_iY = y;	
	};

	APOINT(const APOINT& vp)
	{
		m_iX = vp.m_iX;
		m_iY = vp.m_iY;
	}
	float Length()
	{
		return (float)sqrt((m_iX* m_iX) + (m_iY* m_iY));
	}
	//APOINT Length(const APOINT& vp)
	//{
	//	APOINT dir;
	//	//dir  = m_


	//	return (float)sqrt((m_iX* m_iX) + (m_iY* m_iY));
	//}

	APOINT Normal()
	{
		float fLength = 1.0f / sqrt((m_iX* m_iX) + (m_iY* m_iY));
		return APOINT(m_iX*fLength, m_iY*fLength);
	}
	APOINT operator /= (APOINT const &Ap)
	{

	}
	APOINT operator - (APOINT const &Ap)
	{
		//APOINT p;
		this->m_iX - Ap.m_iX;
		this->m_iY - Ap.m_iY;
		return *this;
	}


	~APOINT() {};
};

