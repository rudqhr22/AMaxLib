#include "MaxSkinExp.h"
#include "MaxWriter.h"

//TM 및 인버스 TM 재확인 및
//SkinExport() 수치 안들어 가는 거 확인

bool		MaxSkinExp::Init()
{

	return true;
}

bool		MaxSkinExp::SkinExport()
{
	int iSkinMeshCount = m_SkinMeshList.size();
	if (iSkinMeshCount <= 0)
	{
		MessageBox(NULL, _T("출력할 정보가 없습니다."), _T("파일생성에러"), MB_ICONERROR);
		return FALSE;
	}

	TCHAR  szFile[256] = { 0, };
	_tcscpy(szFile, I_WRITER.SaveFileDlg(_T("skm"), _T("TSkinExport")));

	FILE	*fp;
	fp = _tfopen(szFile, _T("wb"));
	if (!fp)
	{
		MessageBox(NULL, _T("파일을 생성하지 못했습니다."), _T("파일생성에러"), MB_ICONERROR);
		return FALSE;
	}


	m_Scene.iNumMesh = m_SkinMeshList.size();
	fwrite(&m_Scene, sizeof(AScene), 1, fp);

	for (auto iter = m_SkinMeshList.begin(); iter != m_SkinMeshList.end(); iter++)
	{
		ASkinMesh* pMesh = (*iter);
		TCHAR szName[256] = { 0, };
		TCHAR szParentName[256] = { 0, };
		// 노드 이름
		int iLen = pMesh->m_strNodeName.Length();
		fwrite(&iLen, sizeof(int), 1, fp);
		if (iLen > 0)
		{
			_tcscpy(szName, pMesh->m_strNodeName);
			fwrite(szName, sizeof(TCHAR)*iLen, 1, fp);
		}

		// 노드 부모 이름
		iLen = pMesh->m_strParentName.Length();
		_tcscpy(szParentName, pMesh->m_strParentName);
		fwrite(&iLen, sizeof(int), 1, fp);
		if (iLen > 0)
		{
			fwrite(szParentName, sizeof(TCHAR)*iLen, 1, fp);
		}

		// 월드 행렬
		fwrite(&pMesh->m_matWorld, sizeof(D3D_MATRIX), 1, fp);

		// 서브매터리얼 개수
		int iRealSubMtrlConter = pMesh->m_pSubMesh.size();
		fwrite(&iRealSubMtrlConter, sizeof(int), 1, fp);

		if (iRealSubMtrlConter <= 0)
		{
			ExportMesh(fp, pMesh);
		}
		else
		{
			for (int iMesh = 0; iMesh < pMesh->m_pSubMesh.size(); iMesh++)
			{
				ASkinMesh* pSubMesh = pMesh->m_pSkinSubMesh[iMesh];
				ExportMesh(fp, pSubMesh);
			}
		}
	}

	// 드레스 포즈의 각 바이패드 행렬 출력
	MatrixMap* matIndex;
	D3D_MATRIX d3dMat;
	int iNumMatrix = I_WRITER.m_MatrixMap.Count();
	fwrite(&iNumMatrix, sizeof(int), 1, fp);

	for (int iMat = 0; iMat < iNumMatrix; iMat++)
	{
		matIndex = I_WRITER.m_MatrixMap.GetPtr(iMat);
		I_WRITER.DumpMatrix3(&matIndex->m_InvNodeTM, d3dMat);
		fwrite(&d3dMat, sizeof(D3D_MATRIX), 1, fp);
	}

	fclose(fp);
	Release();
	MessageBox(GetActiveWindow(), szFile, _T("Succeed!!"), MB_OK);




	//int iSkinMeshCount = m_SkinMeshList.size();
	//if (iSkinMeshCount <= 0)
	//{
	//	MessageBox(NULL, _T("출력할 정보가 없습니다."), _T("파일생성에러"), MB_ICONERROR);
	//	return FALSE;
	//}
	//TCHAR  szFile[256] = { 0, };
	//_tcscpy(szFile, I_WRITER.SaveFileDlg(_T("skm"), _T("TSkinExport")));
	//m_FP.open(szFile, ios::out);
	//if (!m_FP)
	//{
	//	MessageBox(NULL, _T("파일을 생성하지 못했습니다."), _T("파일생성에러"), MB_ICONERROR);
	//	return FALSE;
	//}
	//m_Scene.iNumMesh = m_SkinMeshList.size();
	//if (m_FP.is_open())
	//{
	//	m_FP << m_Scene.iFirstFrame << " "
	//		<< m_Scene.iLastFrame << " "
	//		<< m_Scene.iFrameSpeed << " "
	//		<< m_Scene.iTickPerFrame << " "
	//		<< m_Scene.iNumMesh << " "
	//		<< m_Scene.iMaxWeight << " "
	//		<< m_Scene.iBindPose << endl;
	//	for (auto iter = m_SkinMeshList.begin(); iter != m_SkinMeshList.end(); iter++)
	//	{
	//		ASkinMesh* pMesh = (*iter);
	//	//	TCHAR szName[256] = { 0, };
	//	//TCHAR szParentName[256] = { 0, };
	//		// 노드 이름
	//		int iLen = pMesh->m_strNodeName.Length();
	//		m_FP << iLen << " ";
	//		if (iLen > 0)
	//		{
	//			m_FP << pMesh->m_strNodeName << " ";
	//		}
	//		// 노드 부모 이름
	//		iLen = pMesh->m_strParentName.Length();
	//		
	//		if (iLen > 0)
	//		{
	//			m_FP << iLen << " ";
	//			m_FP << pMesh->m_strParentName;
	//		}
	//		else
	//		{
	//			m_FP << iLen << " ";
	//			m_FP << -1;
	//		}
	//		m_FP << endl;
	//		// 월드 행렬
	//		m_FP <<
	//			pMesh->m_matWorld._11<<" " << pMesh->m_matWorld._12 << " " << pMesh->m_matWorld._13 << " " << pMesh->m_matWorld._14 << " "<<
	//			pMesh->m_matWorld._21 << " " << pMesh->m_matWorld._22 << " " << pMesh->m_matWorld._23 << " " << pMesh->m_matWorld._24 << " " <<
	//			pMesh->m_matWorld._31 << " " << pMesh->m_matWorld._32<<" " << pMesh->m_matWorld._33 << " " << pMesh->m_matWorld._34 << " " <<
	//			pMesh->m_matWorld._41 << " " << pMesh->m_matWorld._42 << " " << pMesh->m_matWorld._43 << " " << pMesh->m_matWorld._44 << endl;
	//		//	// 서브매터리얼 개수
	//		int iRealSubMtrlConter = pMesh->m_pSubMesh.size();
	//		m_FP << iRealSubMtrlConter << endl;
	//		if (iRealSubMtrlConter <= 0)
	//		{
	//			ExportMesh(0, pMesh);
	//		}
	//		else
	//		{
	//			for (int iMesh = 0; iMesh < pMesh->m_pSubMesh.size(); iMesh++)
	//			{
	//				ASkinMesh* pSubMesh = pMesh->m_pSkinSubMesh[iMesh];
	//				ExportMesh(0, pSubMesh);
	//			}
	//		}
	//	}
	//	// 드레스 포즈의 각 바이패드 행렬 출력
	//	MatrixMap* matIndex;
	//	D3D_MATRIX d3dMat;
	//	int iNumMatrix = I_WRITER.m_MatrixMap.Count();
	//	m_FP << iNumMatrix << endl;
	//	for (int iMat = 0; iMat < iNumMatrix; iMat++)
	//	{
	//		matIndex = I_WRITER.m_MatrixMap.GetPtr(iMat);
	//		I_WRITER.DumpMatrix3(&matIndex->m_InvNodeTM, d3dMat);
	//		m_FP <<
	//			d3dMat._11 << " " << d3dMat._12 << " " << d3dMat._13 << " " << d3dMat._14 << " " <<
	//			d3dMat._21 << " " << d3dMat._22 << " " << d3dMat._23 << " " << d3dMat._24 << " " <<
	//			d3dMat._31 << " " << d3dMat._32 << " " << d3dMat._33 << " " << d3dMat._34 << " " <<
	//			d3dMat._41 << " " << d3dMat._42 << " " << d3dMat._43 << " " << d3dMat._44 << endl;
	//	}
	//}
	//m_FP.close();
	////fclose(fp);
	//Release();
	//MessageBox(GetActiveWindow(), szFile, _T("Succeed!!"), MB_OK);


	return true;
}

