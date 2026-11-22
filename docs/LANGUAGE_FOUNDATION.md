# Mouz language foundation

## Confirmed source forms

- `.mozf` is the Mouz source extension.
- `.mozhl` is the UI-handler extension.
- `+mozUihandler |path.mozhl|` selects a handler.
- `+#makeUicomp [<|` begins a UI component block.
- `uiStructuredef:= value` selects a UI structure definition.
- `uiStat:= expression` sets the UI state integer from a constant or declared value expression.
- `doComp |+*ComponentName|` references a component.
- `value name := expression` declares an immutable value.
- `state name := expression` declares a mutable value.
- `change name := expression` updates an existing mutable state.

## Expressions and values

Mouz currently supports numeric, text, and Boolean literals; unary operators; arithmetic; string concatenation; comparisons; equality; logical operators; parentheses; and references to earlier `value` and `state` declarations. Declarations are processed in source order. `change` cannot modify an immutable value or change a variable's value type.

The compiler exposes the expression evaluator as `mouz eval <expression>`. The same evaluator is used by `.mozf` parsing.

## Toolchain and bytecode

The C++20 compiler performs lexical line processing, parsing, validation, expression evaluation, and emits a versioned `.mozc` bootstrap container. The interpreter loads the container and referenced `.mozhl` handler metadata. The current bytecode is a text-based bootstrap representation, not the final VM instruction set.

## Platform boundaries

Mouz is an independent language project. The Windows backend is intended to expose native Windows UI capabilities to Mouz programs. C, C++, and C# can implement compiler/runtime/platform infrastructure; application UI logic is authored in `.mozf`, not generated C# source.

The Browser backend continues to use JavaScript as its browser execution layer. Mouz does not replace JavaScript.

## Next language milestones

Function definitions/calls, typed function signatures, scoped blocks, conditionals, event declarations, component properties, and an executable bytecode VM need dedicated grammar, semantics, diagnostics, and tests before they are considered supported.
