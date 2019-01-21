#include "MaxMtrl.h"



void	MaxMtrl::AddMaterial()
{

	for (int iMtl = 0; iMtl < m_pMtlList.size(); iMtl++)
	{
		Mtl* pSrcMtl = m_pMtlList[iMtl];

		AMtrl AddMtl;
		TSTR className;
		pSrcMtl->GetClassName(className);
		AddMtl.strName = FixupName(pSrcMtl->GetName());
		AddMtl.strClassName = FixupName(className);		//마테리얼클래스
		AddMtl.iNumSubMtl = pSrcMtl->NumSubMtls();
		AddMtl.iIndex = iMtl;

		if (pSrcMtl->NumSubMtls() > 0)
		{
			for (int iSubMtrl = 0; iSubMtrl< pSrcMtl->NumSubMtls(); iSubMtrl++)
			{
				Mtl* subMtl = pSrcMtl->GetSubMtl(iSubMtrl);		

				AMtrl AddSubMtl;
				AddSubMtl.iIndex = iSubMtrl;
				AddSubMtl.iNumSubMtl = subMtl->NumSubMtls();
				AddSubMtl.strName = FixupName(subMtl->GetName());

				TSTR className;
				subMtl->GetClassName(className);
				AddSubMtl.strClassName = FixupName(className);

				//서브마테리얼 있을 경우와 없을 경우 구분 해서 ㄱㄱ
				if (subMtl->NumSubMtls()> 0)		
				{
					//MessageBox(0, _T("다중 서브 매터리얼"), _T("Fatal error"), MB_OK);
				}
				if (subMtl)
				{
					GetTexture(&AddSubMtl, subMtl);
				}
				AddMtl.SubMaterial.push_back(AddSubMtl);
			
				//if (subMtl->NumSubMtls() > 0)
				//{
				//	GetTexture(&AddSubMtl, subMtl);
				//}
				//AddMtl.SubMaterial.push_back(AddSubMtl);
			}
		}
		else
		{
			GetTexture(&AddMtl, pSrcMtl);
		}
		m_Material.push_back(AddMtl);
	}

}


void	MaxMtrl::GetTexture(AMtrl* pDestMtl, Mtl* pSrcMtl)
{
	for (int iSubMap = 0; iSubMap < pSrcMtl->NumSubTexmaps(); iSubMap++)
	{
		Texmap* tex = pSrcMtl->GetSubTexmap(iSubMap);	//텍스처 맵을 가져온다.
		if (tex)	//적용이 됐나 안뙜나 확인 -> // 맥스는 다 클래스 아이디로 비교를 한다고 보면 된다.
		{
			if (pSrcMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
			{
				StdMat* std = (StdMat*)pSrcMtl;				//스탠다드 메터리얼을 만들고 컨버팅
				//std적용을 확인을 한다.
				if (std->MapEnabled(iSubMap)==FALSE)
				{
					continue;								//뱅크에 있고 적응이 안되면 출력 ㄴㄴ
				}
			}

			ATexMap TexMap;
			TexMap.iType = iSubMap;

			if (tex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))	//비트맵이면 텍스처를 가져와라
			{
				TSTR mapName = ((BitmapTex *)tex)->GetMapName();	//텍스처 이름 가져오기
				TSTR strFullName;
				SplitPathFile(mapName, &strFullName, &mapName);
				TexMap.strTextureName = FixupName(mapName);
			}
			pDestMtl->TexMaps.push_back(TexMap);
			pDestMtl->iNumMap = pDestMtl->TexMaps.size();
		}
	}
}


MaxMtrl::MaxMtrl()
{
}


MaxMtrl::~MaxMtrl()
{
}
