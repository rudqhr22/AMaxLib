#pragma once
#include "MatrixMap.h"





struct ATri
{
	PNCT_VERTEX	v[3];
	int			iSubIndex;
};

struct AAnimTrack //: public TTCBKey
{
	//DWORD			iType;
	int				iTick;
	Quat			qValue;
	Point3			vValue;
	Point3			intan;		//Inconming Tangent Vector
	Point3			outtan;		//Outgoing Tanglent Vector
	Point3			inLength;	//The in length of the horizontal handle
	Point3			outLength;	//The out length of the horizontal handle
};

struct AMesh
{
	TSTR				m_strNodeName;
	TSTR				m_strParentName;
	vector<AAnimTrack>	m_PosTrack;
	vector<AAnimTrack>	m_RotTrack;
	vector<AAnimTrack>	m_SclTrack;
	vector<AAnimTrack>	m_VisTrack;
	
	vector<ATri>		m_TriList;
	vector<AMesh*>		m_pSubMesh;

	OBJECTCLASSTYPE		m_ClassType;
	Box3				m_Box;
	D3D_MATRIX			m_matWorld;
	int					m_iNumVertex;
	int					m_iNumFace;
	int					m_iMtrlRef;

	AMesh()
	{
		m_iMtrlRef = -1;
		m_iNumFace = 0;
		m_ClassType = CLASS_GEOM;
	};
	~AMesh()
	{
		m_PosTrack.clear();
		m_RotTrack.clear();
		m_SclTrack.clear();
		m_VisTrack.clear();
		m_TriList.clear();
	};
};


struct AScene
{
	int iVersion;		// 파일 버전
	int	iFirstFrame;    // 시작 프레임
	int	iLastFrame;     // 마지막 프레임
	int	iFrameSpeed;    // 1초당 프레임 개수(30)
	int	iTickPerFrame;  // 1프레임의 틱 값(160)
	int iNumMesh;		// 메쉬오브젝트 개수
	int iMaxWeight;		// 정점 당 가중치
	int iBindPose;		// 바인딩 포즈 에니메이션 여부
	//int iMtrlCnt;
	//int iObjCnt;
};



class MaxGlobal
{
public:
	Interface*		m_pInterface;		// 맥스 인터페이스	
	INode*			m_pRootNode;
	Interval		m_Interval;		// 프레임 정보
	AScene			m_Scene;		// 씬 정보 저장
	
	MatrixMapMGR	m_MatrixMap;

	vector<Mtl*>	m_pMtlList;		//매터리얼 리스트	
	
	vector<INode*>	m_pObjList;

	TCHAR			m_tmpBuffer[MAX_PATH];//임시버퍼
public:
	bool			Initialize(Interface*	p3dsMax = NULL);
	void			PreProcess(INode* pNode);
	void			AddObject(INode* pNode);
	void			AddMtl(INode* pNode);// 중복되지 않고 매터리얼 추가
	int				GetMtlRef(Mtl* pMtl); // pMtl이 저장된 인덱스를 반환한다.
	void			SetBindPose(bool bBindPose = false) { m_Scene.iBindPose = bBindPose; }
	int				GetIndex(const TCHAR* strNodeName);

public:
	void			DumpMatrix3(Matrix3* m, D3D_MATRIX& mat);
	void			DumpPoint3(Point3& pDest, Point3& pSrc);
	bool			TMNegParity(Matrix3 &m);
	bool			EqualPoint2(Point2 p1, Point2 p2);
	bool			EqualPoint3(Point3 p1, Point3 p2);
	bool			EqualPoint4(Point4 p1, Point4 p2);
	TCHAR*			FixupName(MSTR name);

public:
	MaxGlobal();
	virtual ~MaxGlobal();
};

