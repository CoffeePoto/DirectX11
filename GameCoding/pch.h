#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment (lib, "DirectXTex\\DirectXTex_Debug.lib")
#else
#pragma comment (lib, "DirectXTex\\DirectXTex.lib")
#endif


#include "Types.h"
#include "Values.h"
#include "Struct.h"

//STL
#include <memory>
#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
using namespace std;

//WIN
#include <Windows.h>
#include <assert.h>

//DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

using namespace DirectX;
using namespace Microsoft::WRL;

#define CHECK(p) assert(SUCCEEDED(p))

//Global
#define		GAME		GGame
#define		INPUT		GAME->GetInputManager()
#define		TIME		GAME->GetTimeManager()
#define		SCENE		GAME->GetSceneManager()
#define		RESOURCES	GAME->GetResourceManager()

//Engine
#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "Geometry.h"
#include "GeometryHelper.h"
#include "VertexData.h"
#include "ShaderBase.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "BlendState.h"
#include "Pipeline.h"
#include "Transform.h"