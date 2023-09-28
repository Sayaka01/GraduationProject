#pragma once

#include <d3d11.h>

#include <windows.h>
#include <sstream>
#include <wrl.h>

#include "HighResolutionTimer.h"

#ifdef USE_IMGUI
#include "../../../imgui/imgui.h"
#include "../../../imgui/imgui_internal.h"
#include "../../../imgui/imgui_impl_dx11.h"
#include "../../../imgui/imgui_impl_win32.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif


CONST BOOL FULLSCREEN{ FALSE };
CONST LPCWSTR APPLICATION_NAME{ L"GraduateProject" };

class Framework
{
public:
	CONST HWND hwnd;

	Framework(HWND hwnd);
	~Framework();

	Framework(const Framework&) = delete;
	Framework& operator=(const Framework&) = delete;
	Framework(Framework&&) noexcept = delete;
	Framework& operator=(Framework&&) noexcept = delete;

	int Run()
	{
		MSG msg{};

		if (!Initialize())
		{
			return 0;
		}

#ifdef USE_IMGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 14.0f, nullptr, glyphRangesJapanese);
		// imgui ドッキングのフラグ    // Enable docking(available in imgui `docking` branch at the moment)
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		// メインウィンドウの外に出す  // Enable Multi-Viewport / Platform Windows
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		io.ConfigViewportsNoDecoration = true; // imguiウィンドウをメインウィンドウの外に出したとき全画面、最小化ができる: false
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(device.Get(), dc.Get());
		ImGui::StyleColorsDark();
#endif

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				tictoc.Tick();
				CalculateFrameStats();
				Update(tictoc.time_interval());
				Draw(tictoc.time_interval());
			}
		}

#ifdef USE_IMGUI
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
#endif

#if 1
		BOOL fullscreen{};
		swapChain->GetFullscreenState(&fullscreen, 0);
		if (fullscreen)
		{
			swapChain->SetFullscreenState(FALSE, 0);
		}
#endif

		return Uninitialize() ? static_cast<int>(msg.wParam) : 0;
	}

	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
#ifdef USE_IMGUI
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }
#endif
		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps{};
			BeginPaint(hwnd, &ps);

			EndPaint(hwnd, &ps);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			if (wparam == VK_ESCAPE)
			{
				PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
			break;
		case WM_ENTERSIZEMOVE:
			tictoc.Stop();
			break;
		case WM_EXITSIZEMOVE:
			tictoc.Start();
			break;
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

private:
	bool Initialize();
	void Update(float elapsedTime/*Elapsed seconds from last frame*/);
	void Draw(float elapsedTime/*Elapsed seconds from last frame*/);
	bool Uninitialize();

private:
	//COMオブジェクトのポインタ変数を定義する
	 Microsoft::WRL::ComPtr<ID3D11Device> device;
	 Microsoft::WRL::ComPtr<ID3D11DeviceContext> dc;
	 Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	 Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	 Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

	HighResolutionTimer tictoc;
	uint32_t frames{ 0 };
	float elapsedTime{ 0.0f };
	void CalculateFrameStats()
	{
		if (++frames, (tictoc.TimeStamp() - elapsedTime) >= 1.0f)
		{
			float fps = static_cast<float>(frames);
			std::wostringstream outs;
			outs.precision(6);
			
			outs << APPLICATION_NAME/* << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)"*/;

#ifdef _DEBUG
			outs << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";

#endif
			SetWindowTextW(hwnd, outs.str().c_str());

			frames = 0;
			elapsedTime += 1.0f;
		}
	}

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[4];

private:
	
};

