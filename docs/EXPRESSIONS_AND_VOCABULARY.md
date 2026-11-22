# Mouz expressions

Mouz has a constant-expression evaluator exposed through `mouz eval`. The same evaluator is used when parsing `uiStat:=` in `.mozf` programs.

## Operators

| Precedence (high → low) | Operators | Meaning |
|---|---|---|
| Primary | literals, `( )` | Values and grouping |
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

The same grammar is available in `uiStat`:

```mozf
+mozUihandler |handlers/uihandle.mozhl|
+#makeUicomp [<|
    uiStructuredef:= .NET
    uiStat:= (2 + 3) * 4
    doComp |+*WinComp|
```

`uiStat` must evaluate to a whole number within the C++ `int` range. Variables, function calls, and reactive state references are future semantic work, not silently treated as supported today.
