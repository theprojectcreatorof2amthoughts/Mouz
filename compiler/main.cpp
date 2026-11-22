#include "mouz/lexer.hpp"
#include "mouz/parser.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
namespace {
bool read_file(const std::string& path, std::string& content) {
  std::ifstream file(path, std::ios::binary);
  if (!file) return false;
  content.assign(std::istreambuf_iterator<char>(file), {});
  return true;
}
void diagnostics(const mouz::ParseResult& result) {
  for (const auto& d : result.diagnostics) {
    if (d.line == 0) std::cerr << "mouz: error: " << d.message << '\n';
    else std::cerr << "mouz:" << d.line << ": error: " << d.message << '\n';
  }
}
bool emit(const std::string& path, const mouz::Program& p) {
  std::ofstream out(path, std::ios::binary);
  if (!out) return false;
  out << "MOUZBC 1\nhandler=" << p.handler << "\nstructure=" << p.structure_definition << "\nstate=" << p.ui_state << '\n';
  for (const auto& c : p.components) out << "component=" << c.reference << '\n';
  return static_cast<bool>(out);
}
}
int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Mouz compiler 0.1.0\nUsage: mouz check <file.mozf> | mouz build <file.mozf> -o <file.mozc>\n";
    return 2;
  }
  const std::string command = argv[1], input_path = argv[2];
  std::string source;
  if (!read_file(input_path, source)) { std::cerr << "mouz: cannot read " << input_path << '\n'; return 1; }
  const auto parsed = mouz::parse_source(mouz::lex_source(source));
  if (!parsed.ok()) { diagnostics(parsed); return 1; }
  if (command == "check") {
    std::cout << "Mouz: syntax OK (" << parsed.program.components.size() << " component(s))\n";
    return 0;
  }
  if (command == "build") {
    if (argc != 5 || std::string(argv[3]) != "-o") { std::cerr << "Usage: mouz build <file.mozf> -o <file.mozc>\n"; return 2; }
    if (!emit(argv[4], parsed.program)) { std::cerr << "mouz: cannot write " << argv[4] << '\n'; return 1; }
    std::cout << "Mouz: compiled " << input_path << " -> " << argv[4] << '\n';
    return 0;
  }
  std::cerr << "mouz: unknown command '" << command << "'\n";
  return 2;
}
