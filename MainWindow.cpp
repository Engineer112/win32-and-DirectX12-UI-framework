#include "MainWindow.h"

void MainWindow::AddControl(Control& newControl)
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

void MainWindow::RemoveControl(int ID)
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


LRESULT MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    MainWindow* pThis = nullptr;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lparam;
        pThis = (MainWindow*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->m_hwnd = hwnd;
    }
    else
    {
        pThis = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis)
    {
        switch (msg)
        {
        case WM_SIZE:
            pThis->SetWidth(LOWORD(lparam));
            pThis->SetHeight(HIWORD(lparam));

            InvalidateRect(hwnd, nullptr, TRUE);

            break;
        case WM_MOVE:
            pThis->SetPosOnMove(LOWORD(lparam), HIWORD(lparam));
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT:

            break;
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
        return 0;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void MainWindow::SetWidth(int newWidth)
{
    m_width = newWidth;
    SetWindowPos(m_hwnd, nullptr, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
}

void MainWindow::SetHeight(int newHeight)
{
    m_height = newHeight;
    SetWindowPos(m_hwnd, nullptr, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOZORDER);
}

void MainWindow::SetPos(int x, int y)
{
    m_x = x;
    m_y = y;
    SetWindowPos(m_hwnd, nullptr, m_x, m_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void MainWindow::SetPosOnMove(int x, int y)
{
    m_x = x;
    m_y = y;
}

void MainWindow::MakeFullScreen() const 
{
    SetWindowLong(m_hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(m_hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
}

void MainWindow::MakeWindowed() const
{
    SetWindowLong(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    SetWindowPos(m_hwnd, HWND_TOP, m_x, m_y, m_width, m_height, SWP_FRAMECHANGED);
}
