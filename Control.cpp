#include "pch.h"
#include "Control.h"

int Control::ID = 0;
bool Control::is_Registered = false;

LRESULT Control::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    Control* pThis = nullptr;

    if (msg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lparam);
        pThis = reinterpret_cast<Control*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else {
        pThis = reinterpret_cast<Control*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    if (pThis) {
        defaultWndProc(pThis, msg, wparam, lparam);
        HRESULT hr = pThis->HandleMessages(msg, wparam, lparam);
        if (hr != 0) {
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
    }
    else {
        int choice = MessageBoxW(hwnd, L"This Window was not initialized properly at runtime\nDo you want to continue?", L"Warning", MB_YESNO | MB_ICONWARNING);
        if (choice == IDNO) {
            PostQuitMessage(0);
        }
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0; // Ensure a return value is always provided
}

Control::Control()
{
    m_x = CW_USEDEFAULT;
    m_y = CW_USEDEFAULT;
    m_height = CW_USEDEFAULT;
    m_width = CW_USEDEFAULT;
    m_isEnabled = true;
    m_ID = ID;
    ID + 1;

    if (is_Registered) {
        wc.lpfnWndProc = WndProc;
        wc.lpszClassName = L"Control";
        wc.hInstance = hInstance();

        wc.style = WS_CHILD | BS_OWNERDRAW;
        wc.cbWndExtra = 0;
        wc.cbClsExtra = 0;
        if (!RegisterClass(&wc)) {
            throw std::runtime_error("Failed to register window class: " + std::to_string(GetLastError()));
        }
    }
}

Control::~Control() {}

HRESULT Control::Create()
{
    HWND hwnd = CreateWindowEx(0, L"Control", nullptr, WS_CHILD | WS_VISIBLE, m_x, m_y, m_width, m_height, m_parent, nullptr, hInstance(), this);

    if (!hwnd) {
        return HRESULT_FROM_WIN32(GetLastError());
    }
    m_hwnd = hwnd;
    return S_OK;
}

HRESULT Control::Destroy() const
{
    if (DestroyWindow(m_hwnd)) {
        return S_OK;
    }
    else {
        return HRESULT_FROM_WIN32(GetLastError());
    }
}

void Control::SetWidth(int newWidth)
{
    if (!MoveWindow(m_hwnd, m_x, m_y, newWidth, m_height, TRUE)) {
        throw std::runtime_error("Failed to move window " + std::to_string(GetLastError()));
    }
    else {
        m_width = newWidth;
    }
}

void Control::SetHeight(int newHeight)
{
    if (!MoveWindow(m_hwnd, m_x, m_y, m_width, newHeight, TRUE)) {
        throw std::runtime_error("Failed to move window " + std::to_string(GetLastError()));
    }
    else {
        m_height = newHeight;
    }
}

void Control::SetPos(int x, int y)
{
    if (!MoveWindow(m_hwnd, x, y, m_width, m_height, TRUE)) {
        throw std::runtime_error("Failed to move window " + std::to_string(GetLastError()));
    }
    else {
        m_x = x;
        m_y = y;
    }
}

void Control::SetPosOnMove(int x, int y)
{
    m_x = x;
    m_y = y;
}

void defaultWndProc(Control* pThis, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_SIZE:
        pThis->SetHeightOnResize(LOWORD(lparam));
        pThis->SetWidthOnResize(HIWORD(lparam));
        break;
    case WM_MOVE:
        pThis->SetPosOnMove(LOWORD(lparam), HIWORD(lparam));
        break;
    }
}
