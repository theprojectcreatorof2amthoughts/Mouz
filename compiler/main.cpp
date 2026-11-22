#include "mouz/expression.hpp"
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
    for (const auto& diagnostic : result.diagnostics) {
        if (diagnostic.line == 0)
            std::cerr << "mouz: error: " << diagnostic.message << '\n';
        else
            std::cerr << "mouz:" << diagnostic.line << ": error: " << diagnostic.message << '\n';
    }
}

bool emit(const std::string& path, const mouz::Program& program) {
    std::ofstream output(path, std::ios::binary);
    if (!output) return false;
    output << "MOUZBC 1\n"
           << "handler=" << program.handler << '\n'
           << "structure=" << program.structure_definition << '\n'
           << "state=" << program.ui_state << '\n';
    for (const auto& component : program.components)
        output << "component=" << component.reference << '\n';
    return static_cast<bool>(output);
}
}

int main(int argc, char** argv) {
    if (argc >= 3 && std::string(argv[1]) == "eval") {
        const auto result = mouz::evaluate_expression(argv[2]);
        if (!result.ok()) {
            std::cerr << "mouz: expression error: " << result.error << '\n';
            return 1;
        }
        std::cout << mouz::value_to_string(result.value) << '\n';
        return 0;
    }

    if (argc < 3) {
        std::cerr << "Mouz compiler 0.2.0\n"
                  << "Usage:\n"
                  << "  mouz check <file.mozf>\n"
                  << "  mouz build <file.mozf> -o <file.mozc>\n"
                  << "  mouz eval <expression>\n";
        return 2;
    }

    const std::string command = argv[1];
    const std::string input_path = argv[2];
    std::string source;
    if (!read_file(input_path, source)) {
        std::cerr << "mouz: cannot read " << input_path << '\n';
        return 1;
    }

    const auto parsed = mouz::parse_source(mouz::lex_source(source));
    if (!parsed.ok()) {
        diagnostics(parsed);
        return 1;
    }

    if (command == "check") {
        std::cout << "Mouz: syntax OK (" << parsed.program.components.size()
                  << " component(s))\n";
        return 0;
    }

    if (command == "build") {
        if (argc != 5 || std::string(argv[3]) != "-o") {
            std::cerr << "Usage: mouz build <file.mozf> -o <file.mozc>\n";
            return 2;
        }
        if (!emit(argv[4], parsed.program)) {
            std::cerr << "mouz: cannot write " << argv[4] << '\n';
            return 1;
        }
        std::cout << "Mouz: compiled " << input_path << " -> " << argv[4] << '\n';
        return 0;
    }

    std::cerr << "mouz: unknown command '" << command << "'\n";
    return 2;
}
