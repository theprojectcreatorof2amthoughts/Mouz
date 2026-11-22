# Mouz vocabulary

This vocabulary is the proposed direction for the next language layer. Words marked *planned* are reserved design vocabulary, not yet executable statements.

| Word | Meaning | Status |
|---|---|---|
| `app` | Application declaration | Planned |
| `window` | Top-level window declaration | Planned |
| `component` | Reusable UI component definition | Planned |
| `doComp` | Instantiate/reference a handler component | Recognized |
| `uiStructuredef` | Select UI structure definition | Recognized |
| `uiStat` | UI state integer expression | Recognized; expressions supported |
| `state` | Mutable reactive value | Planned |
| `value` | Immutable binding | Planned |
| `change` | Update mutable state | Planned |
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
| `check` | Compile-time validation directive | Planned |

## Naming principles

Mouz vocabulary should be concise, consistent, readable, and distinct from C# and JavaScript. Existing syntax supplied by the language author takes precedence over proposals. A keyword becomes part of the language only after grammar, semantics, diagnostics, and tests exist.
