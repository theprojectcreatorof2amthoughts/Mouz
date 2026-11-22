#ifdef _WIN32
#include "mouz_window_host.hpp"

#include <stdexcept>
#include <utility>

namespace mouz::windows {
namespace {
constexpr wchar_t window_class_name[] = L"Mouz.NativeWindow";
}

WindowHost::WindowHost(std::wstring title, int width, int height) {
    const HINSTANCE instance = GetModuleHandleW(nullptr);
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = &WindowHost::window_proc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = window_class_name;
    RegisterClassExW(&wc);

    window_ = CreateWindowExW(0, window_class_name, title.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, instance, this);
}

WindowHost::~WindowHost() {
    if (window_ && IsWindow(window_)) DestroyWindow(window_);
}

bool WindowHost::valid() const noexcept { return window_ != nullptr; }
HWND WindowHost::handle() const noexcept { return window_; }

HWND WindowHost::add_text(const std::wstring& text, int x, int y, int width, int height) {
    if (!window_) return nullptr;
    return CreateWindowExW(0, L"STATIC", text.c_str(), WS_CHILD | WS_VISIBLE,
        x, y, width, height, window_, nullptr, GetModuleHandleW(nullptr), nullptr);
}

HWND WindowHost::add_button(const std::wstring& text, int x, int y, int width, int height,
                            std::function<void()> on_press) {
    if (!window_) return nullptr;
    const int id = next_control_id_++;
    HWND control = CreateWindowExW(0, L"BUTTON", text.c_str(),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, width, height,
        window_, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
        GetModuleHandleW(nullptr), nullptr);
    if (control && on_press) callbacks_.emplace(id, std::move(on_press));
    return control;
}

int WindowHost::run() {
    if (!window_) return 1;
    ShowWindow(window_, SW_SHOW);
    UpdateWindow(window_);
    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return static_cast<int>(message.wParam);
}

LRESULT CALLBACK WindowHost::window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    WindowHost* self = reinterpret_cast<WindowHost*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (message == WM_NCCREATE) {
        const auto* create = reinterpret_cast<const CREATESTRUCTW*>(lparam);
        self = static_cast<WindowHost*>(create->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        if (self) self->window_ = hwnd;
    }
    if (self) return self->dispatch(message, wparam, lparam);
    return DefWindowProcW(hwnd, message, wparam, lparam);
}

LRESULT WindowHost::dispatch(UINT message, WPARAM wparam, LPARAM lparam) {
    if (message == WM_COMMAND && HIWORD(wparam) == BN_CLICKED) {
        const auto found = callbacks_.find(LOWORD(wparam));
        if (found != callbacks_.end() && found->second) found->second();
        return 0;
    }
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(window_, message, wparam, lparam);
}
}
#endif
