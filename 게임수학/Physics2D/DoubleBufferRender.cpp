#include "DoubleBufferRender.h"

HBITMAP DoubleBufferRender::CreateDIBSectionRe(HDC hdc, const int& width, const int& height)
{
    BITMAPINFO bm_info;
    ZeroMemory(&bm_info.bmiHeader, sizeof(BITMAPINFOHEADER));
    bm_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bm_info.bmiHeader.biBitCount = 32; // 컬러 수(color bits) : 1, 4, 8, 16, 24, 31
    bm_info.bmiHeader.biWidth = width; // 너비.
    bm_info.bmiHeader.biHeight = height;// 높이.
    bm_info.bmiHeader.biPlanes = 1;

    return CreateDIBSection(hdc, &bm_info, DIB_RGB_COLORS, (void**)&lpPixel, NULL, 0);
}

void DoubleBufferRender::Clear()
{
    for (int y = (screenHeight - 1); 0 < y; y--)
    {
        for (int x = 0; screenWidth > x; x++)
        {
            lpPixel[y * screenWidth + x] = clearColor;
        }
    }
}

BOOL DoubleBufferRender::Init(HINSTANCE hInstance, LPCWCH caption, UINT width, UINT height, UINT FPS, DWORD clearColor)
{
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, DoubleBufferRender::WndProc, 0, 0, hInstance, 0, 0, 0, NULL, caption, NULL };
    RegisterClassExW(&wcex);
    
    UINT fullScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    UINT fullScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
    UINT captionSize = GetSystemMetrics(SM_CYMENU);

    height += captionSize * 2;
    int cx = (fullScreenWidth - width) * 0.5f;
    int cy = (fullScreenHeight - height) * 0.5f;

    if (hWnd = CreateWindow(caption, caption, WS_SYSMENU | WS_MINIMIZEBOX, cx, cy, width, height, nullptr, nullptr, hInstance, this))
    {
        hDC = GetDC(hWnd);
        backDC = CreateCompatibleDC(hDC);

        RECT windowRect;
        GetWindowRect(hWnd, &windowRect);
        screenWidth = windowRect.right - windowRect.left;
        screenHeight = windowRect.bottom - windowRect.top;

        backBitmap = CreateDIBSectionRe(hDC, screenWidth, screenHeight);
        SelectObject(backDC, backBitmap);

        this->clearColor = clearColor;

        this->FPS = 1000 / FPS;
        currTime = lastTime = GetTickCount64();

        ShowWindow(hWnd, SW_SHOWNORMAL);
        UpdateWindow(hWnd);

        return TRUE;
    }

    return FALSE;
}

void DoubleBufferRender::Release()
{
    DeleteObject(backDC);
    DeleteObject(backBitmap);
    ReleaseDC(hWnd, hDC);
}

void DoubleBufferRender::Run()
{
    if (hWnd)
    {
        currTime = GetTickCount64();
        if (lastTime <= currTime)
        {
            OnUpdate((FPS + currTime - lastTime) * 0.001f);

            Clear();
            OnDraw(backDC);
            BitBlt(hDC, 0, 0, screenWidth, screenHeight, backDC, 0, 0, SRCCOPY);

            lastTime = currTime + FPS;
        }
    }
}
