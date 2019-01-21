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
	int iVersion;		// ���� ����
	int	iFirstFrame;    // ���� ������
	int	iLastFrame;     // ������ ������
	int	iFrameSpeed;    // 1�ʴ� ������ ����(30)
	int	iTickPerFrame;  // 1�������� ƽ ��(160)
	int iNumMesh;		// �޽�������Ʈ ����
	int iMaxWeight;		// ���� �� ����ġ
	int iBindPose;		// ���ε� ���� ���ϸ��̼� ����
	//int iMtrlCnt;
	//int iObjCnt;
};



class MaxGlobal
{
public:
	Interface*		m_pInterface;		// �ƽ� �������̽�	
	INode*			m_pRootNode;
	Interval		m_Interval;		// ������ ����
	AScene			m_Scene;		// �� ���� ����
	
	MatrixMapMGR	m_MatrixMap;

	vector<Mtl*>	m_pMtlList;		//���͸��� ����Ʈ	
	
	vector<INode*>	m_pObjList;

	TCHAR			m_tmpBuffer[MAX_PATH];//�ӽù���
public:
	bool			Initialize(Interface*	p3dsMax = NULL);
	void			PreProcess(INode* pNode);
	void			AddObject(INode* pNode);
	void			AddMtl(INode* pNode);// �ߺ����� �ʰ� ���͸��� �߰�
	int				GetMtlRef(Mtl* pMtl); // pMtl�� ����� �ε����� ��ȯ�Ѵ�.
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

