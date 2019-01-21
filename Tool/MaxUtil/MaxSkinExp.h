#pragma once
#include "MaxObj.h"
#include <fstream>

#define MAX_WEIGHT_BIPED	8
// ������������������������������������������������������������������
// ������ 1�� �� �Ҵ��ʿ�.
// ������������������������������������������������������������������
struct BipedVertex
{
	int					m_dwNumWeight;	//��ũ�Ǿ� �մ� ����� ����
	vector<BYTE>		m_BipIDList;	//��ũ�Ǿ� �մ� ����� Ű ��
	vector<float>		m_fWeightList;	//��ũ�Ǿ� �մ� ����� ����ġ ��
};

// ������������������������������������������������������������������
//  ���̴� ����( ����ġ 8�� ���� ��� )
// ������������������������������������������������������������������
struct PNCT5_VERTEX
{
	Point3		p;
	Point3		n;
	Point4		c;
	Point2		t;
	float		w1[4];	// W0, fW1, fW2, fW3;
	float		i1[4];	// I0, I1, I2, I3
	float		w2[4];	// W4, fW5, fW6, fNumWeight;	//fNumWeight ī���Ͱ� ���� 8�� �ȵ�����
	float		i2[4];	// I4, I5, I6, I7
};
struct ASkinTri : public ATri
{
	PNCT5_VERTEX	m_vVertex[3];
};
struct ASkinMesh : public AMesh
{
	vector<ASkinTri>		m_SkinTriList;
	vector<ASkinMesh*>		m_pSkinSubMesh;

	// �ؽ��� ����( �ؽ��� �� ���� )
	int						m_iNumTex;
	int						m_iTexType[13];
	TCHAR					m_szTexName[13][30];
	ASkinMesh() {};
	virtual ~ASkinMesh() {};
};

class MaxSkinExp : public ASingleton< MaxSkinExp>
{
private:
	friend class ASingleton<MaxSkinExp>;

public:
	AScene		m_Scene;
	fstream		m_FP;
	// ��������ġ ����Ʈ
	vector<BipedVertex>		m_BipedList;

	// �޽� ���̽� ����Ʈ
	vector<ASkinMesh*>		m_SkinMeshList;

public:
	bool		Init();
	bool		SkinExport();
	bool		Release();

	Modifier*	FindModifier(INode *nodePtr, Class_ID classID);
	void		SetBipedInfo(INode* node);

	void		ExportPhysiqueData(INode* node, Modifier	*phyMod);
	void		ExportSkinData(INode* node, Modifier	*skinMod);
	bool		ExportMesh(FILE* fp, ASkinMesh* pMesh);

	void		SetVertexBiped(INode* node, Face*	face,
		int v0, int v1, int v2,
		ASkinTri* pTri);

	bool		GetNodeInfo(INode* node, TimeValue t);
	// ������͸��� ���ø� ����Ͽ� ���̽� ����Ʈ ����

	void		GenerateGroup(INode* node, Mesh *mesh, ASkinMesh* pSkinMesh);

	void		LoadMaterial(ASkinMesh*	pSkinMesh, Mtl* mtl);

	void		SetTriangle(INode* node, Mesh *mesh,ASkinMesh* pSkinMesh, ASkinMesh*	pSumMesh = NULL, int iMtrl = 0);

	int			GetMapID(Class_ID cid, int subNo);

	int			IsEqulVertexList(vector<PNCT5_VERTEX>&  VertexArray, PNCT5_VERTEX& Vertex);



public:
	MaxSkinExp();
	virtual ~MaxSkinExp();
};
#define I_SKINEXP MaxSkinExp::GetInstance()
