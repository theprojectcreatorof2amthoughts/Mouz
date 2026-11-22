# Mouz native Windows backend

The Windows UI backend is implemented in C++ against the Win32 API. It provides a native window host and primitives for adding text labels and buttons with callbacks.

- `mouz_window_host.hpp` — native host interface.
- `mouz_window_host.cpp` — Win32 window, message loop, child controls, and button dispatch.

This backend is built only on Windows by CMake. It does not generate or require C# application code. Mouz source (`.mozf`) remains the language used to declare UI applications. The next integration step is to have the Mouz interpreter instantiate these native controls from compiled component instructions.
