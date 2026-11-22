# Mouz expressions and vocabulary (proposal v0.2)

This document introduces the first expression vocabulary. The syntax is intentionally distinct from C#, JavaScript, and Shay. The expression evaluator in this milestone supports constant expressions; variables, function calls, and UI-state references are planned for the next semantic/runtime milestone.

## Core vocabulary

| Word | Meaning |
|---|---|
| `app` | Declares an application |
| `window` | Declares a top-level window |
| `component` | Declares a reusable UI component |
| `doComp` | Instantiates/references a handler-defined component |
| `state` | Declares reactive UI state (planned) |
| `value` | Declares an immutable value (planned) |
| `change` | Updates mutable state (planned) |
| `when` | Declares an event reaction (planned) |
| `onPress` | Press event vocabulary (planned) |
| `onInput` | Input event vocabulary (planned) |
| `show` | Makes a component visible (planned) |
| `hide` | Hides a component (planned) |
| `place` | Sets layout placement (planned) |
| `theme` | Selects a visual theme (planned) |
| `animate` | Declares an animation (planned) |
| `use` | Imports a handler or module (planned) |

## Expression operators

Implemented by `mouz eval`:

| Group | Operators | Meaning |
|---|---|---|
| Unary | `!`, `-`, `+` | Logical NOT and numeric sign |
| Multiplicative | `*`, `/`, `%` | Multiply, divide, remainder |
| Additive | `+`, `-` | Arithmetic; `+` concatenates if either operand is text |
| Comparison | `<`, `<=`, `>`, `>=` | Numeric comparisons |
| Equality | `==`, `!=` | Equality |
| Logical | `&&`, `||` | Boolean-style logic |
| Grouping | `(`, `)` | Explicit precedence |

Literals: decimal numbers, quoted strings with `\\n` and `\\t` escapes, `true`, and `false`.

Precedence from highest to lowest: primary/grouping, unary, multiplication/remainder, addition/subtraction, comparison, equality, AND, OR.

## Examples

```mozf
+mozUihandler |handlers/uihandle.mozhl|
+#makeUicomp [<|
    uiStructuredef:= .NET
    uiStat:=1
    doComp |+*WinComp|
```

Expression examples:

```text
(12 + 8) * 3       => 60
"Hello, " + "Mouz" => "Hello, Mouz"
(9 >= 9) && true   => true
!(4 > 7)           => true
17 % 5             => 2
```

## Compatibility and status

The original directives remain accepted. This milestone does not silently assign new meanings to `uiStat`, `.NET`, or handler component names. The vocabulary marked planned is a design proposal, not a claim that those statements are executable today.
