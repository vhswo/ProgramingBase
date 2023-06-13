// PhysicsProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

//#include "framework.h"
//#include "PhysicsProject.h"
#include "DoubleBufferRender.h"
//#include <string>
//#include <time.h>
#include "SimplePhysics2D.h"

Scene scene;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    DoubleBufferRender renderer;
    if (renderer.Init(hInstance, L"Simple Physics2D", 800U, 600U, 120U))
    {
        GameObject* obj = new GameObject(Vector2(400, 550), 600, Shape::Box);
        obj->SetStatic();
        scene.AddObject(obj);

       /* obj = new GameObject(Vector2(315, 80), 20, Shape::Circle);
        scene.AddObject(obj);*/
        obj = new GameObject(Vector2(60, 150), 60, Shape::Box);
        scene.AddObject(obj);  obj->SetStatic();
        //obj = new GameObject(Vector2(520, 90), 60, Shape::Circle);
        //scene.AddObject(obj);
        //obj = new GameObject(Vector2(500, 300), 50, Shape::Circle);
        ////obj->SetStatic();
        //scene.AddObject(obj);
        //obj = new GameObject(Vector2(230, 100), 20, Shape::Box);
        //scene.AddObject(obj);
        obj = new GameObject(Vector2(100, 60), 60, Shape::Box);
      
        scene.AddObject(obj);

        while (WM_QUIT != msg.message)
        {
            if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else renderer.Run();
        }
        
        renderer.Release();
    }
    return (int)msg.wParam;
}

void DoubleBufferRender::OnUpdate(float deltaTime)
{
    scene.Update(deltaTime);
}
void DoubleBufferRender::OnDraw(HDC backDC)
{
    scene.Render(backDC);
}

LRESULT CALLBACK DoubleBufferRender::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
