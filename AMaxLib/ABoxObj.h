#pragma once
#include "ADxShape.h"
#include "ADxSO.h"

class ABoxObj : public ABoxShape
{
public:
	int						m_iNumSteamOut;
	ADxSO					m_RenderSO;
	ComPtr<ID3D11GeometryShader>	m_pGS;
	ComPtr<ID3D11GeometryShader>	m_pGSStreamOutput;
	ComPtr<ID3D11VertexShader>		m_pVSPassThroughMain;
	ComPtr<ID3DBlob>	m_pVSBlob;
	ComPtr<ID3DBlob>	m_pGSBlob;
public:
	bool	Init();
	bool CompileShader(ID3D11Device* pDevice, const TCHAR* pShaderFile);
	bool	SetSO(ID3D11Device* pd3dDevice, ID3D11DeviceContext*  pContext);
	bool	Render(ID3D11DeviceContext*  pContext);
	bool	RenderMesh(ID3D11DeviceContext*  pContext, bool bFirstRender = true);

public:
	ABoxObj();
	virtual ~ABoxObj();
};

