//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#define FIRSTW 800.f
#define FIRSTH 600.f

#include <vector>
#include "resource.h"
#include "CCameraFP.h"
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
#include "amgui/imgui.h"

#include "CGraphicsAPI.h"
#include "CPass.h"
#include <iostream>

#ifdef OPENGL
#include "amgui/imgui_impl_glfw.h"
#include "amgui/imgui_impl_opengl3.h"
#include "CModel.h"
#elif D3D11
#include "amgui/imgui_impl_dx11.h"
#include "amgui/imgui_impl_win32.h"
#endif // OPENGL


//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

long long GetCurrentTimeMillis()
{
#ifdef WIN32
	return GetTickCount();
#endif // WIN32
}

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
ID3D11ShaderResourceView*           InactiveSRV = NULL;
#endif

CCamera								g_Camera;
CCameraFP							FirstPerson;
CTexture2D							InactiveCameraTexture;

CDepthStencilView					DepthStencilViewFP;
CRenderTargetView					SecondRTV;
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
CVertexBuffer						g_BoardVB;
CIndexBuffer						g_BoardIB;
CTexture2D							g_DepthStencil;
CDepthStencilView					DepthStencilViewFree;
CVertexShader						g_VertexShader;
CPixelShader						g_PixelShader;
CRenderTargetView					g_RenderTargetView;
CSamplerState						g_SamplerState;

CCamera *							ActiveCamera = NULL;
CCamera *							InactiveCamera = NULL;

unsigned int						imguiWindowW;
unsigned int						imguiWindowH;

glm::vec4							g_LightDir;
glm::vec4							g_PointPos;
glm::vec4							g_PointAtt;
glm::vec3							g_LightDir3 = { 0.f, -1.f, 0.f };
glm::vec4							g_LightColor;
CPass								DiffusePass;	//GBuffer

CPass								SSAOPass;
CBuffer								SSAOBuffer;
SSAOCB								SSAOConsBuff;

CPass								SkyboxPass;

CPass								LightPass;
CBuffer								LightBuffer;
LightCB								LightConsBuff;
ID3D11ShaderResourceView *			irradiancesrv;

CPass								LuminancePass;

CPass								BrightPass;
CBuffer								BrightBuffer;
BrightCB							BrightConsBuff;

CPass								BlurH1;
CBuffer								BlurH1Buffer;
BlurCB								BlurH1ConsBuff;

CPass								BlurH2;
CBuffer								BlurH2Buffer;
BlurCB								BlurH2ConsBuff;

CPass								BlurH3;
CBuffer								BlurH3Buffer;
BlurCB								BlurH3ConsBuff;

CPass								BlurH4;
CBuffer								BlurH4Buffer;
BlurCB								BlurH4ConsBuff;

CPass								BlurV1;
CBuffer								BlurV1Buffer;
BlurCB								BlurV1ConsBuff;

CPass								BlurV2;
CBuffer								BlurV2Buffer;
BlurCB								BlurV2ConsBuff;

CPass								BlurV3;
CBuffer								BlurV3Buffer;
BlurCB								BlurV3ConsBuff;

CPass								BlurV4;
CBuffer								BlurV4Buffer;
BlurCB								BlurV4ConsBuff;

CPass								AddBright1;
CBuffer								AddBright1Buffer;
AddBrightCB							AddBright1ConsBuff;

CPass								AddBright2;
CBuffer								AddBright2Buffer;
AddBrightCB							AddBright2ConsBuff;

CPass								AddBright3;
CBuffer								AddBright3Buffer;
AddBrightCB							AddBright3ConsBuff;

CPass								AddBright4;
CBuffer								AddBright4Buffer;
AddBrightCB							AddBright4ConsBuff;

CPass								ToneMapPass;
CBuffer								ToneMapBuffer;
ToneMapCB							ToneMapConsBuff;

ID3D11DepthStencilState*			g_GBufferDSS;
ID3D11DepthStencilState*			g_SAQDDSS;

ID3D11RasterizerState*				g_GBufferRS;
ID3D11RasterizerState*				g_SAQDRS;

CBuffer								BoneBuffer;

glm::vec3 boardpos(-5, 1, 0);

long long m_startTime;

float GetRunningTime()
{
	float RunningTime = (float)((double)GetCurrentTimeMillis() - (double)m_startTime) / 1000.0f;
	return RunningTime;
}

#ifdef D3D11
ID3D11Device * ptrDevice = static_cast<ID3D11Device*>(g_pDevice->getDevice());
ID3D11DeviceContext * ptrDC = static_cast<ID3D11DeviceContext*>(g_DeviceContext->getDeviceContext());
IDXGISwapChain* ptrSC = static_cast<IDXGISwapChain*>(g_SwapChain->getSwapChain());

CGraphicsAPI						graphicApi;
CSceneManager						SCManager;
CSceneManager						SCAQ;
CSceneManager						Cube;
Assimp::Importer*					MyImporter = new Assimp::Importer();
const aiScene*						MyScene;
#elif OPENGL
unsigned int framebuffer;
unsigned int textureColorbuffer;
unsigned int rbo;
#endif // D3D11

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

#ifdef D3D11
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice();
void CleanupDevice();
void Render();

#elif OPENGL
const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"

"out vec2 TexCoords;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
	"gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
	"TexCoords = aTexCoords;\n"
	"Normal = aNormal;\n"
"}\n";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"in vec3 Normal;\n"
"uniform sampler2D texture_diffuse1;\n"
"uniform vec3 lightDir;\n"
"void main()\n"
"{\n"
	"vec3 light = normalize(-lightDir);\n"
	"float Ndl = dot(Normal, light);\n"
	"FragColor = texture(texture_diffuse1, TexCoords) * Ndl;\n"
"}\n;";

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, heigth);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, width, heigth);

	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, heigth);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	InactiveCamera->setWidth(width);
	InactiveCamera->setHeigth(heigth);
	ActiveCamera->setWidth(width);
	ActiveCamera->setHeigth(heigth);
	InactiveCamera->updatePM();
	ActiveCamera->updatePM();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	imguiWindowW = width;
	imguiWindowH = heigth;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Close app
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	//Key presses
	if (action == GLFW_PRESS)
	{
		ActiveCamera->getKeyPress(key);
	}
	//Key release
	if (action == GLFW_RELEASE)
	{
		ActiveCamera->getKeyRelease(key);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			ActiveCamera->mInitPos = { xpos, ypos, 0.f };
			ActiveCamera->mIsClicked = true;
		}
		else if (action == GLFW_RELEASE)
		{
			ActiveCamera->mIsClicked = false;
		}
	}
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (ActiveCamera->mIsClicked)
	{
		ActiveCamera->mEndPos = { xpos, ypos, 0.f };
		glfwSetCursorPos(window, ActiveCamera->mInitPos.x, ActiveCamera->mInitPos.y);
		ActiveCamera->mDir = ActiveCamera->mInitPos - ActiveCamera->mEndPos;
		ActiveCamera->rotate(ActiveCamera->mDir);
	}
}

