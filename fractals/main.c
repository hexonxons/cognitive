#include <windows.h>
#include <math.h>

struct Ccomplex
{
	double re;
	double im;
};

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

	// max
	double M = 4;
	// num of iterations
	int iter = 100;
	// constant
	struct Ccomplex C;
	struct Ccomplex Z;
	struct Ccomplex temp;
	int x = 0;
	int y = 0;
	int n = 0;
	int XWindowSz = 1024;
	int YWindowSz = 768;

	switch(message)
	{
		case WM_DESTROY : 
			PostQuitMessage (0); 
			break;
		case WM_LBUTTONDOWN:
			hdc = GetDC(Handle);
			for (x = -XWindowSz / 2; x < XWindowSz / 2; ++x)
			{
				for(y = -YWindowSz / 2; y < YWindowSz / 2; ++y)
				{
					// Julia
					
					Z.re = x * 0.005;
					Z.im = y * 0.005;
					C.re = 0.2733400;
					C.im = 0.0074200; 
					

					// Mandelbrot
					/*
					Z.re = 0;
					Z.im = 0;
					C.re = x * 0.005;
					C.im = y * 0.005;
					*/

					n = 0;

					while (Z.im * Z.im + Z.re * Z.re < M * M && n < iter)
					{
						temp = Z;
						Z.re = temp.re * temp.re - temp.im * temp.im + C.re;
						Z.im = 2 * temp.re * temp.im + C.im;
						++n;
					}
					if (n < iter)
						// color of outside region
						SetPixel(hdc, 512 + x, 384 + y, RGB(n * 16, n * 16, 0));
					else
						// color of set
						SetPixel(hdc, 512 + x, 384 + y, RGB(0, 0, 0));

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
