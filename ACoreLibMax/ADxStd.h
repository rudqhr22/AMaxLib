#pragma once
#include <dinput.h>
#include "D3DCompiler.h"
#include <DXGI.h>
#include <D3DX11.h>
#include <D3DX10math.h>
#include <Dwrite.h>
#include <wrl.h>
#include <winnt.h>
#include "D2D1Helper.h"
#include <assert.h>

#pragma comment (lib,"d3dx11.lib")
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"dxgi.lib")
#pragma comment (lib,"d2d1.lib" )
#pragma comment (lib,"dwrite.lib" )
#pragma comment (lib, "dinput8.lib")
#pragma comment( lib, "d3dcompiler.lib" )
#pragma comment( lib, "d3dx10.lib" )

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")


using namespace Microsoft::WRL;
extern ComPtr<ID3D11Device>			g_pd3dDevice;
extern ComPtr<ID3D11DeviceContext>	g_pImmediateContext;
extern ComPtr<ID3D11DeviceContext>	g_pDeferredContext;