//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


#include <vector>
#include "resource.h"
#include "CCamera.h"
#include "CDevice.h"
#include "CSwapChain.h"
#include "CDeviceContext.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CTexture2D.h"
#include "CDepthStencilView.h"
#include "CVertexShader.h"
#include "CPixelShader.h"
#include "CViewport.h"
#include "CRenderTargetView.h"
#include "CSamplerState.h"
#include "imgui.h"

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

struct SimpleVertex
{
    glm::vec3 Pos;
    glm::vec2 Tex;
};

struct CBNeverChanges
{
	glm::mat4 mView;
};

struct CBChangeOnResize
{
	glm::mat4 mProjection;
};

struct CBChangesEveryFrame
{
	glm::mat4 mWorld;
	glm::vec4 vMeshColor;
};

//Lab array
glm::vec3 Lab[32]{
	{-2, 0, 2}, {-2, 0, 4}, {0, 0, 6},
	{2, 0, 6},	{4, 0, 6}, {4, 0, 8},
	{4, 0, 10}, {2, 0, 10}, {0, 0, 10},
	{0, 0, 14}, {2, 0, 14}, {4, 0, 14},
	{6, 0, 14}, {8, 0, 12}, {8, 0, 10},
	{10, 0, 10},{12, 0, 10},{10, 0, 14},
	{12, 0, 14},{14, 0, 14},{16, 0, 12},
	{16, 0, 10},{16, 0, 8}, {16, 0, 6},
	{14, 0, 6},	{12, 0, 6}, {10, 0, 6},
	{8, 0, 6},	{8, 0, 4},	{6, 0, 2},
	{4, 0, 2}, {2, 0, 2}
};

CDevice * CDevice::m_DeviceInstance = nullptr;
CSwapChain * CSwapChain::m_pSCInstance = nullptr;
CDeviceContext * CDeviceContext::m_DCInstance = nullptr;

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE                           g_hInst = NULL;
HWND                                g_hWnd = NULL;
#ifdef D3D11
ID3D11ShaderResourceView*           g_pTextureRV = NULL;
#endif

glm::vec4							g_MeshColor { 0.7f, 0.7f, 0.7f, 1.0f };

CCamera								g_Camera;
CCamera								anotherCamera;

glm::mat4							g_World;

FEATURE_LEVEL						featureLevel = FEATURE_LEVEL_11_0;

//Pointer to CDevice Singleton
CDevice *							g_pDevice = CDevice::getInstance();
//Pointer to CSwapchain Singleton
CSwapChain *						g_SwapChain = CSwapChain::getInstance();
//Pointer to CDeviceContext Singleton
CDeviceContext *					g_DeviceContext = CDeviceContext::getInstance();
CVertexBuffer						g_VertexBuffer;
CIndexBuffer						g_IndexBuffer;
CBuffer								g_CBNeverChanges;
CBuffer								g_CBChangeOnResize;
CBuffer								g_CBChangesEveryFrame;
CTexture2D							g_DepthStencil;
CDepthStencilView					g_DepthStencilView;
CVertexShader						g_VertexShader;
CPixelShader						g_PixelShader;
CRenderTargetView					g_RenderTargetView;
CSamplerState						g_SamplerState;

CBuffer								g_CBNeverChanges2;
CBuffer								g_CBChangeOnResize2;
CBuffer								g_CBChangesEveryFrame2;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();

#ifdef D3D11
HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	int offset = 0;
	for (int i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = offset;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT; offset += 12;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// Try to create Input Layout
	HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	return hr;
}
#endif

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            Render();
        }
    }
    CleanupDevice();
    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
#ifdef D3D11
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}
#endif

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    unsigned int width = rc.right - rc.left;
    unsigned int height = rc.bottom - rc.top;

#ifdef D3D11
	DeviceStruct T;

	T.m_DeviceFlags = 0;
