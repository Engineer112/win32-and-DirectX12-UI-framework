#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pch.h"
#include "Control.h"
#include <vector>
#include <stdexcept>

class Window
{
public:
    void AddControl(Control& newControl);
    void RemoveControl(int ID);

    int GetWidth() const noexcept { return m_width; }
    int GetHeight() const noexcept { return m_height; }
    void SetWidth(int newWidth);
    void SetHeight(int newHeight);

    int GetXPos() const noexcept { return m_x; }
    int GetYPos() const noexcept { return m_y; }
    void SetPos(int x, int y);
    void MakeFullScreen() const;
    void MakeWindowed() const;
    void CloseWindow();
    HRESULT Create(std::wstring heading, HWND parent);
    static void InitializeWndClass(HICON icon, HCURSOR cursor);

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    static WNDCLASS wc;

    // dimensions
    int m_height{ 0 }, m_width{ 0 };

    // coordinates
    int m_x{ 0 }, m_y{ 0 };

    HWND m_hwnd{ nullptr };
    bool m_isEnabled{ true };

    std::vector<Control> Controls;
    void UpdatePosOnMove(int x, int y) noexcept;
    void UpdateSizeOnReSize(int height, int width);
};

#endif // MAINWINDOW_H
