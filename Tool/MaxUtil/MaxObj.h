#pragma once
#include "MaxMtrl.h"

//��ŸƮ ���� ���� ���� ���͸� �ִϸ��̼�
//��ȭ�� ������ ���� CheckAnimation();
//
//


class MaxObj : public MaxMtrl
{
public:
	vector<AMesh>					m_MeshList;
public:
	void		AddObject();
	void		AddObject(MatrixMap* pPoint, int iObj);//������Ʈ ����Ʈ���� ���� ���
													 // ������Ʈ ���� 
	void		GetNodeHeader(INode* node, AMesh&	pMesh);
	void		GetNodeTM(INode* node, AMesh&	pMesh);

	void		GetMesh(INode* node, AMesh&	pMesh);
	Point3		GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);
	TriObject*	GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt);


public:
	int		m_iDeltaTick;
	// ���ø�
	void	GetAnimKeys(INode* pNode, AMesh* pMesh);
	void	DumpScaleSample(INode* node, AMesh* Mesh);
	void	DumpRotSample(INode* node, AMesh* Mesh);
	void	DumpPosSample(INode* node, AMesh* Mesh);
	BOOL	CheckForAnimation(INode* node, BOOL& bPos, BOOL& bRot, BOOL& bScale);
	bool	GetDecompAffine(TimeValue t, INode* pNode, AffineParts* ap, Point3* rotAxis = NULL, float* rotAngle = NULL);
	// Ű ������ 
	void	DumpFloatKeys(Control* cont, AMesh* pMesh);


public:
	MaxObj();
	virtual ~MaxObj();
};



//classTMEsh
//def���ؽ���Ʈ��
//�ε�������Ʈ
//def�ε��� ����Ʈ
//int ref
//
