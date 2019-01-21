#pragma once
; class ARECT
{
public:
	float m_Left;
	float m_Top;
	float m_Right;
	float m_Buttom;
	
public:
	ARECT() {};
	ARECT(float l, float t, float r, float b)
	{
		m_Left = l;
		m_Top = t;
		m_Right = r;
		m_Buttom = b;
	};
	~ARECT() {};
};


