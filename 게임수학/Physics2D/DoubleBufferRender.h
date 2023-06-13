#ifndef DB_RENDERER_H
#define DB_RENDERER_H

#include <Windows.h>

class DoubleBufferRender
{
private:
	HWND hWnd;
	HDC hDC;
	HDC backDC;

	int screenWidth, screenHeight;
	HBITMAP backBitmap;
	DWORD clearColor;
	LPDWORD lpPixel;

	LONGLONG FPS;
	LONGLONG currTime, lastTime;
	float deltaTime = 0;

private:
	HBITMAP CreateDIBSectionRe(HDC hdc, const int& width, const int& height);
	VOID Clear();
	VOID OnUpdate(float deltaTime);
	VOID OnDraw(HDC backDC);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	BOOL Init(HINSTANCE hInstance, LPCWCH caption = L"WinAPI", UINT width = 800U, UINT height = 600U, UINT FPS = 60U, DWORD clearColor = 0xffffffff);
	VOID Release();
	VOID Run();
};

#endif // !DB_RENDERER_H