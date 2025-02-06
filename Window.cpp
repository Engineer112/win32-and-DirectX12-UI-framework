#include "Window.h"

WNDCLASS Window::wc = {};

void Window::AddControl(Control& newControl)
{
    newControl.SetParent(this->m_hwnd);
    HRESULT hr = newControl.Create();
    if (hr != S_OK) {
        throw std::runtime_error("Error creating control: " + hr);
    }
    else {
        Controls.push_back(newControl);
    }
}

void Window::RemoveControl(int ID)
{
    auto it = std::find_if(Controls.begin(), Controls.end(), [ID](const Control& control) {
        return control.GetID() == ID;
        });

    if (it != Controls.end())
    {
        it->Destroy();
        Controls.erase(it);
    }
}

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Window* pThis = nullptr;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lparam;
        pThis = (Window*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->m_hwnd = hwnd;
    }
    else
    {
        pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis)
    {
        switch (msg)
        {
        case WM_SIZE:
            pThis->UpdateSizeOnReSize(LOWORD(lparam), HIWORD(lparam));

            InvalidateRect(hwnd, nullptr, TRUE);

            break;
        case WM_MOVE:
            pThis->UpdatePosOnMove(LOWORD(lparam), HIWORD(lparam));
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT:

        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
        return 0;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::SetWidth(int newWidth)
{
    m_width = newWidth;
    SetWindowPos(m_hwnd, nullptr, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
}

void Window::SetHeight(int newHeight)
{
    m_height = newHeight;
    SetWindowPos(m_hwnd, nullptr, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
}

void Window::SetPos(int x, int y)
{
    m_x = x;
    m_y = y;
    SetWindowPos(m_hwnd, nullptr, m_x, m_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Window::UpdatePosOnMove(int x, int y) noexcept 
{
    m_x = x;
    m_y = y;
}

void Window::UpdateSizeOnReSize(int height, int width)
{
    m_height = height;
    m_width = width;
}

void Window::MakeFullScreen() const 
{
    SetWindowLong(m_hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(m_hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
}

void Window::MakeWindowed() const
{
    SetWindowLong(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    SetWindowPos(m_hwnd, HWND_TOP, m_x, m_y, m_width, m_height, SWP_FRAMECHANGED);
}

void Window::CloseWindow()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
}

HRESULT Window::Create(std::wstring heading, HWND parent = NULL)
{
    m_hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        wc.lpszClassName,               // Window class
        heading.c_str(),                // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        parent,                         // Parent window    
        nullptr,                        // Menu
        GetModuleHandle(nullptr),       // Instance handle
        this                            // Additional application data
    );

    if (m_hwnd == nullptr)
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    ShowWindow(m_hwnd, SW_SHOW);
    return S_OK;
}

void Window::InitializeWndClass(HICON icon, HCURSOR cursor)
{
    wc.lpfnWndProc = Window::WndProc;
    wc.hInstance = GetModuleHandle(nullptr);

    if (icon == NULL) {
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    }
    else 
    {
        wc.hIcon = icon;
    }

    if (cursor == NULL) 
    {
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    }
    else 
    {
        wc.hCursor = cursor;
    }
    wc.lpszClassName = L"WindowClass";
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc))
    {
        throw std::runtime_error("Failed to register window class");
    }
}
