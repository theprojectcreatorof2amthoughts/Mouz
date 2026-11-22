#include "mouz/parser.hpp"
#include <charconv>
#include <string_view>
namespace mouz {
namespace {
bool pipe_value(const std::string& line, std::string& value) {
  const auto first = line.find('|');
  if (first == std::string::npos) return false;
  const auto second = line.find('|', first + 1);
  if (second == std::string::npos) return false;
  value = line.substr(first + 1, second - first - 1);
  return !value.empty();
}
}
ParseResult parse_source(const std::vector<SourceLine>& lines) {
  ParseResult result;
  bool block = false, structure = false, state = false, component = false;
  for (const auto& source_line : lines) {
    const auto& line = source_line.text;
    if (line.rfind("+mozUihandler", 0) == 0) {
      if (!pipe_value(line, result.program.handler)) result.diagnostics.push_back({source_line.number, "expected handler path between pipe delimiters"});
    } else if (line.rfind("+#makeUicomp", 0) == 0) {
      block = true;
    } else if (line.rfind("uiStructuredef:=", 0) == 0) {
      result.program.structure_definition = line.substr(16);
      const auto first = result.program.structure_definition.find_first_not_of(" \t");
      if (first != std::string::npos) result.program.structure_definition.erase(0, first);
      structure = !result.program.structure_definition.empty();
      if (!structure) result.diagnostics.push_back({source_line.number, "uiStructuredef requires a value"});
    } else if (line.rfind("uiStat:=", 0) == 0) {
      const auto value = std::string_view(line).substr(8);
      int parsed_state = 0;
      const auto parsed = std::from_chars(value.data(), value.data() + value.size(), parsed_state);
      if (parsed.ec != std::errc{} || parsed.ptr != value.data() + value.size()) result.diagnostics.push_back({source_line.number, "uiStat must currently be an integer"});
      else { result.program.ui_state = parsed_state; state = true; }
    } else if (line.rfind("doComp", 0) == 0) {
      std::string reference;
      if (!pipe_value(line, reference)) result.diagnostics.push_back({source_line.number, "expected component reference between pipe delimiters"});
      else { result.program.components.push_back({reference}); component = true; }
    }
  }
  if (!block) result.diagnostics.push_back({0, "missing +#makeUicomp component block"});
  if (!structure) result.diagnostics.push_back({0, "missing uiStructuredef declaration"});
  if (!state) result.diagnostics.push_back({0, "missing uiStat declaration"});
  if (!component) result.diagnostics.push_back({0, "at least one doComp declaration is required"});
  if (result.program.handler.empty()) result.diagnostics.push_back({0, "missing +mozUihandler declaration"});
  return result;
}
}
