#include "Win32App.h"
#include "Engine.h"
#include <memory>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int lpCmdShow)
{
	std::shared_ptr<Win32App> win32App;

	win32App = std::make_shared<Win32App>(L"D3D12 Engine", L"hw3d");

	if (!win32App->Initialize(hInstance, lpCmdShow, 100, 100, 800, 600))
		return -1;

	D3D12App::GetApp()->Initialize(win32App);

	while (!win32App->QuitMessagePosted())
	{
		win32App->DispatchMessages();

		D3D12App::GetApp()->Update();

		D3D12App::GetApp()->Draw();
	}

	return 0;
}