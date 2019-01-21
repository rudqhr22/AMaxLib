#pragma once
#include "MaxMtrl.h"

//스타트 부터 엔드 부터 빠귀면 애니메이션
//변화가 없으면 정적 CheckAnimation();
//
//


class MaxObj : public MaxMtrl
{
public:
	vector<AMesh>					m_MeshList;
public:
	void		AddObject();
	void		AddObject(MatrixMap* pPoint, int iObj);//오브젝트 리스트에서 정보 얻기
													 // 오브젝트 정보 
	void		GetNodeHeader(INode* node, AMesh&	pMesh);
	void		GetNodeTM(INode* node, AMesh&	pMesh);

	void		GetMesh(INode* node, AMesh&	pMesh);
	Point3		GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);
	TriObject*	GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt);


public:
	int		m_iDeltaTick;
	// 샘플링
	void	GetAnimKeys(INode* pNode, AMesh* pMesh);
	void	DumpScaleSample(INode* node, AMesh* Mesh);
	void	DumpRotSample(INode* node, AMesh* Mesh);
	void	DumpPosSample(INode* node, AMesh* Mesh);
	BOOL	CheckForAnimation(INode* node, BOOL& bPos, BOOL& bRot, BOOL& bScale);
	bool	GetDecompAffine(TimeValue t, INode* pNode, AffineParts* ap, Point3* rotAxis = NULL, float* rotAngle = NULL);
	// 키 프레임 
	void	DumpFloatKeys(Control* cont, AMesh* pMesh);


public:
	MaxObj();
	virtual ~MaxObj();
};



//classTMEsh
//def버텍스리트슽
//인덱스리스트
//def인덱스 리스트
//int ref
//
