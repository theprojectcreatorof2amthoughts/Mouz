#pragma once
#include <string>
#include <vector>
#include "mouz/expression.hpp"
namespace mouz {
struct Component { std::string reference; };
struct Variable {
  std::string name;
  bool mutable_state = false;
  Value value = 0.0;
};
struct Program {
  std::string handler;
  std::string structure_definition;
  int ui_state = 0;
  std::vector<Component> components;
  std::vector<Variable> variables;
};
}
