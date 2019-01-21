#pragma once
#include "MaxGlobal.h"

struct ATexMap
{
	int		iIndex;
	int		iType; 	// �ؽ��� Ÿ�� �ε���
	TSTR	strTextureName; 	// �ؽ��� �̸�.
};

struct AMtrl
{
	int				iIndex;
	int				iNumSubMtl;	// ���� ���͸��� ����	
	int				iNumMap;	// �ؽ��� �� ����
	TSTR			strName; // ���͸��� �̸�	
	TSTR			strClassName;// ��Ŭ���� �̸�
	vector<ATexMap>	TexMaps; // �� Ÿ�Ե�	
	vector<AMtrl>	SubMaterial; // ���� ���͸��� ���庤��
};

class MaxMtrl : public MaxGlobal
{
public:
	vector<AMtrl>	m_Material;// ��� �� ���� ����Ʈ	
public:
	void	AddMaterial();									//���͸��� ����Ʈ���� ���� ���
	void	GetTexture(AMtrl* pDestMtl, Mtl* pSrcMtl);		//���͸��� ����Ʈ���� ���� ���
public:
	MaxMtrl();
	virtual ~MaxMtrl();
};

