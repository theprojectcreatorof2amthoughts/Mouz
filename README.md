# Mouz 

Mouz (موز) is an independent UI programming language. Its name comes from the Arabic word for bananas. It has its own source format, compiler, interpreter, runtime, and UI-handler format; it is for use to replace the old C# UI system that was used previously by Shay.

## File types
- `.mozf` — Mouz source programs.
- `.mozhl` — Mouz UI handlers.
- `.mozc` — compiled Mouz bootstrap bytecode container.

## Toolchain
- `mouz` — compiler, syntax checker, and expression evaluator.
- `mouzi` — bytecode interpreter and handler loader.
- Mouz Runtime — native runtime services and platform integrations.

## Language example

```mozf
value greeting := "Hello, Mouz!"
state clicks := 2
change clicks := clicks + 1

+mozUihandler |handlers/uihandle.mozhl|
+#makeUicomp [<|
    uiStructuredef:= .NET
    uiStat:= clicks * 10
    doComp |+*WinComp|
```

The language supports immutable values, mutable state, source-ordered updates, typed constant expressions, arithmetic, string concatenation, comparisons, equality, logical operators, and parentheses. See [the grammar](docs/GRAMMAR.md), [vocabulary](docs/VOCABULARY.md), and [expression reference](docs/EXPRESSIONS_AND_VOCABULARY.md).

## Build
Requirements: CMake 3.22+, a C11 compiler, and a C++20 compiler.

```sh
cmake -S . -B build
cmake --build build --config Release
./build/mouz check examples/state-values.mozf
./build/mouz eval "(12 + 8) * 3"
./build/mouz build examples/state-values.mozf -o state-values.mozc
./build/mouzi state-values.mozc
```

## Platform direction
- Windows: Mouz is the UI programming language, backed by a native C++/Win32 host. Mouz does not generate or require C# application code.
- Browser: JavaScript remains the browser execution layer. Mouz does not replace JavaScript.
- C and C++ implement the compiler/runtime/platform infrastructure; C# is the Windows UI language Mouz is intended to replace.

## Status
Early experimental language foundation. Expressions and basic value/state declarations are implemented. The `.mozc` format remains a bootstrap container; functions, scoped control flow, event execution, full `.mozhl` semantics, and native Windows rendering integration remain future milestones.
