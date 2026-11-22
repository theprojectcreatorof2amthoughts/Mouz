#include "mouz/parser.hpp"
#include "mouz/expression.hpp"
#include <cmath>
#include <limits>
#include <string>
namespace mouz {
namespace {
bool pipe_value(const std::string& line, std::string& value) {
    const auto first = line.find('|');
    if (first == std::string::npos) return false;
    const auto second = line.find('|', first + 1);
    if (second == std::string::npos) return false;
    value = line.substr(first + 1, second - first - 1);
    return !value.empty();
}
}
ParseResult parse_source(const std::vector<SourceLine>& lines) {
    ParseResult result;
    bool block=false, structure=false, state=false, component=false, handler=false;
    for (const auto& source_line : lines) {
        const auto& line=source_line.text;
        if (line.rfind("+mozUihandler",0)==0) {
            if(handler) { result.diagnostics.push_back({source_line.number,"only one +mozUihandler declaration is allowed"}); continue; }
            if(!pipe_value(line,result.program.handler)) result.diagnostics.push_back({source_line.number,"expected handler path between pipe delimiters"});
            else handler=true;
        } else if(line.rfind("+#makeUicomp",0)==0) {
            if(block) result.diagnostics.push_back({source_line.number,"duplicate component block"});
            block=true;
        } else if(line.rfind("uiStructuredef:=",0)==0) {
            if(structure) { result.diagnostics.push_back({source_line.number,"uiStructuredef may only be declared once"}); continue; }
            result.program.structure_definition=line.substr(16);
            const auto first=result.program.structure_definition.find_first_not_of(" \\t");
            if(first!=std::string::npos) result.program.structure_definition.erase(0,first);
            structure=!result.program.structure_definition.empty();
            if(!structure) result.diagnostics.push_back({source_line.number,"uiStructuredef requires a value"});
        } else if(line.rfind("uiStat:=",0)==0) {
            if(state) { result.diagnostics.push_back({source_line.number,"uiStat may only be declared once"}); continue; }
            const auto evaluated=evaluate_expression(line.substr(8));
            if(!evaluated.ok()) { result.diagnostics.push_back({source_line.number,"invalid uiStat expression: "+evaluated.error}); continue; }
            const auto* numeric=std::get_if<double>(&evaluated.value);
            if(!numeric || !std::isfinite(*numeric) || std::floor(*numeric)!=*numeric ||
               *numeric<std::numeric_limits<int>::min() || *numeric>std::numeric_limits<int>::max()) {
                result.diagnostics.push_back({source_line.number,"uiStat expression must evaluate to a whole number in integer range"});
                continue;
            }
            result.program.ui_state=static_cast<int>(*numeric);
            state=true;
        } else if(line.rfind("doComp",0)==0) {
            if(!block) { result.diagnostics.push_back({source_line.number,"doComp must appear after +#makeUicomp"}); continue; }
            std::string reference;
            if(!pipe_value(line,reference)) result.diagnostics.push_back({source_line.number,"expected component reference between pipe delimiters"});
            else { result.program.components.push_back({reference}); component=true; }
        } else {
            result.diagnostics.push_back({source_line.number,"unrecognized Mouz directive: "+line});
        }
    }
    if(!block) result.diagnostics.push_back({0,"missing +#makeUicomp component block"});
    if(!structure) result.diagnostics.push_back({0,"missing uiStructuredef declaration"});
    if(!state) result.diagnostics.push_back({0,"missing uiStat declaration"});
    if(!component) result.diagnostics.push_back({0,"at least one doComp declaration is required"});
    if(!handler) result.diagnostics.push_back({0,"missing +mozUihandler declaration"});
    return result;
}
}