#ifdef _DEBUG
	T.m_DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	T.m_DriverTypes[0] = DRIVER_TYPE_HARDWARE;
	T.m_DriverTypes[1] = DRIVER_TYPE_WARP;
	T.m_DriverTypes[2] = DRIVER_TYPE_REFERENCE;

	T.m_FeatureLevels[0] = FEATURE_LEVEL_11_0;
	T.m_FeatureLevels[1] = FEATURE_LEVEL_10_1;
	T.m_FeatureLevels[2] = FEATURE_LEVEL_10_0;

	T.m_numFeatureLevels = ARRAYSIZE(T.m_FeatureLevels);

	unsigned int numDriverTypes = ARRAYSIZE(T.m_DriverTypes);

	g_pDevice->init(T);

	SwapChainStruct S;

	S.bufferCount = 1;
	S.W = width;
	S.H = height;
	S.format = FORMAT_R8G8B8A8_UNORM;
	S.refreshNumerator = 60;
	S.refreshDenominator = 1;
	S.bufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	S.outputWND = g_hWnd;
	S.count = 1;
	S.quality = 0;
	S.windowed = TRUE;

	g_SwapChain->init(S);

    for( unsigned int driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
		g_pDevice->m_struc.m_DriverType = g_pDevice->m_struc.m_DriverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, (D3D_DRIVER_TYPE)g_pDevice->m_struc.m_DriverType, NULL, g_pDevice->m_struc.m_DeviceFlags, (D3D_FEATURE_LEVEL *)g_pDevice->m_struc.m_FeatureLevels, g_pDevice->m_struc.m_numFeatureLevels,
                                            D3D11_SDK_VERSION, &g_SwapChain->m_sd, &g_SwapChain->m_pSwapChain, &g_pDevice->m_Device, (D3D_FEATURE_LEVEL *)&featureLevel, &g_DeviceContext->m_DeviceContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
	CTexture2D BackBuffer;
    hr = g_SwapChain->m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&BackBuffer.m_pTexture );
    if( FAILED( hr ) )
        return hr;

    hr = g_pDevice->m_Device->CreateRenderTargetView(BackBuffer.m_pTexture, NULL, &g_RenderTargetView.m_pRTV);
	BackBuffer.m_pTexture->Release();
    if( FAILED( hr ) )
        return hr;

    // Create depth stencil texture
	Texture2DStruct DepthDesc;
	DepthDesc.W = width;
	DepthDesc.H = height;
	DepthDesc.mipLevels = 1;
	DepthDesc.arraySize = 1;
	DepthDesc.format = FORMAT_D24_UNORM_S8_UINT;
	DepthDesc.sampleDesc.count = 1;
	DepthDesc.sampleDesc.quality = 0;
	DepthDesc.usage = USAGE_DEFAULT;
	DepthDesc.flags = D3D11_BIND_DEPTH_STENCIL;
	DepthDesc.cpuAccessFlags = 0;
	DepthDesc.miscFlags = 0;

	g_DepthStencil.init(DepthDesc);

	hr = g_pDevice->m_Device->CreateTexture2D(&g_DepthStencil.m_Desc, NULL, &g_DepthStencil.m_pTexture);
	if (FAILED(hr))
		return hr;

    // Create the depth stencil view
	DepthStencilViewStruct DSV;
	DSV.format = g_DepthStencil.m_Data.format;
	DSV.viewDimension = DSV_DIMENSION_TEXTURE2D;
	DSV.texture2D.mipSlice = 0;

	g_DepthStencilView.init(DSV, (FORMAT)g_DepthStencil.m_Desc.Format);

    hr = g_pDevice->m_Device->CreateDepthStencilView(g_DepthStencil.m_pTexture, &g_DepthStencilView.m_Desc, &g_DepthStencilView.m_pDepthStencilView );
    if( FAILED( hr ) )
        return hr;

	g_DeviceContext->m_DeviceContext->OMSetRenderTargets( 1, &g_RenderTargetView.m_pRTV, g_DepthStencilView.m_pDepthStencilView);

    // Setup the viewport
	ViewportStruct vpStruct;
	vpStruct.W = width;
	vpStruct.H = height;
	vpStruct.minDepth = 0.0f;
	vpStruct.maxDepth = 1.0f;
	vpStruct.topLeftX = 0;
	vpStruct.topLeftY = 0;

	CViewport tempVP;
	tempVP.init(vpStruct);
	g_DeviceContext->m_DeviceContext->RSSetViewports(1, &tempVP.m_Viewport);

    // Compile the vertex shader
    hr = CompileShaderFromFile( L"Tutorial07.fx", "VS", "vs_4_0", &g_VertexShader.m_pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

    //// Create the vertex shader
	hr = g_pDevice->m_Device->CreateVertexShader( g_VertexShader.m_pVSBlob->GetBufferPointer(), g_VertexShader.m_pVSBlob->GetBufferSize(), NULL, &g_VertexShader.m_pVertexShader );
    if( FAILED( hr ) )
    {    
        g_VertexShader.m_pVSBlob->Release();
        return hr;
    }

	//Create input layout from compiled VS
	hr = CreateInputLayoutDescFromVertexShaderSignature(g_VertexShader.m_pVSBlob, g_pDevice->m_Device, &g_VertexShader.m_pInputLayout);
	if (FAILED(hr))
		return hr;

    // Set the input layout
	g_DeviceContext->m_DeviceContext->IASetInputLayout(g_VertexShader.m_pInputLayout );

    // Compile the pixel shader
    hr = CompileShaderFromFile( L"Tutorial07.fx", "PS", "ps_4_0", &g_PixelShader.m_pPSBlob);
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

    // Create the pixel shader
    hr = g_pDevice->m_Device->CreatePixelShader(g_PixelShader.m_pPSBlob->GetBufferPointer(), g_PixelShader.m_pPSBlob->GetBufferSize(), NULL, &g_PixelShader.m_pPixelShader);
	g_PixelShader.m_pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { glm::vec3( -1.0f, 1.0f, -1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( -1.0f, 1.0f, 1.0f ), glm::vec2( 0.0f, 1.0f ) },

        { glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( 1.0f, -1.0f, -1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( 1.0f, -1.0f, 1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( -1.0f, -1.0f, 1.0f ), glm::vec2( 0.0f, 1.0f ) },

        { glm::vec3( -1.0f, -1.0f, 1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( -1.0f, 1.0f, -1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( -1.0f, 1.0f, 1.0f ), glm::vec2( 0.0f, 1.0f ) },

        { glm::vec3( 1.0f, -1.0f, 1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( 1.0f, -1.0f, -1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec2( 0.0f, 1.0f ) },

        { glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( 1.0f, -1.0f, -1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( -1.0f, 1.0f, -1.0f ), glm::vec2( 0.0f, 1.0f ) },

        { glm::vec3( -1.0f, -1.0f, 1.0f ), glm::vec2( 0.0f, 0.0f ) },
        { glm::vec3( 1.0f, -1.0f, 1.0f ), glm::vec2( 1.0f, 0.0f ) },
        { glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec2( 1.0f, 1.0f ) },
        { glm::vec3( -1.0f, 1.0f, 1.0f ), glm::vec2( 0.0f, 1.0f ) },
    };

	BufferStruct bufferstrct;
	bufferstrct.usage = USAGE_DEFAULT;
	bufferstrct.byteWidth = sizeof(SimpleVertex) * 24;
	bufferstrct.bindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferstrct.cpuAccessFlags = 0;
	SubresourceData subrsrcData;
	subrsrcData.psysMem = vertices;
	g_VertexBuffer.init(subrsrcData, bufferstrct);

    hr = g_pDevice->m_Device->CreateBuffer( &g_VertexBuffer.m_Buffer.m_bd, &g_VertexBuffer.m_Data, &g_VertexBuffer.m_Buffer.m_pBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
	g_DeviceContext->m_DeviceContext->IASetVertexBuffers( 0, 1, &g_VertexBuffer.m_Buffer.m_pBuffer, &stride, &offset );

    // Create index buffer
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

	bufferstrct.usage = USAGE_DEFAULT;
	bufferstrct.byteWidth = sizeof( WORD ) * 36;
	bufferstrct.bindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferstrct.cpuAccessFlags = 0;
	subrsrcData.psysMem = indices;
	g_IndexBuffer.init(subrsrcData, bufferstrct);
    hr = g_pDevice->m_Device->CreateBuffer( &g_IndexBuffer.m_Buffer.m_bd, &g_IndexBuffer.m_Data, &g_IndexBuffer.m_Buffer.m_pBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set index buffer
	g_DeviceContext->m_DeviceContext->IASetIndexBuffer(g_IndexBuffer.m_Buffer.m_pBuffer, DXGI_FORMAT_R16_UINT, 0 );

    // Set primitive topology
	g_DeviceContext->m_DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Create the constant buffers
	bufferstrct.usage = USAGE_DEFAULT;
	bufferstrct.byteWidth = sizeof(CBNeverChanges);
	bufferstrct.bindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferstrct.cpuAccessFlags = 0;
	g_CBNeverChanges.init(bufferstrct);
    hr = g_pDevice->m_Device->CreateBuffer( &g_CBNeverChanges.m_bd, NULL, &g_CBNeverChanges.m_pBuffer );
    if( FAILED( hr ) )
        return hr;
    
	bufferstrct.byteWidth = sizeof(CBChangeOnResize);
	g_CBChangeOnResize.init(bufferstrct);
    hr = g_pDevice->m_Device->CreateBuffer( &g_CBChangeOnResize.m_bd, NULL, &g_CBChangeOnResize.m_pBuffer );
    if( FAILED( hr ) )
        return hr;

	bufferstrct.byteWidth = sizeof(CBChangesEveryFrame);
	g_CBChangesEveryFrame.init(bufferstrct);
    hr = g_pDevice->m_Device->CreateBuffer( &g_CBChangesEveryFrame.m_bd, NULL, &g_CBChangesEveryFrame.m_pBuffer );
    if( FAILED( hr ) )
        return hr;

    // Load the Texture
	//hr = D3DX11CreateShaderResourceViewFromMemory(g_pDevice->m_Device, );
    hr = D3DX11CreateShaderResourceViewFromFile( g_pDevice->m_Device, L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL );
    if( FAILED( hr ) )
        return hr;

    // Create the sample state
	SAMPLER_DESC samplerDsc;
	ZeroMemory(&samplerDsc, sizeof(samplerDsc));
	samplerDsc.filter = FILTER_MIN_MAG_MIP_LINEAR;
	samplerDsc.addresU = TEXTURE_ADDRESS_WRAP;
	samplerDsc.addresV = TEXTURE_ADDRESS_WRAP;
	samplerDsc.addresW = TEXTURE_ADDRESS_WRAP;
	samplerDsc.comparisonFunc = COMPARISON_NEVER;
	samplerDsc.minLOD = 0;
	samplerDsc.maxLOD = D3D11_FLOAT32_MAX;

	g_SamplerState.init(samplerDsc);

    hr = g_pDevice->m_Device->CreateSamplerState( &g_SamplerState.m_Desc, &g_SamplerState.m_pSamplerLinear );
    if( FAILED( hr ) )
        return hr;
#endif
    // Initialize the world matrices
	g_World = glm::mat4(1.0f);

    // Initialize VM and PM of camera

	CameraDesc MyDesc;
	MyDesc.Pos = { 0.f, 1.f, -6.f };
	MyDesc.LAt = { 0.f, 1.f, 0.f };
	MyDesc.Up = { 0.f, 1.f, 0.f };
	MyDesc.FOV = PIDIV4;
	MyDesc.Width = width;
	MyDesc.Height = height;
	MyDesc.NearPlane = 0.01f;
	MyDesc.FarPlane = 100.f;

	g_Camera.init(MyDesc);
	anotherCamera.init(MyDesc);

	// Set camera VM

    CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = g_Camera.View;
#ifdef D3D11
	g_DeviceContext->m_DeviceContext->UpdateSubresource(g_CBNeverChanges.m_pBuffer, 0, NULL, &cbNeverChanges, 0, 0);
#endif // D3D11	

    // Initialize the camera PM
    
    CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = g_Camera.Proj;
#ifdef D3D11
	g_DeviceContext->m_DeviceContext->UpdateSubresource( g_CBChangeOnResize.m_pBuffer, 0, NULL, &cbChangesOnResize, 0, 0 );
#endif
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
#ifdef D3D11
    if( g_DeviceContext->m_DeviceContext) g_DeviceContext->m_DeviceContext->ClearState();
    if( g_SamplerState.m_pSamplerLinear ) g_SamplerState.m_pSamplerLinear->Release();
    if( g_pTextureRV ) g_pTextureRV->Release();
    if( g_CBNeverChanges.m_pBuffer ) g_CBNeverChanges.m_pBuffer->Release();
    if( g_CBChangeOnResize.m_pBuffer ) g_CBChangeOnResize.m_pBuffer->Release();
    if( g_CBChangesEveryFrame.m_pBuffer ) g_CBChangesEveryFrame.m_pBuffer->Release();
    if( g_VertexBuffer.m_Buffer.m_pBuffer) g_VertexBuffer.m_Buffer.m_pBuffer->Release();
    if( g_IndexBuffer.m_Buffer.m_pBuffer ) g_IndexBuffer.m_Buffer.m_pBuffer->Release();
    if( g_VertexShader.m_pInputLayout ) g_VertexShader.m_pInputLayout->Release();
    if( g_VertexShader.m_pVertexShader ) g_VertexShader.m_pVertexShader->Release();
    if( g_PixelShader.m_pPixelShader) g_PixelShader.m_pPixelShader->Release();
    if( g_DepthStencil.m_pTexture) g_DepthStencil.m_pTexture->Release();
    if( g_DepthStencilView.m_pDepthStencilView ) g_DepthStencilView.m_pDepthStencilView->Release();
    if( g_RenderTargetView.m_pRTV ) g_RenderTargetView.m_pRTV->Release();
    if( g_SwapChain->m_pSwapChain) g_SwapChain->m_pSwapChain->Release();
    if( g_DeviceContext->m_DeviceContext) g_DeviceContext->m_DeviceContext->Release();
    if( g_pDevice->m_Device ) g_pDevice->m_Device->Release();
#endif
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

		case WM_SIZE:
		{
#ifdef D3D11
			if (g_DeviceContext->m_DeviceContext != nullptr)
			{
				//Get new window dimensions
				RECT rc;
				GetClientRect(hWnd, &rc);
				UINT width = rc.right - rc.left;
				UINT height = rc.bottom - rc.top;
				//Regenerate world matrix as identity
				g_World = glm::mat4(1.0f);
				//Set w and h for camera
				g_Camera.setHeigth(height);
				g_Camera.setWidth(width);
				//Update projection matrix with new params
				g_Camera.updatePM();
				//Update CB
				CBChangeOnResize cbChangesOnResize;
				cbChangesOnResize.mProjection = g_Camera.Proj;

				g_DeviceContext->m_DeviceContext->UpdateSubresource(g_CBChangeOnResize.m_pBuffer, 0, NULL, &cbChangesOnResize, 0, 0);
				if (g_SwapChain->m_pSwapChain)
				{
					g_DeviceContext->m_DeviceContext->OMSetRenderTargets(0, 0, 0);
					g_RenderTargetView.m_pRTV->Release();
					HRESULT h;
					h = g_SwapChain->m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

					CBuffer tempBack;
					h = g_SwapChain->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tempBack.m_pBuffer);
					h = g_pDevice->m_Device->CreateRenderTargetView(tempBack.m_pBuffer, NULL, &g_RenderTargetView.m_pRTV);
					tempBack.m_pBuffer->Release();

					g_DepthStencil.m_pTexture->Release();
					Texture2DStruct DepthDesc;
					DepthDesc.W = width;
					DepthDesc.H = height;
					DepthDesc.mipLevels = 1;
					DepthDesc.arraySize = 1;
					DepthDesc.format = FORMAT_D24_UNORM_S8_UINT;
					DepthDesc.sampleDesc.count = 1;
					DepthDesc.sampleDesc.quality = 0;
					DepthDesc.usage = USAGE_DEFAULT;
					DepthDesc.flags = D3D11_BIND_DEPTH_STENCIL;
					DepthDesc.cpuAccessFlags = 0;
					DepthDesc.miscFlags = 0;

					g_DepthStencil.init(DepthDesc);

					h = g_pDevice->m_Device->CreateTexture2D(&g_DepthStencil.m_Desc, NULL, &g_DepthStencil.m_pTexture);

					DepthStencilViewStruct DSV;
					DSV.format = g_DepthStencil.m_Data.format;
					DSV.viewDimension = DSV_DIMENSION_TEXTURE2D;
					DSV.texture2D.mipSlice = 0;

					g_DepthStencilView.m_pDepthStencilView->Release();

					g_DepthStencilView.init(DSV, (FORMAT)g_DepthStencil.m_Desc.Format);

					h = g_pDevice->m_Device->CreateDepthStencilView(g_DepthStencil.m_pTexture, &g_DepthStencilView.m_Desc, &g_DepthStencilView.m_pDepthStencilView);

					g_DeviceContext->m_DeviceContext->OMSetRenderTargets(1, &g_RenderTargetView.m_pRTV, g_DepthStencilView.m_pDepthStencilView);

					ViewportStruct V;
					V.W = width;
					V.H = height;
					V.minDepth = 0.f;
					V.maxDepth = 1.f;
					V.topLeftX = 0;
					V.topLeftY = 0;

					CViewport ViewP;
					ViewP.init(V);
					g_DeviceContext->m_DeviceContext->RSSetViewports(1, &ViewP.m_Viewport);
				}

			}
#endif //D3D11
			break;
		}
		case WM_KEYDOWN:
		{
			g_Camera.getKeyPress(wParam);		
			break;			
		}
		case WM_KEYUP:
			g_Camera.getKeyRelease(wParam);
			break;
		case WM_LBUTTONDOWN:
		{
			POINT MousePoint;
			GetCursorPos(&MousePoint);
			g_Camera.mInitPos = {MousePoint.x, MousePoint.y, 0.f};
			g_Camera.mIsClicked = true;
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (g_Camera.mIsClicked)
			{
				POINT EndPoint;
				GetCursorPos(&EndPoint);
				g_Camera.mEndPos = {EndPoint.x, EndPoint.y, 0.f};
				SetCursorPos(g_Camera.mInitPos.x, g_Camera.mInitPos.y);
				g_Camera.mDir = g_Camera.mInitPos - g_Camera.mEndPos;
				g_Camera.rotate(g_Camera.mDir);
				CBNeverChanges cbNeverChanges;
				cbNeverChanges.mView = g_Camera.View;
#ifdef D3D11
				g_DeviceContext->m_DeviceContext->UpdateSubresource(g_CBNeverChanges.m_pBuffer, 0, NULL, &cbNeverChanges, 0, 0);
#endif
			}
			break;
		}
		case WM_LBUTTONUP:
			g_Camera.mIsClicked = false;
			break;
        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
#ifdef D3D11
    // Update our time
    static float t = 0.0f;
    if( g_pDevice->m_struc.m_DriverType == D3D_DRIVER_TYPE_REFERENCE )
    {
        t += ( float )XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if( dwTimeStart == 0 )
            dwTimeStart = dwTimeCur;
        t = ( dwTimeCur - dwTimeStart ) / 1000.0f;
    }

	if (g_Camera.mForward || g_Camera.mBack || g_Camera.mLeft || g_Camera.mRight || g_Camera.mUp || g_Camera.mDown || g_Camera.mRotateLeft || g_Camera.mRotateRight)
	{
		g_Camera.move();
		g_Camera.rotate();
		CBNeverChanges cbNeverChanges;
		cbNeverChanges.mView = g_Camera.View;
		g_DeviceContext->m_DeviceContext->UpdateSubresource(g_CBNeverChanges.m_pBuffer, 0, NULL, &cbNeverChanges, 0, 0);
	}

    // Rotate cube around the origin
    //g_World = XMMatrixRotationY( t );
	//g_World = glm::rotate(g_World, .001f, {0, 1, 0});

    // Modify the color
	g_MeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
	g_MeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
	g_MeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

    //
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	g_DeviceContext->m_DeviceContext->ClearRenderTargetView(g_RenderTargetView.m_pRTV, ClearColor );

    //
    // Clear the depth buffer to 1.0 (max depth)
    //
	g_DeviceContext->m_DeviceContext->ClearDepthStencilView( g_DepthStencilView.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

    //
    // Update variables that change once per frame
    //
	for (int i = 0; i < 32; i++)
	{
		g_World = glm::mat4(1.f);
		g_World = glm::translate(g_World, Lab[i]);

		CBChangesEveryFrame cb;
		cb.mWorld = glm::transpose(g_World);
		cb.vMeshColor = g_MeshColor;
		g_DeviceContext->m_DeviceContext->UpdateSubresource(g_CBChangesEveryFrame.m_pBuffer, 0, NULL, &cb, 0, 0);

		g_DeviceContext->m_DeviceContext->VSSetShader(g_VertexShader.m_pVertexShader, NULL, 0);
		g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(0, 1, &g_CBNeverChanges.m_pBuffer);
		g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(1, 1, &g_CBChangeOnResize.m_pBuffer);
		g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(2, 1, &g_CBChangesEveryFrame.m_pBuffer);
		g_DeviceContext->m_DeviceContext->PSSetShader( g_PixelShader.m_pPixelShader, NULL, 0);
		g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(2, 1, &g_CBChangesEveryFrame.m_pBuffer);
		g_DeviceContext->m_DeviceContext->PSSetShaderResources(0, 1, &g_pTextureRV);
		g_DeviceContext->m_DeviceContext->PSSetSamplers(0, 1, &g_SamplerState.m_pSamplerLinear);
		g_DeviceContext->m_DeviceContext->DrawIndexed(36, 0, 0);
	}

    //
    // Present our back buffer to our front buffer
    //
	g_SwapChain->m_pSwapChain->Present( 0, 0 );
#endif
}