bool		MaxSkinExp::Release()
{
	m_BipedList.clear();
	for (auto iter = m_SkinMeshList.begin(); iter != m_SkinMeshList.end(); iter++)
	{
		ASkinMesh* pMesh = (*iter);
		if (pMesh->m_iMtrlRef > 0)
		{
			pMesh->m_pSkinSubMesh.clear();
			for (int imtl = 0; imtl < pMesh->m_iMtrlRef; imtl++)
			{
				//SAFE_DEL( pMesh->m_pSkinSubMesh[imtl] );

			}
		}
		SAFE_DEL(pMesh);
	}
	m_SkinMeshList.clear();
	return true;
}


Modifier*	MaxSkinExp::FindModifier(INode *nodePtr, Class_ID classID)
{
	Object *ObjectPtr = nodePtr->GetObjectRef();
	if (!ObjectPtr)
	{
		return NULL;
	}
	while (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID && ObjectPtr)
	{
		IDerivedObject *DerivedObjectPtr = (IDerivedObject *)(ObjectPtr);

		int StackIndex = 0;
		while (StackIndex < DerivedObjectPtr->NumModifiers())
		{
			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(StackIndex);

			if (ModifierPtr->ClassID() == classID)
			{
				return ModifierPtr;
			}

			StackIndex++;
		}
		ObjectPtr = DerivedObjectPtr->GetObjRef();
	}
	return NULL;
}

