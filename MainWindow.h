#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pch.h"
#include "Control.h"
#include <vector>
#include <stdexcept>

class MainWindow
{
public:
    void AddControl(Control& newControl);
    void RemoveControl(int ID);

    int GetWidth() const noexcept { return m_width; }
    int GetHeight() const noexcept { return m_height; }
    void SetWidth(int newWidth);
    void SetHeight(int newHeight);
    void SetHeightOnResize(int newHeight) noexcept { m_height = newHeight; }
    void SetWidthOnResize(int newWidth) noexcept { m_width = newWidth; }

    int GetXPos() const noexcept { return m_x; }
    int GetYPos() const noexcept { return m_y; }
    void SetPos(int x, int y);
    void SetPosOnMove(int x, int y) noexcept;
    void MakeFullScreen() const;
    void MakeWindowed() const;

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    // dimensions
    int m_height{ 0 }, m_width{ 0 };

    // coordinates
    int m_x{ 0 }, m_y{ 0 };

    HWND m_hwnd{ nullptr };
    bool m_isEnabled{ true };

    std::vector<Control> Controls;
};

#endif // MAINWINDOW_H
