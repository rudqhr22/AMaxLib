#include "MaxGlobal.h"

int MaxGlobal::GetIndex(const TCHAR* strNodeName)
{
	return m_MatrixMap.GetID(strNodeName);
}

bool MaxGlobal::Initialize(Interface* pMax)
{
	m_pInterface = pMax;
	m_pRootNode = pMax->GetRootNode();

	if (pMax == NULL && m_pInterface == NULL)
	{
		return false;
	}


	// Scene 정보얻기
	m_Interval = m_pInterface->GetAnimRange();

	memset(&m_Scene, 0, sizeof(m_Scene));

	m_Scene.iVersion = 1000;
	m_Scene.iFirstFrame = m_Interval.Start() / GetTicksPerFrame();
	m_Scene.iLastFrame = m_Interval.End() / GetTicksPerFrame();
	m_Scene.iFrameSpeed = GetFrameRate();
	m_Scene.iTickPerFrame = GetTicksPerFrame();

	//마테리얼 카운터 추가
	//m_Scene.iMtrlCnt = mtlsize
	//오브젝트 카운터 추가
	//  최상위 INode 얻기

	PreProcess(m_pInterface->GetRootNode());




	m_Scene.iNumMesh = m_MatrixMap.Count();//  I_MATRIXMAP.Count();
	m_Scene.iMaxWeight = 1;
	return true;
}


void MaxGlobal::PreProcess(INode* pNode)
{
	if (pNode == NULL) return;

	AddObject(pNode);
	AddMtl(pNode);

	int iNumChildren = pNode->NumberOfChildren();	// pNode의 자식 노드 개수를 반환함.
	for (int iCnt = 0; iCnt < iNumChildren; iCnt++)
	{
		INode* pChild = pNode->GetChildNode(iCnt); // iCnt 번째의 자식 노드를 반환함.
		PreProcess(pChild);
	}
}