void		MaxSkinExp::SetBipedInfo(INode* node)
{
	Modifier	*phyMod = FindModifier(node, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B));
	Modifier	*skinMod = FindModifier(node, SKIN_CLASSID);

	if (phyMod)
	{
		ExportPhysiqueData(node, phyMod);
	}
	else if (skinMod)
	{
		ExportSkinData(node, skinMod);
	}
}

void		MaxSkinExp::ExportPhysiqueData(INode* node, Modifier	*phyMod)
{
	IPhysiqueExport *phyExport = (IPhysiqueExport *)phyMod->GetInterface(I_PHYEXPORT);
	IPhyContextExport *mcExport = (IPhyContextExport *)phyExport->GetContextInterface(node);

	//블랜딩  및 논블랜딩 타입을 변환
	mcExport->ConvertToRigid(true);
	mcExport->AllowBlending(true);


	// 블랜딩 타입일 경우 변환, 블랜딩 버텍스?
	// class IPhyBlendedRigidVertex : public IPhyVertexExport
	IPhyBlendedRigidVertex	*rb_vtx = NULL;

	// 논블랜딩 타입일 경우 변환
	// class IPhyRigidVertex : public IPhyVertexExport
	IPhyRigidVertex			*r_vtx = NULL;


	int numverts = mcExport->GetNumberVertices();

	ObjectState os = node->EvalWorldState(I_WRITER.m_pInterface->GetTime());

	//피직스에 적용 된 모든 정점들에 대한 정보를 얻는다
	for (int i = 0; i < numverts; i++)
	{
		float totalWeight = 0.0f, weight = 0.0f;
		TSTR nodeName;

		// 독립된 정점( IPhyVertexExport ) 을 원는다.
		IPhyVertexExport *vi = mcExport->GetVertexInterface(i);
		if (vi)
		{
			// 정점에 적용된 타입을 파악해야 한다.
			int type = vi->GetVertexType();
			switch (type)
			{
				//블랭딩 캐릭터일 경우
			case RIGID_BLENDED_TYPE:
			{
				rb_vtx = (IPhyBlendedRigidVertex*)vi;
				// 현 정점에 영향을 미치는 노드의 갯수
				int iNode = rb_vtx->GetNumberNodes();
				BipedVertex BipVertex;
				BipVertex.m_dwNumWeight = iNode;

				//Point3 BlendP(0.0f, 0.0f, 0.0f);
				for (int x = 0; x < iNode; x++)
				{
					INode* pNode = rb_vtx->GetNode(x);
					nodeName = pNode->GetName();
					// 전체 노드를 저장하고 있는 TMatrixExport에서 해당 인덱스를 리턴받는다.
					BYTE Index = (BYTE)I_WRITER.GetIndex(nodeName);
					BipVertex.m_BipIDList.push_back(Index);

					float fWeight = rb_vtx->GetWeight(x);
					BipVertex.m_fWeightList.push_back(fWeight);

					/*Point3 Offset = rb_vtx->GetOffsetVector(x);
					BlendP += (pNode->GetNodeTM(I_Writer.m_p3dsMax->GetTime()) * Offset) * fWeight;*/

					if (Index < 0)
					{
						break;
					}
				}
				// 최대 가중치 갯수를 저장
				if (m_Scene.iMaxWeight < iNode)
				{
					m_Scene.iMaxWeight = iNode;
					if (m_Scene.iMaxWeight > 8)
					{
						MessageBox(GetActiveWindow(), _T("바이패드+본+더미가 정점 당 8개 이상 적용되었습니다."), _T("최대 적용 에러!"), MB_OK | MB_ICONSTOP);
					}
				}
				m_BipedList.push_back(BipVertex);
			}
			break;

			//논블랭딩 캐릭터일 경우
			case RIGID_NON_BLENDED_TYPE:		
			{
				r_vtx = (IPhyRigidVertex*)vi;
				// 정점에 적용이 된 노드의 이름을 얻는다.
				INode* pNode = r_vtx->GetNode();
				nodeName = pNode->GetName();

				BipedVertex BipVertex;
				BipVertex.m_dwNumWeight = 1;
				//매트릭스 익스포터에서 저장된 모든 노드를 참조해서
				// 현 nodeName과 같은 노드를 찾아서 그 인덱스를 리턴한다.
				BYTE Index = (BYTE)I_WRITER.GetIndex(nodeName);
				BipVertex.m_BipIDList.push_back(Index);
				BipVertex.m_fWeightList.push_back(1.0f);

				// 최대 가중치 갯수를 저장
				if (m_Scene.iMaxWeight < 1)
				{
					m_Scene.iMaxWeight = 1;		//논블랭딩은 가중치가 모두 1
				}
				m_BipedList.push_back(BipVertex);

				if (Index < 0)
				{
					break;
				}
			}
			break;
			default:
				
				break;
			}
			mcExport->ReleaseVertexInterface(vi);
		}
	}
	phyExport->ReleaseContextInterface(mcExport);
	phyMod->ReleaseInterface(I_PHYINTERFACE, phyExport);

}
void		MaxSkinExp::ExportSkinData(INode* node, Modifier	*skinMod)
{
	ISkin *skin = (ISkin *)skinMod->GetInterface(I_SKIN);
	ISkinContextData *skinData = skin->GetContextInterface(node);

	if (skin && skinData)
	{
		int numberOfPoints;
		// 스킨버텍스의 갯수
		numberOfPoints = skinData->GetNumPoints();
		for (int i = 0; i < numberOfPoints; i++)
		{
			// 연결된 본의 갯수
			int numOfWeights = skinData->GetNumAssignedBones(i);	//본 갯수 

			BipedVertex BipVertex;
			BipVertex.m_dwNumWeight = numOfWeights;

			for (int j = 0; j < numOfWeights; j++)
			{
				int boneIndex = skinData->GetAssignedBone(i, j);
				INode * pBone = skin->GetBone(boneIndex);
				if (pBone == NULL)
				{
					break;
				}
				const TCHAR* nodeName = nodeName = pBone->GetName();//skin->GetBoneName(boneIndex);						
				float boneWeight = skinData->GetBoneWeight(i, j);

				BYTE Index = (BYTE)I_WRITER.GetIndex(nodeName);
				BipVertex.m_BipIDList.push_back(Index);
				BipVertex.m_fWeightList.push_back(boneWeight);


				if (Index < 0)
				{
					break;
				}
			}


			m_BipedList.push_back(BipVertex);

			// 최대 가중치 갯수를 저장
			if (m_Scene.iMaxWeight < numOfWeights)
			{
				m_Scene.iMaxWeight = numOfWeights;
				if (m_Scene.iMaxWeight > 8)
				{
					MessageBox(GetActiveWindow(), _T("바이패드+본+더미가 정점 당 8개 이상 적용되었습니다."), _T("최대 적용 에러!"), MB_OK | MB_ICONSTOP);
				}
			}
		}
	}


}

