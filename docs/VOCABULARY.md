# Mouz vocabulary

| Word | Meaning | Status |
|---|---|---|
| `value` | Declares an immutable named value using `value name := expression` | Implemented |
| `state` | Declares a mutable state value using `state name := expression` | Implemented |
| `change` | Updates an existing mutable state using `change name := expression` | Implemented |
| `uiStructuredef` | Selects the UI structure definition | Implemented |
| `uiStat` | Sets the UI state integer from an expression | Implemented |
| `doComp` | References a handler-defined UI component | Implemented |
| `app` | Application declaration | Planned |
| `window` | Top-level window declaration | Planned |
| `component` | Reusable UI component definition | Planned |
| `when` | Event reaction declaration | Planned |
| `onPress` | Pointer/touch press event | Planned |
| `onInput` | Input change event | Planned |
| `show` / `hide` | Visibility operations | Planned |
| `place` | Layout placement | Planned |
| `theme` | Theme declaration | Planned |
| `animate` | Transition or animation declaration | Planned |
| `use` | Import a Mouz module/handler | Planned |
| `give` | Return a value from a function | Planned |
| `make` | Construct a component/value | Planned |

## Example

```mozf
value greeting := "Hello, Mouz!"
state clicks := 2
change clicks := clicks + 1
```

`value` is immutable. `state` is mutable. `change` can update only a previously declared `state`, and the new value must retain the original value type. Declarations are processed in source order, so a later expression can refer to earlier values and state.

## Naming principles

Mouz vocabulary should be concise, consistent, readable, and distinct from C# and JavaScript. Existing syntax supplied by the language author takes precedence. A proposed keyword becomes part of the language only after grammar, semantics, diagnostics, and tests exist.
