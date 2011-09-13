#include <windows.h>
#include <math.h>
#include "ccomplex.h"
#include "fractal.h"

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
/*--------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
{
	HWND Handle;
	MSG Messages;
	WNDCLASSEX WindowClass;
	const char *szClassName = "Win-App";

	WindowClass.hInstance = hThisInstance;
	WindowClass.lpszClassName = szClassName;
	WindowClass.lpfnWndProc = WindowProcedure;
	WindowClass.style = CS_DBLCLKS;
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	WindowClass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
	WindowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
	WindowClass.lpszMenuName = NULL;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	if(!RegisterClassEx (&WindowClass))
		return 0;
	Handle = CreateWindow(szClassName, "WINDOW", WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
							NULL, NULL, hThisInstance, NULL);
	ShowWindow(Handle, nFunsterStil);
	while (GetMessage(&Messages, NULL, 0, 0))
	{
		TranslateMessage(&Messages);
		DispatchMessage(&Messages);
	}
	return Messages.wParam;
}
/*------------------------------------------------------------------------------*/
LRESULT CALLBACK WindowProcedure(HWND Handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	int x = 0;
	int y = 0;
	int XWindowSz = 1024;
	int YWindowSz = 768;

	switch(message)
	{
		case WM_DESTROY : 
			PostQuitMessage (0); 
			break;
		case WM_LBUTTONDOWN :

			hdc = GetDC(Handle);
			for (x = -XWindowSz / 2; x < XWindowSz / 2; ++x)
			{
				for(y = -YWindowSz / 2; y < YWindowSz / 2; ++y)
				{
					fractal1(x, y, hdc);
				}
			}
			ReleaseDC(Handle, hdc);
			break;
		default :
			return DefWindowProc (Handle, message, wParam, lParam);
	}
	return 0;
}
/*------------------------------------------------------------------------------*/
