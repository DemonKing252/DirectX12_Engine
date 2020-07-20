#include "Win32App.h"

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
Win32App::Win32App(LPCWSTR windowName, LPCWSTR windowClass)
{
	OutputDebugString(L"Win32 App Created!\n");
	
	ZeroMemory(&m_windowClass, sizeof(WNDCLASS));
	ZeroMemory(&m_msg, sizeof(MSG));

	this->m_windowClassName = windowClass;
	this->m_windowName = windowName;
	this->m_bQuitMessagePosted = false;
}

Win32App::Win32App()
{
}

Win32App::~Win32App()
{
}

bool Win32App::Initialize(HINSTANCE hInstance, int lpCmdShow, INT x, INT y, INT w, INT h)
{
	this->m_windowClass.lpszClassName = m_windowClassName;
	this->m_windowClass.hInstance = hInstance;
	this->m_windowClass.lpfnWndProc = WinProc;

	if (!RegisterClass(&m_windowClass))
		return false;

	m_hwnd = CreateWindow(m_windowClassName, m_windowName,
		WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		x, y, w, h, nullptr, nullptr, hInstance, nullptr);
	
	ShowWindow(m_hwnd, lpCmdShow);	
	return true;
}

void Win32App::DispatchMessages()
{
	while (PeekMessage(&m_msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);

		if (m_msg.message == WM_QUIT) { m_bQuitMessagePosted = true; }
	}
}

HWND Win32App::Get() const
{
	return m_hwnd;
}

bool Win32App::QuitMessagePosted() const
{
	return m_bQuitMessagePosted;
}
