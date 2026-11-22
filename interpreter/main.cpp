#include <fstream>
#include <iostream>
#include <string>
#include "mouz_runtime.h"
int main(int argc, char** argv) {
  if (argc != 2) { std::cerr << "Mouz interpreter 0.1.0\nUsage: mouzi <file.mozc>\n"; return 2; }
  std::ifstream input(argv[1]);
  if (!input) { std::cerr << "mouzi: cannot read " << argv[1] << '\n'; return 1; }
  std::string line;
  if (!std::getline(input, line) || line != "MOUZBC 1") { std::cerr << "mouzi: unsupported or invalid bytecode header\n"; return 1; }
  std::string handler, structure;
  int state = 0;
  unsigned int components = 0;
  while (std::getline(input, line)) {
    if (line.rfind("handler=", 0) == 0) handler = line.substr(8);
    else if (line.rfind("structure=", 0) == 0) structure = line.substr(10);
    else if (line.rfind("state=", 0) == 0) state = std::stoi(line.substr(6));
    else if (line.rfind("component=", 0) == 0) ++components;
  }
  mouz_runtime_start();
  std::cout << "Mouz interpreter: program loaded\n  handler: " << handler << "\n  structure: " << structure << "\n  UI state: " << state << "\n  components: " << components << '\n';
  mouz_runtime_stop();
  return 0;
}
