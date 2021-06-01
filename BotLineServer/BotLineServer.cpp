// BotLineServer.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "BotLineServer.h"
#include "BotLine.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
    std::unique_ptr<BotLine>    gBotLine;
}

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    gBotLine = std::make_unique<BotLine>();

    // DirectXMath 라이브러리 지원 여부 확인
    if (!DirectX::XMVerifyCPUSupport())
    {
        return -1;
    }
    
    // COM 라이브러리 초기화
    HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
    if (FAILED(hr))
    {
        return -1;
    }

    // 애플리케이션 초기화를 수행합니다:
    {
        WNDCLASSEX wcex     = { sizeof(WNDCLASSEX) };
        wcex.style          = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc    = WndProc;
        wcex.cbClsExtra     = 0;
        wcex.cbWndExtra     = sizeof(LONG_PTR);
        wcex.hInstance      = HINST_THISCOMPONENT;
        wcex.hCursor        = LoadCursor(0, IDC_ARROW);
        wcex.hbrBackground  = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcex.lpszMenuName   = nullptr;
        wcex.lpszClassName  = L"BotLineServer";

        if (!RegisterClassExW(&wcex))
        {
            return -1;
        }

        int w, h;
        gBotLine->GetDefaultSize(w, h);

        RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        HWND hwnd = CreateWindowExW(
            0,
            wcex.lpszClassName,
            L"BotLineServer",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rc.right  - rc.left,
            rc.bottom - rc.top,
            nullptr,
            nullptr,
            HINST_THISCOMPONENT,
            nullptr);

        hr = hwnd ? S_OK : E_FAIL;

        if (SUCCEEDED(hr))
        {
            ShowWindow(hwnd, nCmdShow);
            UpdateWindow(hwnd);

            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(gBotLine.get()));

            GetClientRect(hwnd, &rc);

            gBotLine->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);
        }
    }

    MSG msg = {};
    // 기본 메시지 루프입니다:
    while (WM_QUIT != msg.message)
    {
        // GetMessage와 PeekMessage의 차이
        // GetMessage - 메시지가 있을 때까지 대기
        // PeekMessage - 메시지가 올때까지 기다리지 않음
        // https://stackoverflow.com/questions/2850186/why-peekmessage-before-getmessage
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Loop
            gBotLine->Tick();
        }
    }

    gBotLine.reset();

    CoUninitialize();

    return static_cast<int>(msg.wParam);
}


//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    static bool s_in_sizemove   = false;
    static bool s_in_suspend    = false;
    static bool s_minimized     = false;
    static bool s_fullscreen    = false;

    auto botLine = reinterpret_cast<BotLine*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    switch (message)
    {
    case WM_PAINT:
        if (s_in_sizemove && botLine)
        {
            botLine->Tick();
        }
        else
        {
            PAINTSTRUCT ps;
            (void)BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_MOVE:
        if (botLine)
        {
            botLine->OnWindowMoved();
        }
        break;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            if (!s_minimized)
            {
                s_minimized = true;
                if (!s_in_suspend && botLine)
                {
                    botLine->OnSuspending();
                }
                s_in_suspend = true;
            }
        }
        else if (s_minimized)
        {
            s_minimized = false;
            if (s_in_suspend && botLine)
            {
                botLine->OnResuming();
            }
            s_in_suspend = false;
        }
        else if (!s_in_sizemove && botLine)
        {
            botLine->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
        }
        break;
    case WM_ENTERSIZEMOVE:
        s_in_sizemove = true;
        break;
    case WM_EXITSIZEMOVE:
        s_in_sizemove = false;
        if (botLine)
        {
            RECT rc;
            GetClientRect(hWnd, &rc);

            botLine->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
        }
        break;
    case WM_GETMINMAXINFO:
        if (lParam)
        {
            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 320;
            info->ptMinTrackSize.y = 200;
        }
        break;
    case WM_ACTIVATEAPP:
        if (botLine)
        {
            if (wParam)
            {
                botLine->OnActivated();
            }
            else
            {
                botLine->OnDeactivated();
            }
        }
        break;
    case WM_POWERBROADCAST:
        switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
            if (!s_in_suspend && botLine)
                botLine->OnSuspending();
            s_in_suspend = true;
            return TRUE;

        case PBT_APMRESUMESUSPEND:
            if (!s_minimized)
            {
                if (s_in_suspend && botLine)
                    botLine->OnResuming();
                s_in_suspend = false;
            }
            return TRUE;
        }
        break;
    case WM_SYSKEYDOWN:
        if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
        {
            // Implements the classic ALT+ENTER fullscreen toggle
            if (s_fullscreen)
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

                int width = 800;
                int height = 600;
                if (botLine)
                    botLine->GetDefaultSize(width, height);

                ShowWindow(hWnd, SW_SHOWNORMAL);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }
            else
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, 0);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

                ShowWindow(hWnd, SW_SHOWMAXIMIZED);
            }

            s_fullscreen = !s_fullscreen;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MENUCHAR:
        return MAKELRESULT(0, MNC_CLOSE);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}