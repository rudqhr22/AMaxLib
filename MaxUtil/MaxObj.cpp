#include "MaxObj.h"


void		MaxObj::AddObject()
{
	m_MeshList.resize(m_MatrixMap.Count());
	for (int iObj = 0; iObj < m_MatrixMap.Count(); iObj++)
	{
		MatrixMap *pPoint = (MatrixMap *)m_MatrixMap.GetPtr(iObj);
		if (pPoint == nullptr) break;
		AddObject(pPoint, iObj);
	}

}



void		MaxObj::AddObject(MatrixMap* pPoint, int iObj)
{

	m_MeshList[iObj].m_ClassType = pPoint->m_ClassType;
	GetNodeHeader(pPoint->m_pINode, m_MeshList[iObj]);	// ����� �̸��� ��´�.
	GetNodeTM(pPoint->m_pINode, m_MeshList[iObj]);		// ����� ����� ��´�.

	if (pPoint->m_ClassType == CLASS_GEOM)
	{
		GetMesh(pPoint->m_pINode, m_MeshList[iObj]);	// ���� ������ ��´�.		
	}
	else
	{
		Object* helperObj = pPoint->m_pObj;
		Matrix3		oTM = pPoint->m_pINode->GetObjectTM(m_Interval.Start());			//TM���  ,�ð���� ��ŸƮ
		helperObj->GetDeformBBox(m_Interval.Start(), m_MeshList[iObj].m_Box, &oTM);
		
		//INode* pParent = pPoint->m_pINode->GetParentNode();
		m_MeshList[iObj].m_Box.pmax = m_MeshList[iObj].m_Box.pmax * Inverse(pPoint->m_pINode->GetNodeTM(m_Interval.Start()));
		m_MeshList[iObj].m_Box.pmin = m_MeshList[iObj].m_Box.pmin * Inverse(pPoint->m_pINode->GetNodeTM(m_Interval.Start()));
	}

	GetAnimKeys(pPoint->m_pINode, &m_MeshList[iObj]);	// ���ϸ��̼� ������ ��´�.
}

void MaxObj::GetAnimKeys(INode* pNode,AMesh* pMesh)
{
	BOOL bPosAnim;
	BOOL bRotAnim;
	BOOL bScaleAnim;

	m_iDeltaTick = m_Scene.iTickPerFrame;

	if (CheckForAnimation(pNode, bPosAnim, bRotAnim, bScaleAnim))
	{
		if (bPosAnim)
		{
			DumpPosSample(pNode, pMesh);
		}
		if (bRotAnim)
		{
			DumpRotSample(pNode, pMesh);
		}
		if (bScaleAnim)
		{
			DumpScaleSample(pNode, pMesh);
		}
	}

	// Export the visibility track
	//Control* visCont = pNode->GetVisController();
	//if (visCont)
	//{
	//	DumpFloatKeys(visCont, pMesh);
	//}
}


bool MaxObj::GetDecompAffine(TimeValue t, INode* pNode, AffineParts* ap, Point3* rotAxis, float* rotAngle)
{
	Matrix3 tm = pNode->GetNodeTM(t) * Inverse(pNode->GetParentTM(t));
	decomp_affine(tm, ap);

	//Point3 vRotAxis;
	//float  fRotAngle;
	if (rotAngle != NULL && rotAngle != NULL)
	{
		AngAxisFromQ(ap->q, rotAngle, *rotAxis);
	}
	return true;
}
BOOL MaxObj::CheckForAnimation(INode* pNode, BOOL& bPos, BOOL& bRot, BOOL& bScale)
{
	AffineParts ap;
	Point3 firstPos, firstRotAxis, firstScaleFactor;
	float firstRotAngle;
	GetDecompAffine(m_Interval.Start(), pNode, &ap, &firstRotAxis, &firstRotAngle);
	firstPos = ap.t;
	firstScaleFactor = ap.k;

	TimeValue start = m_Interval.Start() + GetTicksPerFrame();
	TimeValue end = m_Interval.End();

	bPos = bRot = bScale = FALSE;
	float rotAngle;
	Point3 rotAxis;

	for (TimeValue t = start; t <= end; t += m_iDeltaTick)
	{
		GetDecompAffine(t, pNode, &ap, &rotAxis, &rotAngle);

		if (!bPos)
		{
			if (!EqualPoint3(ap.t, firstPos))
			{
				bPos = TRUE;
			}
		}
		if (!bRot)
		{
			if (fabs(rotAngle - firstRotAngle) > ALMOST_ZERO)
			{
				bRot = TRUE;
			}
			else if (!EqualPoint3(rotAxis, firstRotAxis))
			{
				bRot = TRUE;
			}
		}
		if (!bScale)
		{
			if (!EqualPoint3(ap.k, firstScaleFactor))
			{
				bScale = TRUE;
			}
		}

		if (bPos && bRot && bScale)		break;
	}
	return bPos || bRot || bScale;
}

