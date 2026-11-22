# Mouz expressions and values

Mouz has a constant-expression evaluator exposed through `mouz eval`. The same evaluator is used in `.mozf` declarations and `uiStat:=`.

## Values and state

```mozf
value greeting := "Hello, Mouz!"
state clicks := 2
change clicks := clicks + 1
```

- `value` declares an immutable value.
- `state` declares a mutable value.
- `change` updates a previously declared `state`.
- Declarations are evaluated in source order, so expressions can refer to earlier declarations.
- A `change` cannot target an immutable value, an unknown name, or change the variable's value type.

The first implementation infers each value's type from its initializer: number, text, or Boolean.

## Operators

| Precedence (high → low) | Operators | Meaning |
|---|---|---|
| Primary | literals, identifiers, `( )` | Values, references, grouping |
| Unary | `!`, `-`, `+` | Logical NOT and numeric sign |
| Multiplicative | `*`, `/`, `%` | Multiply, divide, remainder |
| Additive | `+`, `-` | Arithmetic; `+` concatenates if either operand is text |
| Comparison | `<`, `<=`, `>`, `>=` | Numeric comparison |
| Equality | `==`, `!=` | Equality |
| Logical | `&&`, `||` | Boolean-style logic |

Literals include decimal numbers, quoted strings (`\\n` and `\\t` escapes), and `true` / `false`. Numeric operators reject text and booleans. Division by zero, malformed literals, unknown identifiers, and invalid tokens produce diagnostics.

## Examples

```sh
mouz eval "(12 + 8) * 3"        # 60
mouz eval '"Hello, " + "Mouz"' # Hello, Mouz
mouz eval "(9 >= 9) && true"   # true
mouz eval "!(4 > 7)"           # true
mouz eval "17 % 5"             # 2
```

The same grammar is available in `.mozf` programs:

```mozf
value multiplier := 10
state clicks := 2
change clicks := clicks + 1
+mozUihandler |handlers/uihandle.mozhl|
+#makeUicomp [<|
    uiStructuredef:= .NET
    uiStat:= clicks * multiplier
    doComp |+*WinComp|
```

`uiStat` must evaluate to a whole number within the C++ `int` range. User-defined functions, scoped blocks, and runtime reactive bindings remain future semantic work.