bool		MaxSkinExp::ExportMesh(FILE* fp, ASkinMesh* pMesh)
{
	//vector <PNCT5_VERTEX>			VertexArray;
	//vector < unsigned int>			IndexArray;
	//VertexArray.reserve(pMesh->m_iNumFace * 3);
	//IndexArray.reserve(pMesh->m_iNumFace * 3);
	//int iPosPushCnt = 0;
	//for (int iFace = 0; iFace < pMesh->m_iNumFace; iFace++)
	//{
	//	for (int iCnt = 0; iCnt < 3; iCnt++)
	//	{
	//		int iPosReturn = IsEqulVertexList(VertexArray, pMesh->m_SkinTriList[iFace].m_vVertex[iCnt]);
	//		if (iPosReturn < 0)
	//		{
	//			// 서브매터리얼 인덱스 => 정점 알파성분에 대입
	//			//pMesh->m_SkinTriList[iFace].m_vVertex[iCnt].c.w = pMesh->m_SkinTriList[iFace].iSubIndex;
	//			VertexArray.push_back(pMesh->m_SkinTriList[iFace].m_vVertex[iCnt]);
	//			iPosReturn = iPosPushCnt++;
	//		}
	//		IndexArray.push_back(iPosReturn);
	//	}
	//}
	//pMesh->m_iNumVertex = VertexArray.size();
	//m_FP << pMesh->m_iNumVertex << " " << pMesh->m_iNumFace << " " << pMesh->m_iNumTex << endl;


	//
	//for (int it=0;  it< VertexArray.size(); it++)
	//{
	//	PNCT5_VERTEX* vertex = &(VertexArray[it]);
	//	m_FP << 
	//		vertex->p.x << " " << VertexArray[it].p.y << " " <<VertexArray[it].p.z << " "<<
	//		VertexArray[it].n.x << " " << VertexArray[it].n.y << " " <<VertexArray[it].n.z << " "<<
	//		VertexArray[it].c.x << " " << VertexArray[it].c.y << " " <<VertexArray[it].c.z << " " <<VertexArray[it].c.w << " "<<
	//		VertexArray[it].t.x << " " << VertexArray[it].t.y << " " <<
	//		VertexArray[it].w1[0] << " " << VertexArray[it].w1[1] << " " << VertexArray[it].w1[2] << " " << VertexArray[it].w1[3] << " " <<
	//		VertexArray[it].i1[0] << " " << VertexArray[it].i1[1] << " " << VertexArray[it].i1[2] << " " << VertexArray[it].i1[3] << " " <<
	//		VertexArray[it].w2[0] << " " << VertexArray[it].w2[1] << " " << VertexArray[it].w2[2] << " " << VertexArray[it].w2[3] << " " <<
	//		VertexArray[it].i2[0] << " " << VertexArray[it].i2[1] << " " << VertexArray[it].i2[2] << " " << VertexArray[it].i2[3] <<endl;
	//}

	//for (auto it = IndexArray.begin(); it != IndexArray.end(); it++)
	//{
	//	m_FP <<	*it._Ptr<< " ";
	//}
	//m_FP << endl;

	//for (int itex = 0; itex < pMesh->m_iNumTex; itex++)
	//{
	//	int iLen = _tcslen(pMesh->m_szTexName[itex]);
	//	m_FP << iLen << " " << pMesh->m_iTexType[itex] << " ";//<< pMesh->m_szTexName[itex] << endl;
	//	char temp[30] = { 0, };
	//	for (int i = 0; i < 30; i++)
	//	{
	//		temp[i] = pMesh->m_szTexName[itex][i];
	//		m_FP << temp[i];
	//	}
	//	m_FP << endl;
	//}



	vector <PNCT5_VERTEX>			VertexArray;
	vector < unsigned int>			IndexArray;
	VertexArray.reserve(pMesh->m_iNumFace * 3);
	IndexArray.reserve(pMesh->m_iNumFace * 3);

	int iPosPushCnt = 0;
	for (int iFace = 0; iFace < pMesh->m_iNumFace; iFace++)
	{
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			int iPosReturn = IsEqulVertexList(VertexArray, pMesh->m_SkinTriList[iFace].m_vVertex[iCnt]);
			if (iPosReturn < 0)
			{
				// 서브매터리얼 인덱스 => 정점 알파성분에 대입
				//pMesh->m_SkinTriList[iFace].m_vVertex[iCnt].c.w = pMesh->m_SkinTriList[iFace].iSubIndex;
				VertexArray.push_back(pMesh->m_SkinTriList[iFace].m_vVertex[iCnt]);
				iPosReturn = iPosPushCnt++;
			}
			IndexArray.push_back(iPosReturn);
		}
	}
	pMesh->m_iNumVertex = VertexArray.size();

	fwrite(&pMesh->m_iNumVertex, sizeof(int), 1, fp);
	fwrite(&pMesh->m_iNumFace, sizeof(int), 1, fp);
	fwrite(&pMesh->m_iNumTex, sizeof(int), 1, fp);

	fwrite(&VertexArray.at(0), sizeof(PNCT5_VERTEX)	* VertexArray.size(), 1, fp);
	fwrite(&IndexArray.at(0), sizeof(unsigned int)	* IndexArray.size(), 1, fp);

	for (int itex = 0; itex < pMesh->m_iNumTex; itex++)
	{
		int iLen = _tcslen(pMesh->m_szTexName[itex]);
		fwrite(&iLen, sizeof(int), 1, fp);
		fwrite(&pMesh->m_iTexType[itex], sizeof(int), 1, fp);
		fwrite(pMesh->m_szTexName[itex], sizeof(TCHAR) * iLen, 1, fp);
	}


	return true;
}

