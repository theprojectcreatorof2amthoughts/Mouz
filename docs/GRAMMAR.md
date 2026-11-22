# Mouz grammar draft

The accepted bootstrap grammar currently consists of optional value/state declarations followed by the UI handler and component block. `WS` means optional horizontal whitespace.

```ebnf
program          = { value_decl | state_decl | change_stmt }, handler, component_block ;
value_decl       = "value", WS, identifier, WS, ":=", WS, expression ;
state_decl       = "state", WS, identifier, WS, ":=", WS, expression ;
change_stmt      = "change", WS, identifier, WS, ":=", WS, expression ;
handler          = "+mozUihandler", WS, "|", path, "|" ;
component_block  = "+#makeUicomp", WS, "[<|", newline,
                   structure, newline,
                   state_status, newline,
                   { component, newline } ;
structure        = "uiStructuredef:=", WS, identifier ;
state_status     = "uiStat:=", WS, expression ;
component        = "doComp", WS, "|", component_reference, "|" ;
```

## Expression grammar

```ebnf
expression = logical_or ;
logical_or = logical_and, { "||", logical_and } ;
logical_and = equality, { "&&", equality } ;
equality = comparison, { ("==" | "!="), comparison } ;
comparison = additive, { ("<" | "<=" | ">" | ">="), additive } ;
additive = multiplicative, { ("+" | "-"), multiplicative } ;
multiplicative = unary, { ("*" | "/" | "%"), unary } ;
unary = ["!" | "-" | "+"], primary ;
primary = number | string | "true" | "false" | identifier | "(", expression, ")" ;
```

Declarations are evaluated in source order. `value` is immutable; `state` is mutable; `change` updates an existing mutable name and preserves its value type. `uiStat` must evaluate to an integer in range. The current bootstrap parser rejects unknown directives, duplicate names, malformed expressions, and component declarations before the component block.

The component block closing delimiter, nested UI grammar, function syntax, and event syntax remain to be specified from the language author's examples.
