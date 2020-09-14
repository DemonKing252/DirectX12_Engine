#include "Win32App.h"
#include "Engine.h"

/*********************************************************************************
DirectX12_Engine::D3DMain.cpp by Liam Blake (C) 2020 All Rights Reserved.

	Created:			 2020-07-20
	Modified:			 2020-08-03
	Total Lines of code: 2532
	
	Last Change:
		Added shadows to each object. They get drawn into the stencil buffer just like the reflections do.

*********************************************************************************/

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int lpCmdShow)
{
	std::shared_ptr<Win32App> win32App;
	win32App = std::make_shared<Win32App>(L"D3D12 Engine", L"hw3d");

	if (!win32App->Initialize(hInstance, lpCmdShow, 100, 100, 800, 600))
	{
		MessageBox(0, L"HWND Register Failed!", L"HWND Error!", 0);
	}
	else
	{
		D3D12App::GetApp()->Initialize(win32App, L"DefaultVS.hlsl", L"DefaultPS.hlsl", L"ShadowPS.hlsl");
		while (!win32App->QuitMessagePosted())
		{
			// Process window events
			win32App->DispatchMessages();

			// Reset the command objects for a new frame
			D3D12App::GetApp()->ResetCommandObjects();

			// Update
			D3D12App::GetApp()->Update();

			// Draw
			D3D12App::GetApp()->Draw();

			// Swap the front and back buffers
			D3D12App::GetApp()->SwapBuffers();

			// Sync the CPU/GPU to prevent screen tearing/flickering
			D3D12App::GetApp()->SyncPreviousFrame();
		}

		win32App->Clean();
		D3D12App::GetApp()->Clean();
	}
	return 0;
}