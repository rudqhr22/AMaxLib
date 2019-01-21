#pragma once
#include "MaxGlobal.h"

struct ATexMap
{
	int		iIndex;
	int		iType; 	// 텍스쳐 타입 인덱스
	TSTR	strTextureName; 	// 텍스쳐 이름.
};

struct AMtrl
{
	int				iIndex;
	int				iNumSubMtl;	// 서브 매터리얼 개수	
	int				iNumMap;	// 텍스쳐 맵 갯수
	TSTR			strName; // 매터리얼 이름	
	TSTR			strClassName;// 맵클래스 이름
	vector<ATexMap>	TexMaps; // 맵 타입들	
	vector<AMtrl>	SubMaterial; // 서브 매터리얼 저장벡터
};

class MaxMtrl : public MaxGlobal
{
public:
	vector<AMtrl>	m_Material;// 출력 할 정보 리스트	
public:
	void	AddMaterial();									//매터리얼 리스트에서 정보 얻기
	void	GetTexture(AMtrl* pDestMtl, Mtl* pSrcMtl);		//매터리얼 리스트에서 정보 얻기
public:
	MaxMtrl();
	virtual ~MaxMtrl();
};

