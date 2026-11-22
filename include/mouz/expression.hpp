#pragma once
#include <string>
#include <variant>
namespace mouz {
using Value = std::variant<double, bool, std::string>;
struct ExpressionResult {
  Value value = 0.0;
  std::string error;
  [[nodiscard]] bool ok() const { return error.empty(); }
};
ExpressionResult evaluate_expression(const std::string& source);
std::string value_to_string(const Value& value);
}