//정점 v0 v2 v1
void		MaxSkinExp::SetVertexBiped(INode* node, Face*	face,
	int v0, int v1, int v2,
	ASkinTri* pTri)
{
	int iNumWeight = 1;
	if (m_BipedList.size())
	{
		iNumWeight = m_BipedList[face->v[v0]].m_BipIDList.size();
		for (int iBip = 0; iBip < iNumWeight; iBip++)
		{
			if (iBip < 4)
			{
				pTri->m_vVertex[0].i1[iBip] = m_BipedList[face->v[v0]].m_BipIDList[iBip];
				pTri->m_vVertex[0].w1[iBip] = m_BipedList[face->v[v0]].m_fWeightList[iBip];
			}
			else
			{
				pTri->m_vVertex[0].i2[iBip - 4] = m_BipedList[face->v[v0]].m_BipIDList[iBip];
				pTri->m_vVertex[0].w2[iBip - 4] = m_BipedList[face->v[v0]].m_fWeightList[iBip];
			}
		}
		iNumWeight = m_BipedList[face->v[v2]].m_BipIDList.size();
		for (int iBip = 0; iBip < iNumWeight; iBip++)
		{
			if (iBip < 4)
			{
				pTri->m_vVertex[1].i1[iBip] = m_BipedList[face->v[v2]].m_BipIDList[iBip];
				pTri->m_vVertex[1].w1[iBip] = m_BipedList[face->v[v2]].m_fWeightList[iBip];
			}
			else
			{
				pTri->m_vVertex[1].i2[iBip - 4] = m_BipedList[face->v[v2]].m_BipIDList[iBip];
				pTri->m_vVertex[1].w2[iBip - 4] = m_BipedList[face->v[v2]].m_fWeightList[iBip];
			}
		}
		iNumWeight = m_BipedList[face->v[v1]].m_BipIDList.size();
		for (int iBip = 0; iBip < iNumWeight; iBip++)
		{
			if (iBip < 4)
			{
				pTri->m_vVertex[2].i1[iBip] = m_BipedList[face->v[v1]].m_BipIDList[iBip];
				pTri->m_vVertex[2].w1[iBip] = m_BipedList[face->v[v1]].m_fWeightList[iBip];
			}
			else
			{
				pTri->m_vVertex[2].i2[iBip - 4] = m_BipedList[face->v[v1]].m_BipIDList[iBip];
				pTri->m_vVertex[2].w2[iBip - 4] = m_BipedList[face->v[v1]].m_fWeightList[iBip];
			}
		}
		// 정점 당 가중치 적용 갯수
		pTri->m_vVertex[0].w2[3] = m_BipedList[face->v[v0]].m_dwNumWeight;
		pTri->m_vVertex[1].w2[3] = m_BipedList[face->v[v2]].m_dwNumWeight;
		pTri->m_vVertex[2].w2[3] = m_BipedList[face->v[v1]].m_dwNumWeight;
	}
	else
	{
		// 정점에 적용된 바이패드가 없다는 것은 자체 에니메이션을 가진 오브젝트가 된다.
		// 이때는 가중치를 얻을 수 없기 때문에 직접 1개의 가중치 값을 세팅해 주어야 한다.
		int iBipIndex = I_WRITER.GetIndex(node->GetName());
		pTri->m_vVertex[0].i1[0] = iBipIndex;
		pTri->m_vVertex[0].w1[0] = 1.0f;
		pTri->m_vVertex[1].i1[0] = iBipIndex;
		pTri->m_vVertex[1].w1[0] = 1.0f;
		pTri->m_vVertex[2].i1[0] = iBipIndex;
		pTri->m_vVertex[2].w1[0] = 1.0f;
		// 가중치 적용 갯수
		pTri->m_vVertex[0].w2[3] = 1.0f;
		pTri->m_vVertex[1].w2[3] = 1.0f;
		pTri->m_vVertex[2].w2[3] = 1.0f;
	}
}