void MaxObj::DumpPosSample(INode* pNode, AMesh* pMesh)
{
	TimeValue start = m_Interval.Start();
	TimeValue end = m_Interval.End();
	AffineParts ap;

	for (TimeValue t = start; t <= end; t += m_iDeltaTick)
	{
		GetDecompAffine(t, pNode, &ap);

		AAnimTrack pAnim;
		pAnim.iTick = t;
		pAnim.vValue.x = ap.t.x;
		pAnim.vValue.y = ap.t.z;
		pAnim.vValue.z = ap.t.y;
		pMesh->m_PosTrack.push_back(pAnim);
	}
}

void MaxObj::DumpRotSample(INode* pNode, AMesh* pMesh)
{
	TimeValue start = m_Interval.Start();
	TimeValue end = m_Interval.End();

	AffineParts ap;
	for (TimeValue t = start; t <= end; t += m_iDeltaTick)
	{
		GetDecompAffine(t, pNode, &ap);

		AAnimTrack pAnim;
		pAnim.iTick = t;
		pAnim.qValue.x = ap.q.x;
		pAnim.qValue.y = ap.q.z;
		pAnim.qValue.z = ap.q.y;
		pAnim.qValue.w = ap.q.w;
		pMesh->m_RotTrack.push_back(pAnim);
	}
}

void MaxObj::DumpScaleSample(INode* pNode, AMesh* pMesh)
{
	TimeValue start = m_Interval.Start();
	TimeValue end = m_Interval.End();

	AffineParts ap;
	for (TimeValue t = start; t <= end; t += m_iDeltaTick)
	{
		GetDecompAffine(t, pNode, &ap);

		AAnimTrack Anim;
		Anim.iTick = t;
		Anim.vValue.x = ap.k.x;
		Anim.vValue.y = ap.k.z;
		Anim.vValue.z = ap.k.y;
		Anim.qValue.x = ap.u.x;
		Anim.qValue.y = ap.u.z;
		Anim.qValue.z = ap.u.y;
		Anim.qValue.w = ap.u.w;
		pMesh->m_SclTrack.push_back(Anim);
	}
}



void		MaxObj::GetNodeHeader(INode* node, AMesh&	pMesh)
{
	pMesh.m_strNodeName = FixupName(node->GetName());
	INode* parent = node->GetParentNode();
	if (parent && (!parent->IsRootNode()))
	{
		pMesh.m_strParentName = FixupName(parent->GetName());
	}
}


void		MaxObj::GetNodeTM(INode* node, AMesh&	pMesh)
{
	//Matrix3 ���� ����ü�� DumpMatrix�� �� �� �־�� �Ѵ�.
	Matrix3 g_matWorld = node->GetNodeTM(m_Interval.Start());

	DumpMatrix3(&g_matWorld, pMesh.m_matWorld);

}

