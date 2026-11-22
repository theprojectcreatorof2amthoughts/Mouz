# Mouz Windows backend

This project is the initial Windows host infrastructure for Mouz. Mouz source files (`.mozf`) remain the application/UI language; this C# assembly is an implementation detail of the Windows backend, not generated application source.

The first host exposes a minimal window, text label, and button API. The next integration milestone is connecting compiler/interpreter component and event instructions to this host. The `.NET` structure selection in the initial language example is preserved as user-provided syntax; its final semantics will be defined in the language specification.
