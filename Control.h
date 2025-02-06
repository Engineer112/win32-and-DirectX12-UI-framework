#ifndef CONTROL_H
#define CONTROL_H

#include <windows.h>
#include <stdexcept>

class Control
{
public:
    Control();
    ~Control();
    virtual HRESULT HandleMessages(UINT msg, WPARAM wparam, LPARAM lParam) = 0;
    HRESULT Create();
    void SetParent(HWND parent) noexcept { m_parent = parent; }
    int GetID() const noexcept { return m_ID; }
    HRESULT Destroy() const noexcept;

    int GetWidth() const noexcept { return m_width; }
    int GetHeight() const noexcept { return m_height; }
    void SetWidth(int newWidth);
    void SetHeight(int newHeight);

    int GetXPos() const noexcept { return m_x; }
    int GetYPos() const noexcept { return m_y; }
    void SetPos(int x, int y);

protected:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    // dimensions
    int m_height{ CW_USEDEFAULT }, m_width{ CW_USEDEFAULT };

    // coordinates
    int m_x{ CW_USEDEFAULT }, m_y{ CW_USEDEFAULT };

    HWND m_parent{ nullptr };
    HWND m_hwnd{ nullptr };
    int m_ID{ 0 };
    bool m_isEnabled{ true };
    static int ID;
    static bool is_Registered;

    void updateDimensionsOnResize(int height, int width);

    void UpdatePosOnMove(int x, int y) noexcept;

    static WNDCLASS wc;

private:
    friend void defaultWndProc(Control* pThis, UINT msg, WPARAM wparam, LPARAM lparam);
};

#endif // CONTROL_H
