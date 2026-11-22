#include "mouz/lexer.hpp"
#include <sstream>
namespace mouz {
std::vector<SourceLine> lex_source(const std::string& source) {
  std::vector<SourceLine> lines;
  std::istringstream input(source);
  std::string line;
  std::size_t number = 0;
  while (std::getline(input, line)) {
    ++number;
    const auto first = line.find_first_not_of(" \t\r");
    if (first == std::string::npos) continue;
    line.erase(0, first);
    const auto last = line.find_last_not_of(" \t\r");
    line.erase(last + 1);
    if (!line.empty()) lines.push_back({number, line});
  }
  return lines;
}
}