#endif

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
			offset += 8;
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
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
#ifdef D3D11
	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	if (FAILED(InitDevice()))
	{
		CleanupDevice();
		return 0;
	}

	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			ImVec2 ImgDimension(imguiWindowW / 5, imguiWindowH / 5);

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Passes", nullptr, 8);
			{
				ImGui::SetWindowSize(ImVec2(0, 0));

				if (ImGui::CollapsingHeader("GBuffer"))
				{
					for (int i = 0; i < DiffusePass.m_PassOutput.size(); i++)
					{
						ImGui::Image(DiffusePass.m_PassOutput[i], ImgDimension);
					}					
				}
				if (ImGui::CollapsingHeader("SSAO"))
				{
					for (int i = 0; i < SSAOPass.m_PassOutput.size(); i++)
					{
						ImGui::Image(SSAOPass.m_PassOutput[i], ImgDimension);
					}
					ImGui::SliderFloat("kIntensity", &SSAOConsBuff.kIntensity, 0.0f, 50.0f, "%.5f");
					ImGui::SliderFloat("kScale", &SSAOConsBuff.kScale, 0.0f, 10.0f, "%.1f");
					ImGui::SliderFloat("kBias", &SSAOConsBuff.kBias, 0.0f, 1.0f, "%.10f");
					ImGui::SliderFloat("kSample", &SSAOConsBuff.kSample, 0.0f, 32.0f, "%.5f");
				}
				if (ImGui::CollapsingHeader("Skybox"))
				{
					for (int i = 0; i < SkyboxPass.m_PassOutput.size(); i++)
					{
						ImGui::Image(SkyboxPass.m_PassOutput[i], ImgDimension);
					}
				}
				if (ImGui::CollapsingHeader("Light"))
				{
					for (int i = 0; i < LightPass.m_PassOutput.size(); i++)
					{
						ImGui::Image(LightPass.m_PassOutput[i], ImgDimension);
					}
					ImGui::SliderFloat("kDiffuse", &LightConsBuff.kDiffuse, 0.0f, 1.0f, "%.10f");
					ImGui::SliderFloat("kAmbient", &LightConsBuff.kAmbient, 0.0f, 1.0f, "%.10f");
					ImGui::SliderFloat("kSpecular", &LightConsBuff.kSpecular, 0.0f, 10.0f, "%.5f");
					ImGui::SliderFloat("SpecularPower", &LightConsBuff.specularPower, 0.0f, 32.0f, "%.5f");
					ImGui::SliderFloat3("LightDir", (float*)&LightConsBuff.lightDir, -1.f, 1.f, "%.10f");
					ImGui::SliderFloat3("LightColor", (float*)&LightConsBuff.lightColor, 0.f, 1.f, "%.1f");
				}
				if (ImGui::CollapsingHeader("Luminance"))
				{
					for (int i = 0; i < LuminancePass.m_PassOutput.size(); i++)
					{
						ImGui::Image(LuminancePass.m_PassOutput[i], ImgDimension);
					}
				}
				if (ImGui::CollapsingHeader("Bright"))
				{
					for (int i = 0; i < BrightPass.m_PassOutput.size(); i++)
					{
						ImGui::Image(BrightPass.m_PassOutput[i], ImgDimension);
					}
					ImGui::SliderInt("mipLevel", &BrightConsBuff.mipLevel, 0, 5);
					ImGui::SliderFloat("Threshold", (float*)&BrightConsBuff.Threshold.x, 0.f, 1.f, "%.10f");
				}
				if (ImGui::CollapsingHeader("BlurH"))
				{
					for (int i = 0; i < BlurH1.m_PassOutput.size(); i++)
					{
						ImGui::Image(BlurH1.m_PassOutput[i], ImgDimension);
					}
					ImGui::SliderInt("mipLevelH1", &BlurH1ConsBuff.mipLevel, 0, 5);
					ImGui::SliderInt("mipLevelH2", &BlurH2ConsBuff.mipLevel, 0, 5);
					ImGui::SliderInt("mipLevelH3", &BlurH3ConsBuff.mipLevel, 0, 5);
					ImGui::SliderInt("mipLevelH4", &BlurH4ConsBuff.mipLevel, 0, 5);
				}
				if (ImGui::CollapsingHeader("BlurV"))
				{
					for (int i = 0; i < BlurV1.m_PassOutput.size(); i++)
					{
						ImGui::Image(BlurV1.m_PassOutput[i], ImgDimension);
					}
					ImGui::SliderInt("mipLevelV1", &BlurV1ConsBuff.mipLevel, 0, 5);
					ImGui::SliderInt("mipLevelV2", &BlurV2ConsBuff.mipLevel, 0, 5);
					ImGui::SliderInt("mipLevelV3", &BlurV3ConsBuff.mipLevel, 0, 5);
					ImGui::SliderInt("mipLevelV4", &BlurV4ConsBuff.mipLevel, 0, 5);
				}
				if (ImGui::CollapsingHeader("AddBright1"))
				{
					for (int i = 0; i < AddBright1.m_PassOutput.size(); i++)
					{
						ImGui::Image(AddBright1.m_PassOutput[i], ImgDimension);
					}
					ImGui::SliderInt("mipLevelAB1", &AddBright1ConsBuff.mipLevel.x, 0, 5);
					ImGui::SliderInt("mipLevelAB2", &AddBright2ConsBuff.mipLevel.x, 0, 5);
					ImGui::SliderInt("mipLevelAB3", &AddBright3ConsBuff.mipLevel.x, 0, 5);
					ImGui::SliderInt("mipLevelAB4", &AddBright4ConsBuff.mipLevel.x, 0, 5);
				}
				if (ImGui::CollapsingHeader("ToneMap"))
				{
					for (int i = 0; i < ToneMapPass.m_PassOutput.size(); i++)
					{
						ImGui::Image(ToneMapPass.m_PassOutput[i], ImgDimension);
					}
					ImGui::SliderFloat("kExposure", &ToneMapConsBuff.kExposure, 0.f, 20.f, "%.5f");
					ImGui::SliderFloat("kBloom", &ToneMapConsBuff.kBloom, 0.f, 10.f, "%.5f");
					ImGui::SliderInt("colorSpace", &ToneMapConsBuff.colorSpace, 0, 3);
				}
				ImGui::GetIO().FontGlobalScale;
			}
			ImGui::End();
			Render();
		}
	}
	CleanupDevice();
	return (int)msg.wParam;