bool		MaxSkinExp::GetNodeInfo(INode* node, TimeValue t)
{
	BOOL needDel;
	TriObject	*tri = I_WRITER.GetTriObjectFromNode(node, t, needDel);
	if (tri == NULL) return false;

	Mesh		*mesh = &tri->GetMesh();
	mesh->buildNormals();
	if (!mesh->faces || !tri || !mesh || !mesh->getNumFaces() || !mesh->getNumVerts() || !mesh->getNumTVerts())
	{
		return false;
	}

	SetBipedInfo(node);

	ASkinMesh*	SkinMesh = NULL;
	SAFE_NEW(SkinMesh, ASkinMesh);

	// 메쉬를 저장한다.( 서브메터리얼 여부를 판단하여 저장)
	GenerateGroup(node, mesh, SkinMesh);

	m_SkinMeshList.push_back(SkinMesh);

	if (needDel)
	{
		delete tri;
	}

	return true;
}


// 서브메터리얼 사용시를 고려하여 페이스 리스트 생성
void		MaxSkinExp::GenerateGroup(INode* node, Mesh *mesh, ASkinMesh* pSkinMesh)
{
	Mtl* mtl = node->GetMtl();
	pSkinMesh->m_strNodeName = node->GetName();

	if (mtl)
	{
		pSkinMesh->m_iMtrlRef = mtl->NumSubMtls();
	}
	if (pSkinMesh->m_iMtrlRef > 0)
	{
		for (int imtl = 0; imtl < pSkinMesh->m_iMtrlRef; imtl++)
		{

			ASkinMesh* pSubSkinMesh = NULL;
			SAFE_NEW(pSubSkinMesh,ASkinMesh);
			// 맥스 서브메터리얼 정보 저장
			Mtl* subMtl = mtl->GetSubMtl(imtl);
			LoadMaterial(pSubSkinMesh, subMtl);
			// 매쉬를 이루는 페이스리스트를 만든다.
			SetTriangle(node, mesh, pSkinMesh, pSubSkinMesh, imtl);
			pSkinMesh->m_pSubMesh.push_back(pSubSkinMesh);
		}
	}
	else
	{
		LoadMaterial(pSkinMesh, mtl);
		// 매쉬를 이루는 페이스리스트를 만든다.
		SetTriangle(node, mesh, pSkinMesh);
	}
}

