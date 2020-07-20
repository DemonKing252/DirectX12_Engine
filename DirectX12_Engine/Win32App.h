#pragma once
#include <Windows.h>

class Win32App
{
private:
	WNDCLASS m_windowClass;
	HWND m_hwnd;
	MSG m_msg;

	LPCWSTR m_windowName;
	LPCWSTR m_windowClassName;
	
	bool m_bQuitMessagePosted;
public:
	Win32App(LPCWSTR windowName, LPCWSTR windowClass);
	Win32App();
	~Win32App();

	bool Initialize(HINSTANCE hInstance, int lpCmdShow, INT x, INT y, INT w, INT h);
	void DispatchMessages();

	HWND Get() const;

	bool QuitMessagePosted() const;
};

