#pragma once
#include <map>
#include <string>
#include <variant>
namespace mouz {
using Value = std::variant<double, bool, std::string>;
using Environment = std::map<std::string, Value>;
struct ExpressionResult {
  Value value = 0.0;
  std::string error;
  [[nodiscard]] bool ok() const { return error.empty(); }
};
ExpressionResult evaluate_expression(const std::string& source, const Environment& environment = {});
std::string value_to_string(const Value& value);
}