void		MaxSkinExp::LoadMaterial(ASkinMesh*	pSkinMesh, Mtl* mtl)
{
	if (mtl)
	{
		pSkinMesh->m_iNumTex = 0;
		memset(pSkinMesh->m_szTexName, 0, sizeof(TCHAR) * 13 * 30);
		for (int i = 0; i < mtl->NumSubTexmaps(); i++)
		{
			Texmap* subTex = mtl->GetSubTexmap(i);
			float amt = 1.0f;
			if (subTex)
			{
				if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
				{
					if (!((StdMat*)mtl)->MapEnabled(i))
						continue;
				}
				if (subTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
				{
					pSkinMesh->m_iTexType[pSkinMesh->m_iNumTex] = GetMapID(mtl->ClassID(), i);
					TSTR mapName = ((BitmapTex *)subTex)->GetMapName();
					TCHAR Drive[MAX_PATH];
					TCHAR Dir[MAX_PATH];
					TCHAR FName[MAX_PATH];
					TCHAR Ext[MAX_PATH];
					_tsplitpath_s(I_WRITER.FixupName(mapName), Drive, Dir, FName, Ext);
					Ext[4] = 0;
					_stprintf_s(pSkinMesh->m_szTexName[pSkinMesh->m_iNumTex], _T("%s%s"), FName, Ext);
					pSkinMesh->m_iNumTex++;
				}
			}
		}
	}
}

void		MaxSkinExp::SetTriangle(INode* node, Mesh *mesh, ASkinMesh* pSkinMesh, ASkinMesh*	pSubMesh, int iMtrl)
{
	ASkinMesh* pMesh = NULL;
	if (pSubMesh == NULL)
	{
		pMesh = pSkinMesh;
	}
	else
	{
		pMesh = pSubMesh;
	}

	Matrix3		tm = node->GetObjTMAfterWSM(I_WRITER.m_Interval.Start());
	Matrix3		nodetm = node->GetNodeTM(I_WRITER.m_Interval.Start());
	I_WRITER.DumpMatrix3(&nodetm, pSkinMesh->m_matWorld);

	int v0, v1, v2;
	v0 = 0, v1 = 1, v2 = 2;
	if (I_WRITER.TMNegParity(tm))
	{
		v0 = 2, v1 = 1, v2 = 0;
	}
	Matrix3 invTM = Inverse(nodetm);
	for (int i = 0; i < mesh->getNumFaces(); i++)
	{
		// 서브매터리얼 사용시는 그룹화 해야하기 때문에 해당 매터리얼이 아닐시 반복시킴
		if (pMesh->m_iMtrlRef > 0 && mesh->faces[i].getMatID() != iMtrl)
		{
			continue;
		}

		ASkinTri Triangle;
		// 페이스 리스트 초기화
		memset(&Triangle, 0, sizeof(ATri));
		// 서브매터리얼 인덱스
		Triangle.iSubIndex = mesh->faces[i].getMatID();

		// 정점 위치 저장
		if (mesh->getNumVerts() > 0)
		{
			SetVertexBiped(node, &mesh->faces[i], v0, v1, v2, &Triangle);
			for (int iVertex = 0; iVertex < 3; iVertex++)
			{
				Point3 v;
				if (iVertex == 0)		v = tm * mesh->verts[mesh->faces[i].v[v0]];
				else if (iVertex == 1)	v = tm * mesh->verts[mesh->faces[i].v[v2]];
				else					v = tm * mesh->verts[mesh->faces[i].v[v1]];
				//v = invTM * v;
				Triangle.m_vVertex[iVertex].p.x = v.x;
				Triangle.m_vVertex[iVertex].p.y = v.z;
				Triangle.m_vVertex[iVertex].p.z = v.y;
			}
		}

		// 정점 텍스쳐좌표 저장
		if (mesh->getNumTVerts() > 0)
		{
			Triangle.m_vVertex[0].t.x = mesh->tVerts[mesh->tvFace[i].t[v0]].x;
			Triangle.m_vVertex[0].t.y = 1.0f - mesh->tVerts[mesh->tvFace[i].t[v0]].y;
			Triangle.m_vVertex[1].t.x = mesh->tVerts[mesh->tvFace[i].t[v2]].x;
			Triangle.m_vVertex[1].t.y = 1.0f - mesh->tVerts[mesh->tvFace[i].t[v2]].y;
			Triangle.m_vVertex[2].t.x = mesh->tVerts[mesh->tvFace[i].t[v1]].x;
			Triangle.m_vVertex[2].t.y = 1.0f - mesh->tVerts[mesh->tvFace[i].t[v1]].y;
		}

		// 정점 컬러 저장
		Triangle.m_vVertex[0].c = Point4(1.0f, 1.0f, 1.0f, 1.0f);
		Triangle.m_vVertex[1].c = Point4(1.0f, 1.0f, 1.0f, 1.0f);
		Triangle.m_vVertex[2].c = Point4(1.0f, 1.0f, 1.0f, 1.0f);
		if (mesh->getNumVertCol() > 0)
		{
			Triangle.m_vVertex[0].c.w = 0xff;
			Triangle.m_vVertex[0].c.x = mesh->vertCol[mesh->vcFace[i].t[v0]].x;
			Triangle.m_vVertex[0].c.y = mesh->vertCol[mesh->vcFace[i].t[v0]].y;
			Triangle.m_vVertex[0].c.z = mesh->vertCol[mesh->vcFace[i].t[v0]].z;

			Triangle.m_vVertex[1].c.w = 0xff;
			Triangle.m_vVertex[1].c.x = mesh->vertCol[mesh->vcFace[i].t[v2]].x;
			Triangle.m_vVertex[1].c.y = mesh->vertCol[mesh->vcFace[i].t[v2]].y;
			Triangle.m_vVertex[1].c.z = mesh->vertCol[mesh->vcFace[i].t[v2]].z;

			Triangle.m_vVertex[2].c.w = 0xff;
			Triangle.m_vVertex[2].c.x = mesh->vertCol[mesh->vcFace[i].t[v1]].x;
			Triangle.m_vVertex[2].c.y = mesh->vertCol[mesh->vcFace[i].t[v1]].y;
			Triangle.m_vVertex[2].c.z = mesh->vertCol[mesh->vcFace[i].t[v1]].z;
		}

		// 정점 노말 및 페이스 노말 저장
		Point3 vn, fn;  // Vertex normal, Face normal
		int  vert;
		Face* f;

		f = &mesh->faces[i];
		vert = f->getVert(v0);
		vn = I_WRITER.GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));

		Triangle.m_vVertex[0].n.x = vn.x;
		Triangle.m_vVertex[0].n.y = vn.z;
		Triangle.m_vVertex[0].n.z = vn.y;
		vert = f->getVert(v2);
		vn = I_WRITER.GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));

		Triangle.m_vVertex[1].n.x = vn.x;
		Triangle.m_vVertex[1].n.y = vn.z;
		Triangle.m_vVertex[1].n.z = vn.y;
		vert = f->getVert(v1);
		vn = I_WRITER.GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));

		Triangle.m_vVertex[2].n.x = vn.x;
		Triangle.m_vVertex[2].n.y = vn.z;
		Triangle.m_vVertex[2].n.z = vn.y;

		pMesh->m_SkinTriList.push_back(Triangle);
	}
	pMesh->m_iNumFace = pMesh->m_SkinTriList.size();
}