void		MaxObj::GetMesh(INode* node, AMesh&	pMesh)
{
	//��������� ��ġ�� ���Ŀ� �����Ͱ� ����
	Matrix3 tm = node->GetObjTMAfterWSM(m_Interval.Start());

	//���� ����̸� ���� �������Ű�
	//������ �ƴϸ� ���� ���� 
	//tm�� ���ϸ� ����� ���� �ְ� ���� ���̴�.

	// �۾��� �� �۾��Ϸ�� ������������ ����� �׼��������ν� �������� ����� �����ش�.
	ObjectState os = node->EvalWorldState(m_Interval.Start());		//���͹� ��ŸƮ�� �����ϴ� �۾�

	if (!os.obj || os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID)	//�� ��������Ʈ
	{

		return;
	}

	BOOL needDel;
	TriObject* tri = GetTriObjectFromNode(node, m_Interval.Start(), needDel);

	if (!tri)
	{

		return;
	}



	Mesh* mesh = &(tri->GetMesh());
	// �ٿ�� �ڽ�
	mesh->buildBoundingBox();

	Box3 box = mesh->getBoundingBox(&tm);
	DumpPoint3(pMesh.m_Box.pmax, box.pmax);
	DumpPoint3(pMesh.m_Box.pmin, box.pmin);

	pMesh.m_iNumFace = mesh->getNumFaces();

	BOOL negScale = TMNegParity(tm);	// UV�� �� ���� ���� Ȯ��
	int v0, v1, v2;
	if (negScale) 
	{ v0 = 2;		v1 = 1;		v2 = 0; }
	else			{ v0 = 0;		v1 = 1;		v2 = 2; }

	pMesh.m_TriList.resize(pMesh.m_iNumFace);
	for (int iFace = 0; iFace < pMesh.m_iNumFace; iFace++)
	{
		Matrix3 pInverse = Inverse(node->GetNodeTM(m_Interval.Start()));
		Point3 v;

		if (mesh->getNumVerts() > 0)
		{
			/*			vv = tm * mesh->verts[   mesh->faces[iFace].v[v0]    ];
			v = mesh->verts[   mesh->faces[iFace].v[v0]    ] * tm;	*/
	
			v = mesh->verts[mesh->faces[iFace].v[v0]] * tm * pInverse;
			DumpPoint3(pMesh.m_TriList[iFace].v[0].p, v);

			v = mesh->verts[mesh->faces[iFace].v[v2]] * tm * pInverse;
			DumpPoint3(pMesh.m_TriList[iFace].v[1].p, v);

			v = mesh->verts[mesh->faces[iFace].v[v1]] * tm * pInverse;
			DumpPoint3(pMesh.m_TriList[iFace].v[2].p, v);
		}

		//T��
		if (mesh->getNumTVerts() > 0)
		{
			pMesh.m_TriList[iFace].v[0].t.x = mesh->tVerts[mesh->tvFace[iFace].t[v0]].x;
			pMesh.m_TriList[iFace].v[0].t.y = 1.0f - mesh->tVerts[mesh->tvFace[iFace].t[v0]].y;
			pMesh.m_TriList[iFace].v[1].t.x = mesh->tVerts[mesh->tvFace[iFace].t[v2]].x;
			pMesh.m_TriList[iFace].v[1].t.y = 1.0f - mesh->tVerts[mesh->tvFace[iFace].t[v2]].y;
			pMesh.m_TriList[iFace].v[2].t.x = mesh->tVerts[mesh->tvFace[iFace].t[v1]].x;
			pMesh.m_TriList[iFace].v[2].t.y = 1.0f - mesh->tVerts[mesh->tvFace[iFace].t[v1]].y;
		}


		//�÷�
		if (mesh->getNumVertCol() > 0)
		{
			pMesh.m_TriList[iFace].v[0].c.x = mesh->vertCol[mesh->vcFace[iFace].t[v0]].x;
			pMesh.m_TriList[iFace].v[0].c.y = mesh->vertCol[mesh->vcFace[iFace].t[v0]].y;
			pMesh.m_TriList[iFace].v[0].c.z = mesh->vertCol[mesh->vcFace[iFace].t[v0]].z;
			pMesh.m_TriList[iFace].v[0].c.w = 1.0f;
			pMesh.m_TriList[iFace].v[1].c.x = mesh->vertCol[mesh->vcFace[iFace].t[v2]].x;
			pMesh.m_TriList[iFace].v[1].c.y = mesh->vertCol[mesh->vcFace[iFace].t[v2]].y;
			pMesh.m_TriList[iFace].v[1].c.z = mesh->vertCol[mesh->vcFace[iFace].t[v2]].z;
			pMesh.m_TriList[iFace].v[1].c.w = 1.0f;
			pMesh.m_TriList[iFace].v[2].c.x = mesh->vertCol[mesh->vcFace[iFace].t[v1]].x;
			pMesh.m_TriList[iFace].v[2].c.y = mesh->vertCol[mesh->vcFace[iFace].t[v1]].y;
			pMesh.m_TriList[iFace].v[2].c.z = mesh->vertCol[mesh->vcFace[iFace].t[v1]].z;
			pMesh.m_TriList[iFace].v[2].c.w = 1.0f;
		}
		else
		{
			pMesh.m_TriList[iFace].v[0].c = Point4(1, 1, 1, 1);
			pMesh.m_TriList[iFace].v[1].c = Point4(1, 1, 1, 1);
			pMesh.m_TriList[iFace].v[2].c = Point4(1, 1, 1, 1);
		}

		// ���� �븻 ��� 
		mesh->buildNormals();	//�� ���� �ؾ� �븻�� ���� ���� �ִ�.

		int  vert = mesh->faces[iFace].getVert(v0);		//���̽��� ID�� �ʿ䰡 �ϴ�.
		Point3 vn = GetVertexNormal(mesh, iFace, mesh->getRVertPtr(vert));				//������ �ּҰ����� �Ѱܼ� �����ͷ� ����?
		DumpPoint3(pMesh.m_TriList[iFace].v[0].n, vn);

		vert = mesh->faces[iFace].getVert(v2);			//2��
		vn = GetVertexNormal(mesh, iFace, mesh->getRVertPtr(vert));
		DumpPoint3(pMesh.m_TriList[iFace].v[1].n, vn);

		vert = mesh->faces[iFace].getVert(v1);			//1��
		vn = GetVertexNormal(mesh, iFace, mesh->getRVertPtr(vert));
		DumpPoint3(pMesh.m_TriList[iFace].v[2].n, vn);

		// ���� ���͸��� �ε���
		pMesh.m_TriList[iFace].iSubIndex = mesh->faces[iFace].getMatID();
	}
	if (needDel)
	{
		delete tri;
	}
	pMesh.m_iMtrlRef = GetMtlRef(node->GetMtl());
}

