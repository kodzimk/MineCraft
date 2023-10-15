#include "Window.h"

Window::Window()
{

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;

		SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)window);
		window->setHWND(hwnd);
		window->onCreate();
		break;
	}
	case WM_DESTROY:
	{
		Window* window = (Window*)GetWindowLong(hwnd, GWL_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	

	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);


	}

	return NULL;
}

bool Window::init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = TEXT("MyWindowClass");
	wc.lpszMenuName = TEXT("");
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	
	if (!::RegisterClassEx(&wc))
		return false;


	m_hand = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, TEXT("MyWindowClass")
		, TEXT("DirectX Applictaion"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
		CW_USEDEFAULT ,1024, 760
	,NULL,NULL,NULL,this);

	if (!m_hand)
		return false;

	::ShowWindow(m_hand, SW_SHOW);
	::UpdateWindow(m_hand);



	m_is_run = true;

	return true;
}

bool Window::broadcast()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)>0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	this->onUpdate();

	Sleep(1);


	return true;
}

bool Window::release()
{
	if (!::DestroyWindow(m_hand))
		return false;
		
	return true;
}

bool Window::isRun()
{
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hand, &rc);
	return rc;
}

void Window::setHWND(HWND hwnd)
{
	this->m_hand = hwnd;
}

void Window::onCreate()
{

}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
	m_is_run = false;
}

Window::~Window()
{
}
