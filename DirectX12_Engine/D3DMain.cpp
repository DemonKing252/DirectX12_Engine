#include "Win32App.h"
#include "Engine.h"


/*********************************************************************************
DirectX12_Engine::D3DMain.cpp by Liam Blake (C) 2020 All Rights Reserved.

	Created:			 2020-07-20
	Modified:			 2020-09-23
	Total Lines of code: 2532
	
	Last Change:
		Added ImGui to modify the enviorment.

*********************************************************************************/



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int lpCmdShow)
{
	GameTimer gt;
	gt.Reset();

	std::shared_ptr<Win32App> win32App;
	win32App = std::make_shared<Win32App>(L"D3D12 Engine", L"hw3d");

	if (!win32App->Initialize(hInstance, lpCmdShow, 100, 100, 1024, 768))
	{
		MessageBox(0, L"HWND Register Failed!", L"HWND Error!", 0);
	}
	else
	{
		D3D12App::GetApp()->Initialize(win32App, L"DefaultVS.hlsl", L"DefaultPS.hlsl", L"ShadowPS.hlsl");
		while (!win32App->QuitMessagePosted())
		{
			gt.frameStart = clock();

			// Process window events
			win32App->DispatchMessages(gt);

			// Reset the command objects for a new frame
			D3D12App::GetApp()->ResetCommandObjects();

			// Update
			D3D12App::GetApp()->Update(gt);

			// Draw
			D3D12App::GetApp()->Draw();

			// Swap the front and back buffers
			D3D12App::GetApp()->SwapBuffers();

			// Sync the CPU/GPU to prevent screen tearing/flickering
			D3D12App::GetApp()->SyncPreviousFrame();

			gt.frameEnd = clock();
			gt.SetDeltaTime((gt.frameEnd - gt.frameStart) / CLOCKS_PER_SEC);

			if (gt.Running())
				gt.SetTime(gt.GetTime() + gt.GetDeltaTime());


		}

		win32App->Clean();
		D3D12App::GetApp()->Clean();
	}
	return 0;
}