int			MaxSkinExp::GetMapID(Class_ID cid, int subNo)
{
	int iResultIndex = 1;
	if (cid == Class_ID(0, 0))
	{
		iResultIndex = 30;
	}
	else if (cid == Class_ID(DMTL_CLASS_ID, 0))
	{
		switch (subNo)
		{
		case ID_AM: iResultIndex = 0; break;
		case ID_DI: iResultIndex = 1; break;
		case ID_SP: iResultIndex = 2; break;
		case ID_SH: iResultIndex = 3; break;
		case ID_SS: iResultIndex = 4; break;
		case ID_SI: iResultIndex = 5; break;
		case ID_OP: iResultIndex = 6; break;
		case ID_FI: iResultIndex = 7; break;
		case ID_BU: iResultIndex = 8; break;
		case ID_RL: iResultIndex = 9; break;
		case ID_RR: iResultIndex = 10; break;
		}
	}
	else
	{
		iResultIndex = 31;
	}
	return iResultIndex;
}

int			MaxSkinExp::IsEqulVertexList(vector<PNCT5_VERTEX>&  VertexArray, PNCT5_VERTEX& Vertex)
{
	for (int iVertex = 0; iVertex < VertexArray.size(); iVertex++)
	{
		if (I_WRITER.EqualPoint3(VertexArray[iVertex].p, Vertex.p) &&
			I_WRITER.EqualPoint3(VertexArray[iVertex].n, Vertex.n) &&
			I_WRITER.EqualPoint4(VertexArray[iVertex].c, Vertex.c) &&
			I_WRITER.EqualPoint2(VertexArray[iVertex].t, Vertex.t))
		{
			return iVertex;
		}
	}
	return -1;
}



MaxSkinExp::MaxSkinExp()
{
	
}


MaxSkinExp::~MaxSkinExp()
{
	Release();
}
