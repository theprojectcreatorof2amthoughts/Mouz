#pragma once
#include <string>
#include <vector>
namespace mouz {
struct SourceLine { std::size_t number = 0; std::string text; };
std::vector<SourceLine> lex_source(const std::string& source);
}
