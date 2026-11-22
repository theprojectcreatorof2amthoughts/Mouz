# Mouz 🍌

Mouz (موز) is an independent UI programming language. Its name comes from the Arabic word for bananas. It has its own source format, compiler, interpreter, runtime, and UI-handler format; it is not a Shay subproject.

## File types
- `.mozf` — Mouz source programs.
- `.mozhl` — Mouz UI handlers.
- `.mozc` — compiled Mouz bytecode container (bootstrap format).

## Toolchain
- `mouz` — compiler, syntax checker, and constant-expression evaluator.
- `mouzi` — bytecode interpreter and handler loader.
- Mouz Runtime — native runtime services and platform integrations.

## Platform direction
- Windows: Mouz is the UI programming language. Application UI logic is authored in `.mozf`, not C#.
- Browser: JavaScript remains the browser execution layer. Mouz does not replace JavaScript.
- C, C++, and C# may implement compiler/runtime/platform infrastructure; they are implementation technologies, not the language users write UI applications in.

## Language example

```mozf
+mozUihandler |handlers/uihandle.mozhl|
+#makeUicomp [<|
    uiStructuredef:= .NET
    uiStat:= (2 + 3) * 4
    doComp |+*WinComp|
```

The compiler validates the supplied handler/component directives, evaluates constant expressions in `uiStat`, and emits a versioned bytecode container. The initial expression language supports numeric and string literals, booleans, grouping, arithmetic, comparisons, equality, and logical operators.

## Build
Requirements: CMake 3.22+, a C11 compiler, and a C++20 compiler.

```sh
cmake -S . -B build
cmake --build build --config Release
./build/mouz check examples/first.mozf
./build/mouz eval "(12 + 8) * 3"
./build/mouz build examples/first.mozf -o first.mozc
./build/mouzi first.mozc
```

## Design documents
- [Language foundation](docs/LANGUAGE_FOUNDATION.md)
- [Grammar draft](docs/GRAMMAR.md)
- [Vocabulary](docs/VOCABULARY.md)
- [Expressions](docs/EXPRESSIONS_AND_VOCABULARY.md)

## Status
Early experimental language foundation. Expression evaluation is implemented and integrated into `uiStat`. The bytecode is still a bootstrap metadata container; general variables, functions, event execution, and native UI rendering are follow-up milestones.
