#include <windows.h>
#include <gl/gl.h>
#include<math.h>

#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076 

#pragma comment(lib, "opengl32.lib")

namespace camera
{
	struct SCamera
	{
		float x, y, z;
		float XRot, ZRot;
	}camera = { 0,0,1.7,0,0 };

	void Camera_Apply()
	{
		glRotatef(-camera.XRot, 1, 0, 0);
		glRotatef(-camera.ZRot, 0, 0, 1);
		glTranslatef(-camera.x, -camera.y, -camera.z);
	}

	void Camera_Rotation(float zAngle, float xAngle)
	{
		camera.ZRot += zAngle;
		if (camera.ZRot < 0)camera.ZRot += 360;
		if (camera.ZRot > 360)camera.ZRot -= 360;
		camera.XRot += xAngle;
		if (camera.XRot < 0)camera.XRot = 0;
		if (camera.XRot > 180)camera.XRot = 180;


	}

	void Camera_AutoMoveByMouse(int centerX, int centerY, float speed)
	{
		POINT cur;
		POINT base = { centerX,centerY };
		GetCursorPos(&cur);
		Camera_Rotation((base.y - cur.y) / 5.0, (base.x - cur.x) / 5.0);
		SetCursorPos(base.x, base.y);
	}

	void Camera_MoveDirection(int forwardMove, int rightMove, float speed)
	{
		float ugol = -camera.ZRot / 180 * 3.14;

		if(forwardMove>0)
			ugol += rightMove > 0 ? -M_PI_4 : (rightMove < 0 ? -M_PI_4 : 0);
			if(forwardMove<0)		
				ugol += 3.14+(rightMove > 0 ? -M_PI_4 : (rightMove < 0 ? M_PI_4 : 0));
			if (forwardMove == 0)
			{
				ugol += rightMove > 0 ? M_PI_2 : -M_PI_2;
				if (rightMove == 0)speed = 0;
			}

			if (speed != 0)
			{
				camera.x += sin(ugol) * speed;
				camera.y += cos(ugol) * speed;
			}


	}
}

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


void Player_Move()
{
	camera::Camera_MoveDirection(GetKeyState('W') < 0 ? 1 : (GetKeyState('S') < 0 ? -1 : 0),
		GetKeyState('D')<0?1:(GetKeyState('A')<0?-1:0),0.1);
	camera::Camera_AutoMoveByMouse(400, 400, 0.2);
}

void WndResize(int x,int y)
{
	glViewport(0, 0, x, y);
	float k = x / (float)y;
	float sz = 0.1;
	glLoadIdentity();
	glFrustum(-k * sz, k * sz, -sz, sz, sz * 2, 100);
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hwnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL bQuit = FALSE;
	float theta = 0.0f;

	/* register window class */
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("GLSample");
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


	if (!RegisterClassEx(&wcex))
		return 0;

	/* create main window */
	hwnd = CreateWindowEx(0,
		TEXT("GLSample"),
		TEXT("OpenGL Sample"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		600,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);

	/* enable OpenGL for the window */
	EnableOpenGL(hwnd, &hDC, &hRC);

	RECT rct;
	GetClientRect(hwnd,&rct);
	WndResize(rct.right, rct.bottom);

	/* program main loop */
	while (!bQuit)
	{
		/* check for messages */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT)
			{
				bQuit = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			/* OpenGL animation code goes here */

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glPushMatrix();
			if (GetForegroundWindow() == hwnd)
				Player_Move();

			camera::Camera_Apply();

			glBegin(GL_TRIANGLES);

			glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f, 1.0f);
			glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f, -0.5f);
			glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);

			glEnd();
		
			glPopMatrix();

			SwapBuffers(hDC);

			theta += 1.0f;
			Sleep(1);
		}
	}

	/* shutdown OpenGL */
	DisableOpenGL(hwnd, hDC, hRC);

	/* destroy the window explicitly */
	DestroyWindow(hwnd);

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		return 0;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	}
	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
	PIXELFORMATDESCRIPTOR pfd;

	int iFormat;

	/* get the device context (DC) */
	*hDC = GetDC(hwnd);

	/* set the pixel format for the DC */
	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(*hDC, &pfd);

	SetPixelFormat(*hDC, iFormat, &pfd);

	/* create and enable the render context (RC) */
	*hRC = wglCreateContext(*hDC);

	wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hwnd, hDC);
}
