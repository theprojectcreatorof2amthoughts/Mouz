# Mouz language foundation

## Confirmed syntax

- `.mozf` is the Mouz source extension.
- `.mozhl` is the UI-handler extension.
- `+mozUihandler |path.mozhl|` selects a handler.
- `+#makeUicomp [<|` begins a UI component block.
- `uiStructuredef:= value` selects a UI structure definition.
- `uiStat:= integer` sets the UI state value.
- `doComp |+*ComponentName|` references a component.

## Bootstrap behavior

The first parser recognizes the directives above and checks that required declarations exist. The compiler emits a versioned text bytecode container (`MOUZBC 1`). The interpreter reads that container and reports loaded metadata.

This is a bootstrap format, not the final bytecode specification. Handler semantics, block termination, expressions, event syntax, component properties, and the platform ABI remain open design work. New rules should come from Mouz examples, not assumptions.

## Platform boundaries

Mouz application code is authored in `.mozf`. The Windows backend is intended to expose native Windows UI capabilities to Mouz programs. C, C++, and C# can implement compiler/runtime/platform infrastructure; they are not generated source languages for Mouz applications.

The Browser backend continues to use JavaScript as its browser execution layer. Mouz does not replace JavaScript.
