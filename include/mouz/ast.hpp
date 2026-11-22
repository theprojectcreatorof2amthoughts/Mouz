#pragma once
#include <string>
#include <vector>
namespace mouz {
struct Component { std::string reference; };
struct Program {
  std::string handler;
  std::string structure_definition;
  int ui_state = 0;
  std::vector<Component> components;
};
}
