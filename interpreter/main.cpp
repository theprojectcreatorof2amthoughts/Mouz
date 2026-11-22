#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "mouz_runtime.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Mouz interpreter 0.1.0\nUsage: mouzi <file.mozc>\n";
    return 2;
  }

  const std::filesystem::path bytecode_path(argv[1]);
  std::ifstream input(bytecode_path);
  if (!input) {
    std::cerr << "mouzi: cannot read " << argv[1] << '\n';
    return 1;
  }

  std::string line;
  if (!std::getline(input, line) || line != "MOUZBC 1") {
    std::cerr << "mouzi: unsupported or invalid bytecode header\n";
    return 1;
  }

  std::string handler_path;
  std::string structure;
  int state = 0;
  unsigned int components = 0;
  while (std::getline(input, line)) {
    if (line.rfind("handler=", 0) == 0) handler_path = line.substr(8);
    else if (line.rfind("structure=", 0) == 0) structure = line.substr(10);
    else if (line.rfind("state=", 0) == 0) {
      try { state = std::stoi(line.substr(6)); }
      catch (const std::exception&) {
        std::cerr << "mouzi: invalid state value in bytecode\n";
        return 1;
      }
    } else if (line.rfind("component=", 0) == 0) {
      ++components;
    }
  }

  std::filesystem::path resolved_handler(handler_path);
  std::ifstream handler(resolved_handler);
  if (!handler && resolved_handler.is_relative()) {
    resolved_handler = bytecode_path.parent_path() / resolved_handler;
    handler.clear();
    handler.open(resolved_handler);
  }
  if (!handler) {
    std::cerr << "mouzi: cannot load UI handler '" << handler_path << "'\n";
    return 1;
  }

  mouz_runtime_start();
  std::cout << "Mouz interpreter: program loaded\n"
            << "  handler: " << resolved_handler.string() << " (loaded)\n"
            << "  structure: " << structure << '\n'
            << "  UI state: " << state << '\n'
            << "  components: " << components << '\n';
  mouz_runtime_stop();
  return 0;
}
