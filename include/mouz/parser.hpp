#pragma once
#include "mouz/ast.hpp"
#include "mouz/lexer.hpp"
#include <vector>
namespace mouz {
struct Diagnostic { std::size_t line = 0; std::string message; };
struct ParseResult {
  Program program;
  std::vector<Diagnostic> diagnostics;
  [[nodiscard]] bool ok() const { return diagnostics.empty(); }
};
ParseResult parse_source(const std::vector<SourceLine>& lines);
}
