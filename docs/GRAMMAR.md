# Mouz grammar draft

The grammar below specifies the currently accepted bootstrap form. `WS` means optional horizontal whitespace. Handler paths are delimited by vertical bars.

```ebnf
program          = handler, component_block ;
handler          = "+mozUihandler", WS, "|", path, "|" ;
component_block  = "+#makeUicomp", WS, "[<|", newline,
                   structure, newline,
                   state, newline,
                   { component, newline } ;
structure        = "uiStructuredef:=", WS, identifier ;
state            = "uiStat:=", WS, integer_expression ;
component        = "doComp", WS, "|", component_reference, "|" ;
```

The expression grammar for `uiStat` and `mouz eval` is:

```ebnf
expression = logical_or ;
logical_or = logical_and, { "||", logical_and } ;
logical_and = equality, { "&&", equality } ;
equality = comparison, { ("==" | "!="), comparison } ;
comparison = additive, { ("<" | "<=" | ">" | ">="), additive } ;
additive = multiplicative, { ("+" | "-"), multiplicative } ;
multiplicative = unary, { ("*" | "/" | "%"), unary } ;
unary = ["!" | "-" | "+"], primary ;
primary = number | string | "true" | "false" | "(", expression, ")" ;
```

The parser rejects unknown directives, duplicate singleton declarations, malformed expressions, and component declarations that appear before the component block. The component block closing delimiter and richer nested UI grammar remain to be specified from the author's examples.
