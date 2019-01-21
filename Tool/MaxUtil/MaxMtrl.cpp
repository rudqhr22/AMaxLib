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
		AddMtl.strClassName = FixupName(className);		//���׸���Ŭ����
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

				//���긶�׸��� ���� ���� ���� ��� ���� �ؼ� ����
				if (subMtl->NumSubMtls()> 0)		
				{
					//MessageBox(0, _T("���� ���� ���͸���"), _T("Fatal error"), MB_OK);
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
		Texmap* tex = pSrcMtl->GetSubTexmap(iSubMap);	//�ؽ�ó ���� �����´�.
		if (tex)	//������ �Ƴ� �Ȍ��� Ȯ�� -> // �ƽ��� �� Ŭ���� ���̵�� �񱳸� �Ѵٰ� ���� �ȴ�.
		{
			if (pSrcMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
			{
				StdMat* std = (StdMat*)pSrcMtl;				//���Ĵٵ� ���͸����� ����� ������
				//std������ Ȯ���� �Ѵ�.
				if (std->MapEnabled(iSubMap)==FALSE)
				{
					continue;								//��ũ�� �ְ� ������ �ȵǸ� ��� ����
				}
			}

			ATexMap TexMap;
			TexMap.iType = iSubMap;

			if (tex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))	//��Ʈ���̸� �ؽ�ó�� �����Ͷ�
			{
				TSTR mapName = ((BitmapTex *)tex)->GetMapName();	//�ؽ�ó �̸� ��������
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