void MaxGlobal::AddObject(INode* pNode)
{
	ObjectState os = pNode->EvalWorldState(m_Interval.Start());

	if (os.obj)
	{
		// 타켓 클래스 오브젝트는 불필요한 오브젝트
		if (os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
		{
			return;
		}
		switch (os.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
		case HELPER_CLASS_ID:
		{
			int iIndex = m_MatrixMap.Add(pNode->GetName());
			TSTR parentName = pNode->GetParentNode()->GetName();
			m_MatrixMap.GetPtr(iIndex)->SetNode(pNode, m_Interval.Start(),m_MatrixMap.GetPtr(parentName));
		}
		break;
		}
	}
}
void MaxGlobal::AddMtl(INode* pNode)
{
	Mtl* pMtl = pNode->GetMtl();
	if (pMtl != NULL)
	{
		bool bAdd = true;
		for (int iCnt = 0; iCnt < m_pMtlList.size(); iCnt++)
		{
			if (m_pMtlList[iCnt] == pMtl) // 중복 제거
			{
				bAdd = false;
				break;
			}
		}
		if (bAdd)
		{
			m_pMtlList.push_back(pMtl);
		}
	}
}

int MaxGlobal::GetMtlRef(Mtl* pMtl)
{
	for (int iMtl = 0; iMtl < m_pMtlList.size(); iMtl++)
	{
		if (m_pMtlList[iMtl] == pMtl)
		{
			return iMtl;
		}
	}
	return  -1;
}


bool		MaxGlobal::TMNegParity(Matrix3 &tm)
{
	Point3 vDir = CrossProd(tm.GetRow(0), tm.GetRow(1));

	return float (DotProd(vDir, tm.GetRow(2)) < 0.0f) ? true : false;
}


TCHAR* MaxGlobal::FixupName(MSTR name)
{
	memset(m_tmpBuffer, 0, sizeof(TCHAR)*MAX_PATH);

	TCHAR* cPtr;
	_tcscpy(m_tmpBuffer, name);
	cPtr = m_tmpBuffer;

	while (*cPtr)
	{
		if (*cPtr == '"')		*cPtr = SINGLE_QUOTE;
		else if (*cPtr == ' ' || *cPtr <= CTL_CHARS)
			*cPtr = _T('_');
		cPtr++;
	}
	return m_tmpBuffer;
}

void MaxGlobal::DumpMatrix3(Matrix3* m, D3D_MATRIX& mat)
{
	Point3 row;
	
	row = m->GetRow(0);
	mat._11 = row.x; mat._13 = row.y; mat._12 = row.z;
	
	row = m->GetRow(2);
	mat._21 = row.x; mat._23 = row.y; mat._22 = row.z;
	
	row = m->GetRow(1);
	mat._31 = row.x; mat._33 = row.y; mat._32 = row.z;
	
	row = m->GetRow(3);
	mat._41 = row.x; mat._43 = row.y; mat._42 = row.z;

	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
}


void			MaxGlobal::DumpPoint3(Point3& pDest, Point3& pSrc)
{
	pDest.x = pSrc.x;
	pDest.y = pSrc.z;
	pDest.z = pSrc.y;

}




bool MaxGlobal::EqualPoint2(Point2 p1, Point2 p2)
{
	if (fabs(p1.x - p2.x) > ALMOST_ZERO)
	{
		return false;
	}
	if (fabs(p1.y - p2.y) > ALMOST_ZERO)
	{
		return false;
	}
	return true;
}
bool MaxGlobal::EqualPoint3(Point3 p1, Point3 p2)
{
	if (fabs(p1.x - p2.x) > ALMOST_ZERO)
	{
		return false;
	}
	if (fabs(p1.y - p2.y) > ALMOST_ZERO)
	{
		return false;
	}
	if (fabs(p1.z - p2.z) > ALMOST_ZERO)
	{
		return false;
	}

	return true;
}
bool MaxGlobal::EqualPoint4(Point4 p1, Point4 p2)
{
	if (fabs(p1.x - p2.x) > ALMOST_ZERO)
	{
		return false;
	}
	if (fabs(p1.y - p2.y) > ALMOST_ZERO)
	{
		return false;
	}
	if (fabs(p1.z - p2.z) > ALMOST_ZERO)
	{
		return false;
	}
	if (fabs(p1.w - p2.w) > ALMOST_ZERO)
	{
		return false;
	}
	return true;
}


MaxGlobal::MaxGlobal()
{
}


MaxGlobal::~MaxGlobal()
{
}


//애니메이션 있는지 없는지 체크 하는 함수
//void CheckForAnimation(INode* pNode, BOOL& bPos, BOOL& bRot, BOOL& bScale)
//{
//	AffineParts ap;
//	Point3 firstPos, firstRotAxis, firstScaleFactor;
//	float firstRotAngle;
//	GetDecompAffine(m_Interval.Start(), pNode, &ap, &firstRotAxis, &firstRotAngle);		//분해하는 함수
//
//
//	Matrix3 tm = node0 > GetNOdeTM(m_Interval.Start());
//	Matrix3 tmParent = node->GetParentTM(m_Interval.Start());
//	tm = tm * Inverse(tmParent);
//	AffineParts ap;
//	decomp_affine(tm, ap);
//
//
//	TimeValue start = m_Interval.Start() + GetTicksPerFrame();	//1프레임 진행 시킨 상태에서 진행
//	TimeValue end = m_Interval.End();
//
//	for (TimeValue t = start; t <= end; t += m_iDeltaTick) //m_iDeltaTick or GetRocksPerFrame()
//	{
//		Matrix3 tmFrame = node0 > GetNOdeTM(m_Interval.Start());
//		Matrix3 tmFrameParent = node->GetParentTM(m_Interval.Start());
//		tmFrame = tmFrame * Inverse(tmFrameParent);
//		AffineParts apFrame;
//		decomp_affine(tm, apFrame);
//
//
//		if (bPos==false)
//		{
//			if (ap.t != apFranme.t)
//			{
//				false;
//			}
//
//			if (!EqualPoint3(ap.t, firstPos))
//			{
//				bPos = TRUE;
//			}
//		}
//		if (bRot == false)
//		{
//			if (ap.u != apFranme.t)
//			{
//				false;
//			}
//
//			if (fabs(rotAngle - firstRotAngle) > ALMOST_ZERO)
//			{
//				bRot = TRUE;
//			}
//			else if (!EqualPoint3(rotAxis, firstRotAxis))
//			{
//				bRot = TRUE;
//			}
//		}
//		if (bScale == false)
//		{
//			if (ap.q != apFranme.t)
//			{
//				false;
//			}
//
//			if (!EqualPoint3(ap.k, firstScaleFactor))
//			{
//				bScale = TRUE;
//			}
//		}
//		if (bPos && bRot && bScale)		break;
//	}
//	return bPos || bRot || bScale;
//}



//BOOL bPosAnim;
//BOOL bRotAnim;
//BOOL bScaleAnim;
//for (int iNOde = 0; iNOde <m_objList, size())
//
//	if (CheckForAnimation(pNode, bPosAnim, bRotAnim, bScaleAnim))
//	{
//		if (bPosAnim)
//		{
//			DumpPosSample(pNode, pMesh);
//		}
//		if (bRotAnim)
//		{
//			DumpRotSample(pNode, pMesh);
//		}
//		if (bScaleAnim)
//		{
//			DumpScaleSample(pNode, pMesh);
//		}
//	}