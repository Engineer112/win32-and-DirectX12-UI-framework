#ifndef CONTROL_H
#define CONTROL_H

#include <windows.h>

class Control
{
public:
    Control();
    virtual ~Control();
    virtual HRESULT HandleMessages(UINT msg, WPARAM wparam, LPARAM lParam) = 0;
    HRESULT Create();
    void SetParent(HWND parent) noexcept { m_parent = parent; }
    int GetID() const noexcept { return m_ID; }
    HRESULT Destroy() const;

protected:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    // dimensions
    int m_height{ 0 }, m_width{ 0 };

    // coordinates
    int m_x{ 0 }, m_y{ 0 };

    HWND m_parent{ nullptr };
    HWND m_hwnd{ nullptr };
    int m_ID{ 0 };
    bool m_isEnabled{ true };
    static int ID;
    static bool is_Registered;

    int GetWidth() const noexcept { return m_width; }
    int GetHeight() const noexcept { return m_height; }
    void SetWidth(int newWidth) noexcept { m_width = newWidth; }
    void SetHeight(int newHeight) noexcept { m_height = newHeight; }
    void SetHeightOnResize(int newHeight) noexcept { m_height = newHeight; }
    void SetWidthOnResize(int newWidth) noexcept { m_width = newWidth; }

    int GetXPos() const noexcept { return m_x; }
    int GetYPos() const noexcept { return m_y; }
    void SetPos(int x, int y) noexcept { m_x = x; m_y = y; }
    void SetPosOnMove(int x, int y) noexcept { m_x = x; m_y = y; }

    static WNDCLASS wc;

private:
    friend void defaultWndProc(Control* pThis, UINT msg, WPARAM wparam, LPARAM lparam);
};

#endif // CONTROL_H
