#pragma once

// WinAPI����
#pragma warning(push)
#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#include "windows.h"
#include <wincodec.h>

// DirectX11 ����
#pragma warning(disable:4005)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include "dinput.h"
#include "mmsystem.h"


// C++ ���C�u����
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <map>
#include <random>
#include <cmath> 
#include <algorithm>
#include <fstream>
#include <windows.h>
#include <stdexcept>
#include <filesystem>
#include <cwchar>
#include <list>
#include <future>
//#include <minmax.h>
#include <functional>
#include <filesystem>

//FBXSDK���C�u����
#include <fbxsdk.h>

//directxtex
#include "DirectXTex.h"
#include "DDSTextureLoader11.h"
#include "WICTextureLoader11.h"

//physix
//#include "PxPhysicsAPI.h"

#pragma warning(pop)
//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#pragma comment (lib,"libfbxsdk-mt.lib")
#pragma comment (lib,"libxml2-mt.lib")
#pragma comment (lib,"zlib-mt.lib")

#pragma comment (lib,"DirectXTex.lib")

//#pragma comment(lib, "PhysX_64.lib")
//#pragma comment(lib, "PhysXCommon_64.lib")
//#pragma comment(lib, "PhysXCooking_64.lib")
//#pragma comment(lib, "PhysXExtensions_static_64.lib")
//#pragma comment(lib, "PhysXFoundation_64.lib")
//#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
//#pragma comment(lib, "PhysXTask_static_64.lib")
//#pragma comment(lib, "SceneQuery_static_64.lib")
//#pragma comment(lib, "SimulationController_static_64.lib")
//


// �{���̓w�b�_�ɏ����Ȃ������ǂ�
using namespace DirectX;
using namespace std;
//using namespace physx;

// �v���W�F�N�g�Ǝ�
#include "Math.h"

