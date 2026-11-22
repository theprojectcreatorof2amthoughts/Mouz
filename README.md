# Mouz 🍌

Mouz (موز) is an independent UI programming language. Its name comes from the Arabic word for bananas. It has its own source format, compiler, interpreter, runtime, and UI-handler format; it is not a Shay subproject.

## File types
- `.mozf` — Mouz source programs.
- `.mozhl` — Mouz UI handlers.

## Toolchain
- `mouz` — compiler and source checker.
- `mouzi` — bytecode interpreter.
- Mouz Runtime — native runtime services and platform integrations.

## Platform direction
- Windows: Mouz is the UI programming language. Application UI logic is authored in `.mozf`, not C#.
- Browser: JavaScript remains the browser execution layer. Mouz does not replace JavaScript.
- C, C++, and C# may implement compiler/runtime/platform infrastructure; they are implementation technologies, not the language users write UI applications in.

## First syntax example
```mozf
+mozUihandler |uihandle.mozhl|
+#makeUicomp [<|
    uiStructuredef:= .NET
    uiStat:=1

    doComp |+*WinComp|
```

The first compiler milestone recognizes these directives and emits a small versioned bytecode file. The grammar is being developed from real Mouz examples; unspecified syntax and semantics are not invented yet.

## Build
Requirements: CMake 3.22+, a C11 compiler, and a C++20 compiler.

```sh
cmake -S . -B build
cmake --build build --config Release
./build/mouz check examples/first.mozf
./build/mouz build examples/first.mozf -o first.mozc
./build/mouzi first.mozc
```

## Status
Early foundation / experimental. The initial parser supports handler, component-block, structure-definition, UI-state, and component-dispatch directives. It is not yet a complete UI renderer or production-ready language implementation.