#else

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Tutorial 07", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	//Callbacks set
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	glEnable(GL_DEPTH_TEST);
	//Vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//VS error check
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	}
	//Pixel Shader
	int pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(pixelShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(pixelShader);
	//PS error check
	glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(pixelShader, 512, NULL, infoLog);
	}
	//Link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, pixelShader);
	glLinkProgram(shaderProgram);
	//Linking error check
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);

	CameraDesc MyDesc;
	MyDesc.Pos = { 0.f, 0.f, 3.f };
	MyDesc.LAt = { 0.f, 0.f, 0.f };
	MyDesc.Up = { 0.f, 1.f, 0.f };
	MyDesc.FOV = PIDIV4;
	MyDesc.Width = 800;
	MyDesc.Height = 600;
	MyDesc.NearPlane = 0.01f;
	MyDesc.FarPlane = 100.f;

	g_Camera.init(MyDesc);
	FirstPerson.init(MyDesc);

	ActiveCamera = &g_Camera;
	InactiveCamera = &FirstPerson;

	CModel nano("Models/Scene/Scene.fbx");

	//Framebuffer configuration
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//Create a color attachment texture	
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FIRSTW, FIRSTH, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	//Create a RenderBufferObject for depth and stencil attachment	
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, FIRSTW, FIRSTH);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	//Check for errors
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return -1;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	imguiWindowH = FIRSTH;
	imguiWindowW = FIRSTW;

	float vertices[] = {
		// positions       // texture coords    //Normals
		 1.f,  1.f, 0.0f,  0.0f, 0.0f,			1.f,  1.f, 0.0f,// top right
		 1.f, -1.f, 0.0f,  0.0f, 1.0f,			1.f, -1.f, 0.0f,// bottom right
		-1.f, -1.f, 0.0f,  1.0f, 1.0f,			-1.f, -1.f, 0.0f,// bottom left
		-1.f,  1.f, 0.0f,  1.0f, 0.0f,			-1.f,  1.f, 0.0f// top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	glm::vec3 boardpos(-5,1,0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		//Update
		if (ActiveCamera->mForward || ActiveCamera->mBack || ActiveCamera->mLeft || ActiveCamera->mRight || ActiveCamera->mUp || ActiveCamera->mDown)
		{
			ActiveCamera->move();
		}
		if (ActiveCamera->mRotateLeft || ActiveCamera->mRotateRight)
		{
			ActiveCamera->rotate();
		}

		//Render
		//Bind to Framebuffer and draw inactive camera
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		//Clear FBO content and set background color
		glClearColor(0.f, 0.5f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Use linked shader
		glUseProgram(shaderProgram);
		//Create matrix for 3D rendering
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		//glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 view = glm::transpose(InactiveCamera->View);
		//glm::mat4 projection = glm::mat4(1.0f);

		glm::mat4 projection = glm::scale(InactiveCamera->Proj, glm::vec3(1, -1, 1));

		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		
		//Get uniforms from shader
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		unsigned int lightLoc = glGetUniformLocation(shaderProgram, "lightDir");
		
		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
		glUniform3fv(lightLoc, 1, &g_LightDir3[0]);

		//Draw models
		nano.Draw(shaderProgram);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.f, 0.5f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 boardLook = glm::normalize(ActiveCamera->getPos() - boardpos);
		glm::vec3 boardRight = glm::cross(glm::normalize(ActiveCamera->Up), boardLook);
		glm::vec3 boardUp = glm::cross(boardLook, boardRight);

		glm::mat4 boarMat(boardRight.x, boardRight.y, boardRight.z, 0, boardUp.x, boardUp.y, boardUp.z, 0, boardLook.x, boardLook.y, boardLook.z, 0, boardpos.x, boardpos.y, boardpos.z, 1);

		model = glm::mat4(1.0f);
		view = glm::transpose(ActiveCamera->View);
		projection = ActiveCamera->Proj;

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
		glUniform3fv(lightLoc, 1, &g_LightDir3[0]);

		nano.Draw(shaderProgram);
		glBindVertexArray(VAO);

		model = boarMat;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Draw ImGui elements
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Inactive Camera", nullptr, 8);
		{
			ImGui::SetWindowSize(ImVec2(0,0));
			ImGui::Image((void*)textureColorbuffer, ImVec2(imguiWindowW / 4, imguiWindowH / 4));
			if (ImGui::Button("Change Camera"))
			{
				CCamera * temp = ActiveCamera;
				ActiveCamera = InactiveCamera;
				InactiveCamera = temp;
			}
			ImGui::SliderFloat("X", &g_LightDir3.x, -1.f, 1.f, "%.2f", .5f);
			ImGui::SliderFloat("Y", &g_LightDir3.y, -1.f, 1.f, "%.2f", .5f);
			ImGui::SliderFloat("Z", &g_LightDir3.z, -1.f, 1.f, "%.2f", .5f);
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);		
	}
	//Free resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
#endif   
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

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
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

HRESULT CompileShaders(WCHAR* shaderfile, const char* vertexEntryPoint, const char* pixelEntryPoint, CVertexShader &VS, CPixelShader &PS)
{
	// Compile the vertex shader
	HRESULT hr = CompileShaderFromFile(shaderfile, vertexEntryPoint, "vs_4_0", &VS.m_pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	//// Create the vertex shader
	hr = ptrDevice->CreateVertexShader(VS.m_pVSBlob->GetBufferPointer(), VS.m_pVSBlob->GetBufferSize(), NULL, &VS.m_pVertexShader);
	if (FAILED(hr))
	{
		VS.m_pVSBlob->Release();
		return hr;
	}

	//Create input layout from compiled VS
	hr = CreateInputLayoutDescFromVertexShaderSignature(VS.m_pVSBlob, ptrDevice, &VS.m_pInputLayout);
	if (FAILED(hr))
		return hr;

	// Compile the pixel shader
	hr = CompileShaderFromFile(shaderfile, pixelEntryPoint, "ps_4_0", &PS.m_pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = ptrDevice->CreatePixelShader(PS.m_pPSBlob->GetBufferPointer(), PS.m_pPSBlob->GetBufferSize(), NULL, &PS.m_pPixelShader);
	PS.m_pPSBlob->Release();
	if (FAILED(hr))
		return hr;
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	unsigned int width = imguiWindowW = rc.right - rc.left;
	unsigned int height = imguiWindowH = rc.bottom - rc.top;

#ifdef D3D11
	DeviceStruct T;

	T.m_DeviceFlags = 0;
#ifdef _DEBUG
	T.m_DeviceFlags |= 2;		//D3D11_CREATE_DEVICE_DEBUG;
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
	S.outputWND = (unsigned int)g_hWnd;
	S.count = 1;
	S.quality = 0;
	S.windowed = TRUE;

	g_SwapChain->init(S);

	for (unsigned int driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_pDevice->m_struc.m_DriverType = g_pDevice->m_struc.m_DriverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, (D3D_DRIVER_TYPE)g_pDevice->m_struc.m_DriverType, NULL, g_pDevice->m_struc.m_DeviceFlags, (D3D_FEATURE_LEVEL *)g_pDevice->m_struc.m_FeatureLevels, g_pDevice->m_struc.m_numFeatureLevels,
			D3D11_SDK_VERSION, &g_SwapChain->m_sd, &ptrSC, &ptrDevice, (D3D_FEATURE_LEVEL *)&featureLevel, &ptrDC);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Create a render target view
	CTexture2D BackBuffer;
	hr = ptrSC->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer.m_pTexture);
	if (FAILED(hr))
		return hr;

	hr = ptrDevice->CreateRenderTargetView(BackBuffer.m_pTexture, NULL, &g_RenderTargetView.m_pRTV);
	BackBuffer.m_pTexture->Release();
	if (FAILED(hr))
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
	DepthDesc.flags = 64;				//D3D11_BIND_DEPTH_STENCIL;
	DepthDesc.cpuAccessFlags = 0;
	DepthDesc.miscFlags = 0;

	g_DepthStencil.init(DepthDesc);

	hr = ptrDevice->CreateTexture2D(&g_DepthStencil.m_Desc, NULL, &g_DepthStencil.m_pTexture);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	DepthStencilViewStruct DSV;
	DSV.format = g_DepthStencil.m_Data.format;
	DSV.viewDimension = DSV_DIMENSION_TEXTURE2D;
	DSV.texture2D.mipSlice = 0;

	DepthStencilViewFree.init(DSV, (FORMAT)g_DepthStencil.m_Desc.Format);

	hr = ptrDevice->CreateDepthStencilView(g_DepthStencil.m_pTexture, &DepthStencilViewFree.m_Desc, &DepthStencilViewFree.m_pDepthStencilView);
	if (FAILED(hr))
		return hr;

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
	ptrDC->RSSetViewports(1, &tempVP.m_Viewport);



	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f , -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(1.0f, 1.0f , 1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
	};

	BufferStruct bufferstrct;
	bufferstrct.usage = USAGE_DEFAULT;
	bufferstrct.byteWidth = sizeof(SimpleVertex) * 24;
	bufferstrct.bindFlags = 1;			// D3D11_BIND_VERTEX_BUFFER;
	bufferstrct.cpuAccessFlags = 0;

	SubresourceData subrsrcData;
	subrsrcData.psysMem = vertices;
	g_VertexBuffer.init(subrsrcData, bufferstrct);

	// Create the constant buffers
	bufferstrct.usage = USAGE_DEFAULT;
	bufferstrct.byteWidth = sizeof(CBNeverChanges);
	bufferstrct.bindFlags = 4;		// D3D11_BIND_CONSTANT_BUFFER;
	bufferstrct.cpuAccessFlags = 0;

	//NeverChanges

	//Free Camera
	g_Camera.m_CBNeverChanges.init(bufferstrct);
	hr = ptrDevice->CreateBuffer(&g_Camera.m_CBNeverChanges.m_bd, NULL, &g_Camera.m_CBNeverChanges.m_pBuffer);
	if (FAILED(hr))
		return hr;

	//FPS Camera
	FirstPerson.m_CBNeverChanges.init(bufferstrct);
	hr = ptrDevice->CreateBuffer(&FirstPerson.m_CBNeverChanges.m_bd, NULL, &FirstPerson.m_CBNeverChanges.m_pBuffer);
	if (FAILED(hr))
		return hr;

	//ChangeOnResize
	bufferstrct.byteWidth = sizeof(CBChangeOnResize);
	//Free Camera
	g_Camera.m_CBChangesOnResize.init(bufferstrct);
	hr = ptrDevice->CreateBuffer(&g_Camera.m_CBChangesOnResize.m_bd, NULL, &g_Camera.m_CBChangesOnResize.m_pBuffer);
	if (FAILED(hr))
		return hr;

	//FPS Camera
	FirstPerson.m_CBChangesOnResize.init(bufferstrct);
	hr = ptrDevice->CreateBuffer(&FirstPerson.m_CBChangesOnResize.m_bd, NULL, &FirstPerson.m_CBChangesOnResize.m_pBuffer);
	if (FAILED(hr))
		return hr;

	//ChangeEveryFrame
	bufferstrct.byteWidth = sizeof(CBChangesEveryFrame);
	//Free Camera
	g_Camera.m_CBChangesEveryFrame.init(bufferstrct);
	hr = ptrDevice->CreateBuffer(&g_Camera.m_CBChangesEveryFrame.m_bd, NULL, &g_Camera.m_CBChangesEveryFrame.m_pBuffer);
	if (FAILED(hr))
		return hr;

	//FPS Camera
	FirstPerson.m_CBChangesEveryFrame.init(bufferstrct);
	hr = ptrDevice->CreateBuffer(&FirstPerson.m_CBChangesEveryFrame.m_bd, NULL, &FirstPerson.m_CBChangesEveryFrame.m_pBuffer);
	if (FAILED(hr))
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

	hr = ptrDevice->CreateSamplerState(&g_SamplerState.m_Desc, &g_SamplerState.m_pSamplerLinear);
	if (FAILED(hr))
		return hr;

#endif
	// Initialize the world matrix
	g_World = glm::mat4(1.0f);

	// Initialize VM and PM of cameras
	CameraDesc MyDesc;
	MyDesc.Pos = { 10.f, 0.f, 0.f };
	MyDesc.LAt = { 0.f, 0.f, 0.f };
	MyDesc.Up = { 0.f, 1.f, 0.f };
	MyDesc.FOV = PIDIV4;
	MyDesc.Width = width;
	MyDesc.Height = height;
	MyDesc.NearPlane = 0.01f;
	MyDesc.FarPlane = 100.f;

	g_Camera.init(MyDesc);
	FirstPerson.init(MyDesc);

	// Set Free Camera VM

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = g_Camera.View;
#ifdef D3D11
	ptrDC->UpdateSubresource(g_Camera.m_CBNeverChanges.m_pBuffer, 0, NULL, &cbNeverChanges, 0, 0);
#endif // D3D11	

	//Set FPS Camera VM
	CBNeverChanges cbNeverChanges2;
	cbNeverChanges2.mView = FirstPerson.View;
#ifdef D3D11
	ptrDC->UpdateSubresource(FirstPerson.m_CBNeverChanges.m_pBuffer, 0, NULL, &cbNeverChanges2, 0, 0);
#endif // D3D11

	// Initialize Free Camera PM    
	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = glm::transpose(g_Camera.Proj);
#ifdef D3D11
	ptrDC->UpdateSubresource(g_Camera.m_CBChangesOnResize.m_pBuffer, 0, NULL, &cbChangesOnResize, 0, 0);
#endif

	//Initialize FPS Camera PM
	CBChangeOnResize cbChangesOnResize2;
	cbChangesOnResize2.mProjection = glm::transpose(FirstPerson.Proj);
#ifdef D3D11
	ptrDC->UpdateSubresource(FirstPerson.m_CBChangesOnResize.m_pBuffer, 0, NULL, &cbChangesOnResize2, 0, 0);
#endif

	//Initialize texture, SRV and RTV for inactive camera

	//Texture
	Texture2DStruct D;
	ZeroMemory(&D, sizeof(D));
	D.W = width;
	D.H = height;
	D.mipLevels = D.arraySize = 1;
	D.format = FORMAT_R16G16B16A16_FLOAT;
	D.sampleDesc.count = 1;
	D.usage = USAGE_DEFAULT;
	D.flags = 8 | 32;		//D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	D.cpuAccessFlags = 0;	//D3D11_CPU_ACCESS_WRITE;
	D.miscFlags = 0;

	InactiveCameraTexture.init(D);
#ifdef D3D11
	hr = ptrDevice->CreateTexture2D(&InactiveCameraTexture.m_Desc, NULL, &InactiveCameraTexture.m_pTexture);
	if (FAILED(hr))
		return hr;
#endif

	//Render Target View
	RenderTargetViewStruct rtDesc;
	rtDesc.format = D.format;
	rtDesc.viewDimension = RTV_DIMENSION_TEXTURE2D;
	rtDesc.texture2D.mipSlice = 0;

	SecondRTV.init(rtDesc);
#ifdef D3D11
	hr = ptrDevice->CreateRenderTargetView(InactiveCameraTexture.m_pTexture, &SecondRTV.m_Desc, &SecondRTV.m_pRTV);
	if (FAILED(hr))
		return hr;
#endif

#ifdef D3D11
	D3D11_SHADER_RESOURCE_VIEW_DESC view;
	view.Format = (DXGI_FORMAT)D.format;
	view.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view.Texture2D.MostDetailedMip = 0;
	view.Texture2D.MipLevels = 1;

	hr = ptrDevice->CreateShaderResourceView(InactiveCameraTexture.m_pTexture, &view, &InactiveSRV);
	if (FAILED(hr))
		return hr;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(ptrDevice, ptrDC);
	ImGui::StyleColorsDark();

	g_pDevice->m_Device = ptrDevice;
	g_DeviceContext->m_DeviceContext = ptrDC;
	//Scene model
	graphicApi.loadMesh("Models/drakefire_pistol_low.obj", &SCManager, graphicApi.m_Model, g_DeviceContext, g_pDevice, MyImporter, "Models/base_albedo.jpg", "Models/base_normal.jpg", "Models/base_metallic.jpg", false);

	//Screen aligned quad
	graphicApi.loadMesh("Models/ScreenAlignedQuad.3ds", &SCAQ, graphicApi.m_Model, g_DeviceContext, g_pDevice, MyImporter, "", "", "", false);

#endif

	//Set active and inactive camera
	ActiveCamera = &g_Camera;
	InactiveCamera = &FirstPerson;

	//Initialize  Rasterizer state
	D3D11_RASTERIZER_DESC	rsdesc;
	ZeroMemory(&rsdesc, sizeof(rsdesc));
	rsdesc.FillMode = D3D11_FILL_SOLID;
	rsdesc.CullMode = D3D11_CULL_FRONT;
	rsdesc.FrontCounterClockwise = true;

	hr = g_pDevice->m_Device->CreateRasterizerState(&rsdesc, &g_GBufferRS);

	if (FAILED(hr))
	{
		return hr;
	}

	rsdesc.CullMode = D3D11_CULL_NONE;

	hr = g_pDevice->m_Device->CreateRasterizerState(&rsdesc, &g_SAQDRS);

	if (FAILED(hr))
	{
		return hr;
	}

	//Initialize Depth stencil state

	D3D11_DEPTH_STENCIL_DESC dsdesc;
	ZeroMemory(&dsdesc, sizeof(dsdesc));
	dsdesc.StencilEnable = true;
	dsdesc.DepthEnable = true;

	dsdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsdesc.DepthFunc = D3D11_COMPARISON_LESS;

	dsdesc.StencilReadMask = 0xFF;
	dsdesc.StencilWriteMask = 0xFF;

	dsdesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsdesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dsdesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsdesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = g_pDevice->m_Device->CreateDepthStencilState(&dsdesc, &g_GBufferDSS);

	if (FAILED(hr))
	{
		return hr;
	}

	dsdesc.StencilEnable = false;
	dsdesc.DepthEnable = false;

	hr = g_pDevice->m_Device->CreateDepthStencilState(&dsdesc, &g_SAQDDSS);

	if (FAILED(hr))
	{
		return hr;
	}

	//Initialize passes

	

	//GBuffer
	StructPass P;
	P.DeviceContext = g_DeviceContext;
	P.RTVcount = 5;
	P.Device = g_pDevice;
	P.TextureStruct = D;
	ZeroMemory(&P.rstate, sizeof(P.rstate));
	P.rstate.FillMode = D3D11_FILL_SOLID;
	P.rstate.CullMode = D3D11_CULL_FRONT;
	P.rstate.FrontCounterClockwise = true;

	CompileShaders(L"Tutorial07.fx", "vs_main", "ps_main", DiffusePass.m_pVS, DiffusePass.m_pPS);

	DiffusePass.init(P, 1);

	//SSAO
	P.RTVcount = 1;
	P.rstate.CullMode = D3D11_CULL_NONE;
	CompileShaders(L"SSAO.fx", "vs_main", "ps_main", SSAOPass.m_pVS, SSAOPass.m_pPS);

	SSAOPass.init(P, 1);

	BufferStruct ssaoBS;
	ssaoBS.usage = USAGE_DEFAULT;
	ssaoBS.byteWidth = sizeof(SSAOCB);
	ssaoBS.bindFlags = 4;
	ssaoBS.cpuAccessFlags = 0;

	SSAOBuffer.init(ssaoBS);
	hr = ptrDevice->CreateBuffer(&SSAOBuffer.m_bd, NULL, &SSAOBuffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	SSAOCB CB;
	SSAOConsBuff.kIntensity = 8.2f;
	SSAOConsBuff.kScale = .5f;
	SSAOConsBuff.kBias = .1f;
	SSAOConsBuff.kSample = 2.4f;

	SSAOPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[0]);
	SSAOPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[2]);

	//Skybox
	CompileShaders(L"Skybox.fx", "vs_main", "ps_main", SkyboxPass.m_pVS, SkyboxPass.m_pPS);
	SkyboxPass.init(P, 1);

	graphicApi.loadMesh("Models/Sphere.3ds", &Cube, graphicApi.m_Model, g_DeviceContext, g_pDevice, MyImporter, "", "", "", false);

	D3DX11_IMAGE_LOAD_INFO loadinfo;
	loadinfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* texture;
	hr = D3DX11CreateTextureFromFile(g_pDevice->m_Device, L"Models/grace_cube.dds", &loadinfo, 0, (ID3D11Resource**)&texture, 0);

	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.Format = textureDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MipLevels = textureDesc.MipLevels;
	viewDesc.TextureCube.MostDetailedMip = 0;

	hr = g_pDevice->m_Device->CreateShaderResourceView(texture, &viewDesc, &Cube.getMesh(0)->m_Materials->m_TextureDiffuse);

	if (FAILED(hr))
	{
		return hr;
	}

	//Light
	P.RTVcount = 1;
	LightPass.init(P, 1);

	ID3D11Texture2D* irradiancetexture;
	hr = D3DX11CreateTextureFromFile(g_pDevice->m_Device, L"Models/grace_diffuse_cube.dds", &loadinfo, 0, (ID3D11Resource**)&irradiancetexture, 0);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = g_pDevice->m_Device->CreateShaderResourceView(irradiancetexture, &viewDesc, &irradiancesrv);

	if (FAILED(hr))
	{
		return hr;
	}

	LightPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[4]);
	LightPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[1]);
	LightPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[2]);
	LightPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[3]);	
	LightPass.m_ShaderResources.push_back(SkyboxPass.m_PassOutput[0]);
	LightPass.m_ShaderResources.push_back(irradiancesrv);

	//LightPass.m_pRTVs.push_back(SkyboxPass.m_pRTVs[0]);
	//LightPass.m_PassOutput.push_back(SkyboxPass.m_PassOutput[0]);

	CompileShaders(L"Light.fx", "vs_main", "ps_main", LightPass.m_pVS, LightPass.m_pPS);

	LightConsBuff.kDiffuse = 1.f;
	LightConsBuff.kAmbient = .028f;
	LightConsBuff.kSpecular = 2.f;
	LightConsBuff.specularPower = 16.f;
	LightConsBuff.lightDir = { -1, -1, -1, 0 };
	LightConsBuff.lightColor = {.97f, .94f, 1.f, 1.f};

	BufferStruct lightBS;
	lightBS.usage = USAGE_DEFAULT;
	lightBS.byteWidth = sizeof(LightCB);
	lightBS.bindFlags = 4;
	lightBS.cpuAccessFlags = 0;

	LightBuffer.init(lightBS);
	hr = ptrDevice->CreateBuffer(&LightBuffer.m_bd, NULL, &LightBuffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	//Luminance
	P.RTVcount = 1;
	P.TextureStruct.mipLevels = 5;
	P.TextureStruct.miscFlags = 0x1L;
	LuminancePass.init(P, -1);

	LuminancePass.m_ShaderResources.push_back(LightPass.m_PassOutput[0]);

	CompileShaders(L"Luminance.fx", "vs_main", "ps_main", LuminancePass.m_pVS, LuminancePass.m_pPS);

	//Bright
	BrightPass.init(P, -1);

	BrightPass.m_ShaderResources.push_back(LuminancePass.m_PassOutput[0]);
	BrightPass.m_ShaderResources.push_back(LightPass.m_PassOutput[0]);

	CompileShaders(L"Bright.fx", "vs_main", "ps_main", BrightPass.m_pVS, BrightPass.m_pPS);

	BufferStruct brightBS;
	brightBS.usage = USAGE_DEFAULT;
	brightBS.byteWidth = sizeof(BrightCB);
	brightBS.bindFlags = 4;
	brightBS.cpuAccessFlags = 0;

	BrightBuffer.init(brightBS);
	hr = ptrDevice->CreateBuffer(&BrightBuffer.m_bd, NULL, &BrightBuffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	BrightConsBuff.mipLevel = 0;
	BrightConsBuff.Threshold.x = .16f;

	//BlurH1

	BlurH1.init(P, -1);

	BlurH1.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);

	CompileShaders(L"BlurH.fx", "vs_main", "ps_main", BlurH1.m_pVS, BlurH1.m_pPS);

	BufferStruct blurhBS;
	blurhBS.usage = USAGE_DEFAULT;
	blurhBS.byteWidth = sizeof(BlurCB);
	blurhBS.bindFlags = 4;
	blurhBS.cpuAccessFlags = 0;

	BlurH1Buffer.init(blurhBS);
	hr = ptrDevice->CreateBuffer(&BlurH1Buffer.m_bd, NULL, &BlurH1Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	BlurH1ConsBuff.mipLevel = 4;
	BlurH1ConsBuff.Viewport.x = width;

	//BlurV1

	BlurV1.init(P, -1);

	BlurV1.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);

	CompileShaders(L"BlurV.fx", "vs_main", "ps_main", BlurV1.m_pVS, BlurV1.m_pPS);

	BlurV1Buffer.init(blurhBS);
	hr = ptrDevice->CreateBuffer(&BlurV1Buffer.m_bd, NULL, &BlurV1Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	BlurV1ConsBuff.mipLevel = 4;
	BlurV1ConsBuff.Viewport.x = height;

	//AddBright1

	AddBright1.init(P, -1);

	AddBright1.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
	AddBright1.m_ShaderResources.push_back(BlurH1.m_PassOutput[0]);
	AddBright1.m_ShaderResources.push_back(BlurV1.m_PassOutput[0]);

	CompileShaders(L"AddBright.fx", "vs_main", "ps_main", AddBright1.m_pVS, AddBright1.m_pPS);

	BufferStruct addbrightBS;
	addbrightBS.usage = USAGE_DEFAULT;
	addbrightBS.byteWidth = sizeof(AddBrightCB);
	addbrightBS.bindFlags = 4;
	addbrightBS.cpuAccessFlags = 0;

	AddBright1Buffer.init(addbrightBS);
	hr = ptrDevice->CreateBuffer(&AddBright1Buffer.m_bd, NULL, &AddBright1Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	AddBright1ConsBuff.mipLevel.x = 3;

	//BlurH2
	//P.RTVcount = 0;
	BlurH2.init(P, -1);

	BlurH2.m_pVS = BlurH1.m_pVS;
	BlurH2.m_pPS = BlurH1.m_pPS;

	BlurH2.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
	//BlurH2.m_PassOutput.push_back(BlurH1.m_PassOutput[0]);
	//BlurH2.m_pRTVs.push_back(BlurH1.m_pRTVs[0]);

	BlurH2Buffer.init(blurhBS);
	hr = ptrDevice->CreateBuffer(&BlurH2Buffer.m_bd, NULL, &BlurH2Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	BlurH2ConsBuff.mipLevel = 3;
	BlurH2ConsBuff.Viewport.x = width;

	//BlurV2
	P.RTVcount = 0;
	BlurV2.init(P, -1);

	BlurV2.m_pVS = BlurV1.m_pVS;
	BlurV2.m_pPS = BlurV1.m_pPS;

	BlurV2.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
	BlurV2.m_PassOutput.push_back(BlurV1.m_PassOutput[0]);
	BlurV2.m_pRTVs.push_back(BlurV1.m_pRTVs[0]);

	BlurV2Buffer.init(blurhBS);
	hr = ptrDevice->CreateBuffer(&BlurV2Buffer.m_bd, NULL, &BlurV2Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	BlurV2ConsBuff.mipLevel = 3;
	BlurV2ConsBuff.Viewport.x = height;

	//AddBright2

	AddBright2.init(P, -1);

	AddBright2.m_pVS = AddBright1.m_pVS;
	AddBright2.m_pPS = AddBright1.m_pPS;

	AddBright2.m_PassOutput.push_back(AddBright1.m_PassOutput[0]);
	AddBright2.m_pRTVs.push_back(AddBright1.m_pRTVs[0]);

	AddBright2.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
	AddBright2.m_ShaderResources.push_back(BlurH2.m_PassOutput[0]);
	AddBright2.m_ShaderResources.push_back(BlurV2.m_PassOutput[0]);

	AddBright2Buffer.init(addbrightBS);
	hr = ptrDevice->CreateBuffer(&AddBright2Buffer.m_bd, NULL, &AddBright2Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	AddBright2ConsBuff.mipLevel.x = 2;

	//BlurH3

	BlurH3.init(P, -1);

	BlurH3.m_pVS = BlurH2.m_pVS;
	BlurH3.m_pPS = BlurH2.m_pPS;

	BlurH3.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
	BlurH3.m_PassOutput.push_back(BlurH2.m_PassOutput[0]);
	BlurH3.m_pRTVs.push_back(BlurH2.m_pRTVs[0]);

	BlurH3Buffer.init(blurhBS);
	hr = ptrDevice->CreateBuffer(&BlurH3Buffer.m_bd, NULL, &BlurH3Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	BlurH3ConsBuff.mipLevel = 2;
	BlurH3ConsBuff.Viewport.x = width;

	//BlurV3
	BlurV3.init(P, -1);

	BlurV3.m_pVS = BlurV2.m_pVS;
	BlurV3.m_pPS = BlurV2.m_pPS;

	BlurV3.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
	BlurV3.m_PassOutput.push_back(BlurV2.m_PassOutput[0]);
	BlurV3.m_pRTVs.push_back(BlurV2.m_pRTVs[0]);

	BlurV3Buffer.init(blurhBS);
	hr = ptrDevice->CreateBuffer(&BlurV3Buffer.m_bd, NULL, &BlurV3Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	BlurV3ConsBuff.mipLevel = 2;
	BlurV3ConsBuff.Viewport.x = height;

	//AddBright3

	AddBright3.init(P, 5);

	AddBright3.m_pVS = AddBright2.m_pVS;
	AddBright3.m_pPS = AddBright2.m_pPS;

	AddBright3.m_PassOutput.push_back(AddBright2.m_PassOutput[0]);
	AddBright3.m_pRTVs.push_back(AddBright2.m_pRTVs[0]);

	AddBright3.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
	AddBright3.m_ShaderResources.push_back(BlurH3.m_PassOutput[0]);
	AddBright3.m_ShaderResources.push_back(BlurV3.m_PassOutput[0]);

	AddBright3Buffer.init(addbrightBS);
	hr = ptrDevice->CreateBuffer(&AddBright3Buffer.m_bd, NULL, &AddBright3Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	AddBright3ConsBuff.mipLevel.x = 1;

	//BlurH4
	BlurH4.init(P, 5);

	BlurH4.m_pVS = BlurH3.m_pVS;
	BlurH4.m_pPS = BlurH3.m_pPS;

	BlurH4.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
	BlurH4.m_PassOutput.push_back(BlurH3.m_PassOutput[0]);
	BlurH4.m_pRTVs.push_back(BlurH3.m_pRTVs[0]);

	BlurH4Buffer.init(blurhBS);
	hr = ptrDevice->CreateBuffer(&BlurH4Buffer.m_bd, NULL, &BlurH4Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	BlurH4ConsBuff.mipLevel = 1;
	BlurH4ConsBuff.Viewport.x = width;

	//BlurV4
	BlurV4.init(P, -1);

	BlurV4.m_pVS = BlurV2.m_pVS;
	BlurV4.m_pPS = BlurV2.m_pPS;

	BlurV4.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
	BlurV4.m_PassOutput.push_back(BlurV3.m_PassOutput[0]);
	BlurV4.m_pRTVs.push_back(BlurV3.m_pRTVs[0]);

	BlurV4Buffer.init(blurhBS);
	hr = ptrDevice->CreateBuffer(&BlurV4Buffer.m_bd, NULL, &BlurV4Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	BlurV4ConsBuff.mipLevel = 2;
	BlurV4ConsBuff.Viewport.x = height;

	//AddBright4

	AddBright4.init(P, -1);

	AddBright4.m_pVS = AddBright3.m_pVS;
	AddBright4.m_pPS = AddBright3.m_pPS;

	AddBright4.m_PassOutput.push_back(AddBright3.m_PassOutput[0]);
	AddBright4.m_pRTVs.push_back(AddBright3.m_pRTVs[0]);

	AddBright4.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
	AddBright4.m_ShaderResources.push_back(BlurH4.m_PassOutput[0]);
	AddBright4.m_ShaderResources.push_back(BlurV4.m_PassOutput[0]);

	AddBright4Buffer.init(addbrightBS);
	hr = ptrDevice->CreateBuffer(&AddBright4Buffer.m_bd, NULL, &AddBright4Buffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	AddBright4ConsBuff.mipLevel.x = 0;

	//ToneMap
	P.RTVcount = 1;
	P.TextureStruct.miscFlags = 0;	
	P.TextureStruct.mipLevels = 1;

	ToneMapPass.init(P, 1);
	
	CompileShaders(L"ToneMap.fx", "vs_main", "ps_main", ToneMapPass.m_pVS, ToneMapPass.m_pPS);

	ToneMapPass.m_ShaderResources.push_back(LightPass.m_PassOutput[0]);
	ToneMapPass.m_ShaderResources.push_back(SSAOPass.m_PassOutput[0]);
	ToneMapPass.m_ShaderResources.push_back(AddBright4.m_PassOutput[0]);

	BufferStruct tonemapBS;
	tonemapBS.usage = USAGE_DEFAULT;
	tonemapBS.byteWidth = sizeof(ToneMapCB);
	tonemapBS.bindFlags = 4;
	tonemapBS.cpuAccessFlags = 0;

	ToneMapBuffer.init(tonemapBS);

	hr = ptrDevice->CreateBuffer(&ToneMapBuffer.m_bd, NULL, &ToneMapBuffer.m_pBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	ToneMapConsBuff.colorSpace = 2;
	ToneMapConsBuff.kBloom = .3f;
	ToneMapConsBuff.kExposure = 3.2f;

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
#ifdef D3D11
    if( ptrDC) ptrDC->ClearState();
    if( g_SamplerState.m_pSamplerLinear ) g_SamplerState.m_pSamplerLinear->Release();
    if( g_pTextureRV ) g_pTextureRV->Release();
    if( g_Camera.m_CBNeverChanges.m_pBuffer ) g_Camera.m_CBNeverChanges.m_pBuffer->Release();
    if( g_Camera.m_CBChangesOnResize.m_pBuffer ) g_Camera.m_CBChangesOnResize.m_pBuffer->Release();
    if( g_Camera.m_CBChangesEveryFrame.m_pBuffer ) g_Camera.m_CBChangesEveryFrame.m_pBuffer->Release();
    if( g_VertexBuffer.m_Buffer.m_pBuffer) g_VertexBuffer.m_Buffer.m_pBuffer->Release();
	if (g_BoardVB.m_Buffer.m_pBuffer) g_BoardVB.m_Buffer.m_pBuffer->Release();
    if( g_IndexBuffer.m_Buffer.m_pBuffer ) g_IndexBuffer.m_Buffer.m_pBuffer->Release();
	if (g_BoardIB.m_Buffer.m_pBuffer) g_BoardIB.m_Buffer.m_pBuffer->Release();
	if (LightBuffer.m_pBuffer ) LightBuffer.m_pBuffer->Release();
    if( g_VertexShader.m_pInputLayout ) g_VertexShader.m_pInputLayout->Release();
    if( g_VertexShader.m_pVertexShader ) g_VertexShader.m_pVertexShader->Release();
    if( g_PixelShader.m_pPixelShader) g_PixelShader.m_pPixelShader->Release();
    if( g_DepthStencil.m_pTexture) g_DepthStencil.m_pTexture->Release();
    if( DepthStencilViewFree.m_pDepthStencilView ) DepthStencilViewFree.m_pDepthStencilView->Release();
    if( g_RenderTargetView.m_pRTV ) g_RenderTargetView.m_pRTV->Release();
    if( ptrSC) ptrSC->Release();
	if (FirstPerson.m_CBNeverChanges.m_pBuffer) FirstPerson.m_CBNeverChanges.m_pBuffer->Release();
	if (FirstPerson.m_CBChangesOnResize.m_pBuffer) FirstPerson.m_CBChangesOnResize.m_pBuffer->Release();
	if (FirstPerson.m_CBChangesEveryFrame.m_pBuffer) FirstPerson.m_CBChangesEveryFrame.m_pBuffer->Release();
    if( ptrDC) ptrDC->Release();
    if( ptrDevice ) ptrDevice->Release();	
#endif
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) {
		return true;
	}

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
				UINT width = imguiWindowW = rc.right - rc.left;
				UINT height = imguiWindowH = rc.bottom - rc.top;
				//Regenerate world matrix as identity
				g_World = glm::mat4(1.0f);
				//Set w and h for active camera
				ActiveCamera->setHeigth(height);
				ActiveCamera->setWidth(width);
				//Update projection matrix with new params
				ActiveCamera->updatePM();
				//Update CB
				CBChangeOnResize cbChangesOnResize;
				cbChangesOnResize.mProjection = glm::transpose(ActiveCamera->Proj);

				ptrDC->UpdateSubresource(ActiveCamera->m_CBChangesOnResize.m_pBuffer, 0, NULL, &cbChangesOnResize, 0, 0);

				//Set w and h for inactive camera
				InactiveCamera->setHeigth(height);
				InactiveCamera->setWidth(width);
				//Update projection matrix with new params
				InactiveCamera->updatePM();
				cbChangesOnResize.mProjection = glm::transpose(InactiveCamera->Proj);
				//Update CB
				ptrDC->UpdateSubresource(InactiveCamera->m_CBChangesOnResize.m_pBuffer, 0, NULL, &cbChangesOnResize, 0, 0);

				if (ptrSC)
				{
					HRESULT h;

					//Release all passes resources (textures, render target views & shader resource views)

					ToneMapPass.releasePass();
					
					//Because Addbright 2 - 4, BlurH 2 - 4 & BlurV 2 - 4
					//use resources from their first passes, they only need
					//to clear the shader resources

					AddBright4.clearShaderResources();
					AddBright3.clearShaderResources();
					AddBright2.clearShaderResources();

					BlurV4.clearShaderResources();
					BlurV3.clearShaderResources();
					BlurV2.clearShaderResources();

					BlurH4.clearShaderResources();
					BlurH3.clearShaderResources();
					BlurH2.clearShaderResources();

					//Continue releasing resources

					AddBright1.releasePass();
					BlurV1.releasePass();
					BlurH1.releasePass();
					BrightPass.releasePass();
					LuminancePass.releasePass();
					LightPass.releasePass();
					SkyboxPass.releasePass();
					SSAOPass.releasePass();
					DiffusePass.releasePass();

					//Texture
					Texture2DStruct D;
					ZeroMemory(&D, sizeof(D));
					D.W = width;
					D.H = height;
					D.mipLevels = D.arraySize = 1;
					D.format = FORMAT_R16G16B16A16_FLOAT;
					D.sampleDesc.count = 1;
					D.usage = USAGE_DEFAULT;
					D.flags = 8 | 32;		//D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
					D.cpuAccessFlags = 0;	//D3D11_CPU_ACCESS_WRITE;
					D.miscFlags = 0;

					StructPass P;
					P.DeviceContext = g_DeviceContext;
					P.RTVcount = 5;
					P.Device = g_pDevice;
					P.TextureStruct = D;

					//Almost same process as InitDevice with minor changes
					//Texture resolution is adjusted to new window height/width
					//As so, shader resources need to be rebinded

					//GBuffer
					DiffusePass.init(P, 1);

					//SSAO
					P.RTVcount = 1;
					SSAOPass.init(P, 1);
					//Example of resource rebinding
					SSAOPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[0]);
					SSAOPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[2]);

					//Skybox
					SkyboxPass.init(P, 1);

					//Light
					LightPass.init(P, 1);
					LightPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[4]);
					LightPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[1]);
					LightPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[2]);
					LightPass.m_ShaderResources.push_back(DiffusePass.m_PassOutput[3]);
					LightPass.m_ShaderResources.push_back(SkyboxPass.m_PassOutput[0]);
					LightPass.m_ShaderResources.push_back(irradiancesrv);

					//Luminance
					P.TextureStruct.mipLevels = 5;
					P.TextureStruct.miscFlags = 0x1L;
					LuminancePass.init(P, -1);
					LuminancePass.m_ShaderResources.push_back(LightPass.m_PassOutput[0]);

					//Bright
					BrightPass.init(P, -1);

					BrightPass.m_ShaderResources.push_back(LuminancePass.m_PassOutput[0]);
					BrightPass.m_ShaderResources.push_back(LightPass.m_PassOutput[0]);

					//BlurH1
					BlurH1.init(P, -1);

					BlurH1.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					BlurH1ConsBuff.Viewport.x = width;

					//BlurV1
					BlurV1.init(P, -1);

					BlurV1.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					BlurV1ConsBuff.Viewport.x = height;

					//AddBright1
					AddBright1.init(P, -1);

					AddBright1.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					AddBright1.m_ShaderResources.push_back(BlurH1.m_PassOutput[0]);
					AddBright1.m_ShaderResources.push_back(BlurV1.m_PassOutput[0]);

					//Because this pases use info from their originals, its only needed to
					//Rebind resources, rtvs & srvs

					//BlurH2					

					BlurH2.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					BlurH2.m_PassOutput[0] = BlurH1.m_PassOutput[0];
					BlurH2.m_pRTVs[0] = BlurH1.m_pRTVs[0];
					BlurH2ConsBuff.Viewport.x = width;

					//BlurV2

					BlurV2.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					BlurV2.m_PassOutput[0] = BlurV1.m_PassOutput[0];
					BlurV2.m_pRTVs[0] = BlurV1.m_pRTVs[0];
					BlurV2ConsBuff.Viewport.x = height;

					//AddBright2

					AddBright2.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					AddBright2.m_ShaderResources.push_back(BlurH2.m_PassOutput[0]);
					AddBright2.m_ShaderResources.push_back(BlurV2.m_PassOutput[0]);
					AddBright2.m_PassOutput[0] = AddBright1.m_PassOutput[0];
					AddBright2.m_pRTVs[0] = AddBright1.m_pRTVs[0];
					
					//BlurH3

					BlurH3.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					BlurH3.m_PassOutput[0] = BlurH2.m_PassOutput[0];
					BlurH3.m_pRTVs[0] = BlurH2.m_pRTVs[0];
					BlurH3ConsBuff.Viewport.x = width;

					//BlurV3

					BlurV3.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					BlurV3.m_PassOutput[0] = BlurV2.m_PassOutput[0];
					BlurV3.m_pRTVs[0] = BlurV2.m_pRTVs[0];
					BlurV3ConsBuff.Viewport.x = height;

					//AddBright3

					AddBright3.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					AddBright3.m_ShaderResources.push_back(BlurH3.m_PassOutput[0]);
					AddBright3.m_ShaderResources.push_back(BlurV3.m_PassOutput[0]);
					AddBright3.m_PassOutput[0] = AddBright2.m_PassOutput[0];
					AddBright3.m_pRTVs[0] = AddBright2.m_pRTVs[0];

					//BlurH4

					BlurH4.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					BlurH4.m_PassOutput[0] = BlurH3.m_PassOutput[0];
					BlurH4.m_pRTVs[0] = BlurH3.m_pRTVs[0];
					BlurH4ConsBuff.Viewport.x = width;

					//BlurV4

					BlurV4.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					BlurV4.m_PassOutput[0] = BlurV3.m_PassOutput[0];
					BlurV4.m_pRTVs[0] = BlurV3.m_pRTVs[0];
					BlurV4ConsBuff.Viewport.x = height;

					//AddBright4

					AddBright4.m_ShaderResources.push_back(BrightPass.m_PassOutput[0]);
					AddBright4.m_ShaderResources.push_back(BlurH4.m_PassOutput[0]);
					AddBright4.m_ShaderResources.push_back(BlurV4.m_PassOutput[0]);
					AddBright4.m_PassOutput[0] = AddBright3.m_PassOutput[0];
					AddBright4.m_pRTVs[0] = AddBright3.m_pRTVs[0];

					//ToneMap

					P.TextureStruct.miscFlags = 0;
					P.TextureStruct.mipLevels = 1;

					ToneMapPass.init(P, 1);

					ToneMapPass.m_ShaderResources.push_back(LightPass.m_PassOutput[0]);
					ToneMapPass.m_ShaderResources.push_back(SSAOPass.m_PassOutput[0]);
					ToneMapPass.m_ShaderResources.push_back(AddBright4.m_PassOutput[0]);

					//Handle inactive camera first
					//Release inactive camera texture, SRV and RTV
					InactiveCameraTexture.m_pTexture->Release();
					InactiveSRV->Release();
					SecondRTV.m_pRTV->Release();

					//Resize inactive camera texture					
					Texture2DStruct T;
					ZeroMemory(&T, sizeof(T));
					T.W = width;
					T.H = height;
					T.mipLevels = T.arraySize = 1;
					T.format = FORMAT_R8G8B8A8_UNORM;
					T.sampleDesc.count = 1;
					T.usage = USAGE_DEFAULT;
					T.flags = 8 | 32;			// D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
					T.cpuAccessFlags = 65536;	//D3D11_CPU_ACCESS_WRITE;
					T.miscFlags = 0;
					InactiveCameraTexture.init(T);

					h = g_pDevice->m_Device->CreateTexture2D(&InactiveCameraTexture.m_Desc, NULL, &InactiveCameraTexture.m_pTexture);

					RenderTargetViewStruct RTV;
					RTV.format = T.format;
					RTV.viewDimension = RTV_DIMENSION_TEXTURE2D;
					RTV.texture1D.mipSlice = 0;
					SecondRTV.init(RTV);
					h = ptrDevice->CreateRenderTargetView(InactiveCameraTexture.m_pTexture, &SecondRTV.m_Desc, &SecondRTV.m_pRTV);

					D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
					SRV.Format = (DXGI_FORMAT)T.format;
					SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					SRV.Texture2D.MostDetailedMip = 0;
					SRV.Texture2D.MipLevels = 1;

					h = g_pDevice->m_Device->CreateShaderResourceView(InactiveCameraTexture.m_pTexture, &SRV, &InactiveSRV);

					//
					ptrDC->OMSetRenderTargets(0, 0, 0);
					g_RenderTargetView.m_pRTV->Release();

					
					h = ptrSC->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

					CBuffer tempBack;
					h = ptrSC->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tempBack.m_pBuffer);
					h = ptrDevice->CreateRenderTargetView(tempBack.m_pBuffer, NULL, &g_RenderTargetView.m_pRTV);
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

					DepthStencilViewFree.m_pDepthStencilView->Release();

					DepthStencilViewFree.init(DSV, (FORMAT)g_DepthStencil.m_Desc.Format);

					h = g_pDevice->m_Device->CreateDepthStencilView(g_DepthStencil.m_pTexture, &DepthStencilViewFree.m_Desc, &DepthStencilViewFree.m_pDepthStencilView);

					g_DeviceContext->m_DeviceContext->OMSetRenderTargets(1, &g_RenderTargetView.m_pRTV, DepthStencilViewFree.m_pDepthStencilView);

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
				ImGui::GetStyle().ScaleAllSizes(1);
			}
#endif //D3D11
			break;
		}
		case WM_KEYDOWN:
			ActiveCamera->getKeyPress(wParam);
			break;			
		case WM_KEYUP:
			ActiveCamera->getKeyRelease(wParam);
			break;
		case WM_RBUTTONDOWN:
		{
			POINT MousePoint;
			GetCursorPos(&MousePoint);
			ActiveCamera->mInitPos = {MousePoint.x, MousePoint.y, 0.f};
			ActiveCamera->mIsClicked = true;
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (ActiveCamera->mIsClicked)
			{
				POINT EndPoint;
				GetCursorPos(&EndPoint);
				ActiveCamera->mEndPos = {EndPoint.x, EndPoint.y, 0.f};
				SetCursorPos(ActiveCamera->mInitPos.x, ActiveCamera->mInitPos.y);
				ActiveCamera->mDir = ActiveCamera->mInitPos - ActiveCamera->mEndPos;
				ActiveCamera->rotate(ActiveCamera->mDir);
				CBNeverChanges cbNeverChanges;
				cbNeverChanges.mView = ActiveCamera->View;
#ifdef D3D11
				g_DeviceContext->m_DeviceContext->UpdateSubresource(ActiveCamera->m_CBNeverChanges.m_pBuffer, 0, NULL, &cbNeverChanges, 0, 0);
#endif
			}
			break;
		}
		case WM_RBUTTONUP:
			ActiveCamera->mIsClicked = false;
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

	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // red, green, blue, alpha

	//Set GBuffer and configs (constant buffers and resources)

	DiffusePass.setPass(&DepthStencilViewFree);
	DiffusePass.clear(ClearColor, &DepthStencilViewFree);

	if (ActiveCamera->mForward || ActiveCamera->mBack || ActiveCamera->mLeft || ActiveCamera->mRight || ActiveCamera->mUp || ActiveCamera->mDown || ActiveCamera->mRotateLeft || ActiveCamera->mRotateRight)
	{
		ActiveCamera->move();
		ActiveCamera->rotate();
		CBNeverChanges cbNeverChanges;
		cbNeverChanges.mView = ActiveCamera->View;
		g_DeviceContext->m_DeviceContext->UpdateSubresource(ActiveCamera->m_CBNeverChanges.m_pBuffer, 0, NULL, &cbNeverChanges, 0, 0);
	}

	/*//Update bone transforms

	std::vector<glm::mat4> Transforms;

	float RunningTime = GetRunningTime();

	graphicApi.BoneTransform(RunningTime, Transforms, MyScene, &SCManager);

	BoneCB cbBone;

	for (int i = 0; i < Transforms.size(); i++)
	{
		if (i < 100)
		{
			cbBone.gBones[i] = Transforms[i];
		}
	}

	g_DeviceContext->m_DeviceContext->UpdateSubresource(BoneBuffer.m_pBuffer, 0, NULL, &cbBone, 0, 0);*/

	//Set inactive camera RTV and DSV
	CBChangesEveryFrame cbMesh;
	cbMesh.mWorld =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	g_DeviceContext->m_DeviceContext->UpdateSubresource(ActiveCamera->m_CBChangesEveryFrame.m_pBuffer, 0, NULL, &cbMesh, 0, 0);

	g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(0, 1, &ActiveCamera->m_CBNeverChanges.m_pBuffer);
	g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(1, 1, &ActiveCamera->m_CBChangesOnResize.m_pBuffer);
	g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(2, 1, &ActiveCamera->m_CBChangesEveryFrame.m_pBuffer);

	DiffusePass.setRasterizerState(g_GBufferRS);
	DiffusePass.setDepthStencilState(g_GBufferDSS, 0);
	
	g_DeviceContext->m_DeviceContext->PSSetSamplers(0, 1, &g_SamplerState.m_pSamplerLinear);

	DiffusePass.draw(&SCManager, sizeof(SimpleVertex));

	//SSAO

	ClearColor[0] = 0.f;
	ClearColor[1] = 0.f;
	ClearColor[2] = 0.f;
	ClearColor[3] = 0.f;

	SSAOPass.setPass(&DepthStencilViewFree);

	g_DeviceContext->m_DeviceContext->UpdateSubresource(SSAOBuffer.m_pBuffer, 0, NULL, &SSAOConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &SSAOBuffer.m_pBuffer);	

	SSAOPass.setRasterizerState(g_SAQDRS);
	SSAOPass.setDepthStencilState(g_SAQDDSS, 0);

	SSAOPass.draw(&SCAQ, sizeof(SimpleVertex));

	//Skybox

	SkyboxPass.setPass(&DepthStencilViewFree);

	g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(0, 1, &ActiveCamera->m_CBNeverChanges.m_pBuffer);
	g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(1, 1, &ActiveCamera->m_CBChangesOnResize.m_pBuffer);
	g_DeviceContext->m_DeviceContext->VSSetConstantBuffers(2, 1, &ActiveCamera->m_CBChangesEveryFrame.m_pBuffer);

	SkyboxPass.draw(&Cube, sizeof(SimpleVertex));

	//Light

	LightPass.setPass(&DepthStencilViewFree);
	LightConsBuff.vViewPos = { ActiveCamera->getLAt().x, ActiveCamera->getLAt().y, ActiveCamera->getLAt().z, 0 };

	g_DeviceContext->m_DeviceContext->UpdateSubresource(LightBuffer.m_pBuffer, 0, NULL, &LightConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &LightBuffer.m_pBuffer);

	LightPass.draw(&SCAQ, sizeof(SimpleVertex));

	//Luminance

	LuminancePass.setPass(&DepthStencilViewFree);
	LuminancePass.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(LuminancePass.m_PassOutput[0]);

	//Bright

	BrightPass.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(BrightBuffer.m_pBuffer, 0, NULL, &BrightConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &BrightBuffer.m_pBuffer);

	BrightPass.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(BrightPass.m_PassOutput[0]);

	//BlurH1

	BlurH1.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(BlurH1Buffer.m_pBuffer, 0, NULL, &BlurH1ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &BlurH1Buffer.m_pBuffer);

	BlurH1.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(BlurH1.m_PassOutput[0]);

	//BlurV1

	BlurV1.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(BlurV1Buffer.m_pBuffer, 0, NULL, &BlurV1ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &BlurV1Buffer.m_pBuffer);

	BlurV1.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(BlurV1.m_PassOutput[0]);

	//AddBright1

	AddBright1.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(AddBright1Buffer.m_pBuffer, 0, NULL, &AddBright1ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &AddBright1Buffer.m_pBuffer);

	AddBright1.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(AddBright1.m_PassOutput[0]);

	//BlurH2

	BlurH2.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(BlurH2Buffer.m_pBuffer, 0, NULL, &BlurH2ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &BlurH2Buffer.m_pBuffer);

	BlurH2.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(BlurH2.m_PassOutput[0]);

	//BlurV2

	BlurV2.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(BlurV2Buffer.m_pBuffer, 0, NULL, &BlurV2ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &BlurV2Buffer.m_pBuffer);

	BlurV2.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(BlurV2.m_PassOutput[0]);

	//AddBright2

	AddBright2.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(AddBright2Buffer.m_pBuffer, 0, NULL, &AddBright2ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &AddBright2Buffer.m_pBuffer);

	AddBright2.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(AddBright2.m_PassOutput[0]);

	//BlurH3

	BlurH3.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(BlurH3Buffer.m_pBuffer, 0, NULL, &BlurH3ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &BlurH3Buffer.m_pBuffer);

	BlurH3.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(BlurH3.m_PassOutput[0]);

	//BlurV3

	BlurV3.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(BlurV3Buffer.m_pBuffer, 0, NULL, &BlurV3ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &BlurV3Buffer.m_pBuffer);

	BlurV3.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(BlurV3.m_PassOutput[0]);

	//AddBright3

	AddBright3.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(AddBright3Buffer.m_pBuffer, 0, NULL, &AddBright3ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &AddBright3Buffer.m_pBuffer);

	AddBright3.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(AddBright3.m_PassOutput[0]);

	//BlurH4

	BlurH4.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(BlurH4Buffer.m_pBuffer, 0, NULL, &BlurH4ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &BlurH4Buffer.m_pBuffer);

	BlurH4.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(BlurH4.m_PassOutput[0]);

	//BlurV4

	BlurV4.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(BlurV4Buffer.m_pBuffer, 0, NULL, &BlurV4ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &BlurV4Buffer.m_pBuffer);

	BlurV4.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(BlurV4.m_PassOutput[0]);

	//AddBright4

	AddBright4.setPass(&DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(AddBright4Buffer.m_pBuffer, 0, NULL, &AddBright4ConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &AddBright4Buffer.m_pBuffer);

	AddBright4.draw(&SCAQ, sizeof(SimpleVertex));

	g_DeviceContext->m_DeviceContext->GenerateMips(AddBright4.m_PassOutput[0]);

	//ToneMap

	ToneMapPass.setPass(&DepthStencilViewFree);
	ToneMapPass.clear(ClearColor, &DepthStencilViewFree);
	g_DeviceContext->m_DeviceContext->UpdateSubresource(ToneMapBuffer.m_pBuffer, 0, NULL, &ToneMapConsBuff, 0, 0);
	g_DeviceContext->m_DeviceContext->PSSetConstantBuffers(0, 1, &ToneMapBuffer.m_pBuffer);

	ToneMapPass.draw(&SCAQ, sizeof(SimpleVertex));

	//Main window (backbuffer)

	g_DeviceContext->m_DeviceContext->OMSetRenderTargets(1, &g_RenderTargetView.m_pRTV, DepthStencilViewFree.m_pDepthStencilView);
	g_DeviceContext->m_DeviceContext->ClearRenderTargetView(g_RenderTargetView.m_pRTV, ClearColor);
	g_DeviceContext->m_DeviceContext->ClearDepthStencilView(DepthStencilViewFree.m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

	ToneMapPass.draw(&SCAQ, sizeof(SimpleVertex));

    //
    // Present our back buffer to our front buffer
    //
	
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ptrSC->Present( 0, 0 );
#endif
}