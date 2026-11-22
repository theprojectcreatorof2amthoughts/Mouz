#pragma once
#ifdef _WIN32
#include <windows.h>
#include <functional>
#include <string>
#include <unordered_map>

namespace mouz::windows {
class WindowHost {
public:
    WindowHost(std::wstring title, int width = 800, int height = 600);
    ~WindowHost();
    WindowHost(const WindowHost&) = delete;
    WindowHost& operator=(const WindowHost&) = delete;

    bool valid() const noexcept;
    HWND handle() const noexcept;
    HWND add_text(const std::wstring& text, int x, int y, int width = 320, int height = 28);
    HWND add_button(const std::wstring& text, int x, int y, int width = 120, int height = 32,
                    std::function<void()> on_press = {});
    int run();

private:
    static LRESULT CALLBACK window_proc(HWND, UINT, WPARAM, LPARAM);
    LRESULT dispatch(UINT, WPARAM, LPARAM);
    HWND window_ = nullptr;
    std::unordered_map<int, std::function<void()>> callbacks_;
    int next_control_id_ = 1000;
};
}
#endif