//RVertex ���� -> ���� ���ؽ� -> ������ ��� �ϴ����� ���� �޶���?
//�븻������
Point3		MaxObj::GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{

	Face* f = &mesh->faces[faceNo];				//���̽� �����͸� ����
	DWORD smGroup = f->smGroup;
	int numNormals = rv->rFlags & NORCT_MASK;
	Point3 vertexNormal;

	if (rv->rFlags & SPECIFIED_NORMAL)				//���̽��� �پ��ִ� �����̱׷� - �� �׷츸 ���� �ϰڴ�
	{
		vertexNormal = rv->rn.getNormal();
	}
	else if (numNormals  && smGroup)
	{
		if (numNormals == 1)
		{
			vertexNormal = rv->rn.getNormal();
		}
		else     //�븻�� 1���� �ƴ� ���
		{
			for (int i = 0; i < numNormals; i++)
			{
				if (rv->ern[i].getSmGroup() & smGroup)
				{
					vertexNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else
	{
		vertexNormal = mesh->getFaceNormal(faceNo);			//���̽� �븻�� ���ؼ�
	}
	return vertexNormal;
}


TriObject*	MaxObj::GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject *tri = (TriObject *)obj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));
		if (obj != (Object*)tri) deleteIt = TRUE;
		return tri;
	}
	else
	{
		return NULL;
	}

}

MaxObj::MaxObj()
{
}


MaxObj::~MaxObj()
{
}


