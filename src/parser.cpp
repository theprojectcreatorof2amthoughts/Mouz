#include "mouz/parser.hpp"
#include "mouz/expression.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <string>

namespace mouz {
namespace {
bool pipe_value(const std::string& line, std::string& value) {
    const auto first=line.find('|');
    if(first==std::string::npos) return false;
    const auto second=line.find('|',first+1);
    if(second==std::string::npos) return false;
    value=line.substr(first+1,second-first-1);
    return !value.empty();
}
std::string trim(std::string value) {
    const auto first=value.find_first_not_of(" \t");
    if(first==std::string::npos) return {};
    const auto last=value.find_last_not_of(" \t");
    return value.substr(first,last-first+1);
}
bool valid_name(const std::string& name) {
    if(name.empty() || !(std::isalpha(static_cast<unsigned char>(name[0])) || name[0]=='_')) return false;
    for(char c:name) if(!(std::isalnum(static_cast<unsigned char>(c)) || c=='_')) return false;
    return true;
}
}

ParseResult parse_source(const std::vector<SourceLine>& lines) {
    ParseResult result;
    Environment environment;
    bool block=false, structure=false, state=false, component=false, handler=false;

    for(const auto& source_line:lines) {
        const auto& line=source_line.text;

        auto declare=[&](const std::string& keyword, bool mutable_state) -> bool {
            const std::string prefix=keyword+" ";
            if(line.rfind(prefix,0)!=0) return false;
            const auto assignment=line.find(":=",prefix.size());
            if(assignment==std::string::npos) {
                result.diagnostics.push_back({source_line.number,"expected ':=' in "+keyword+" declaration"});
                return true;
            }
            const auto name=trim(line.substr(prefix.size(),assignment-prefix.size()));
            const auto expression=trim(line.substr(assignment+2));
            if(!valid_name(name)) {
                result.diagnostics.push_back({source_line.number,"invalid identifier '"+name+"'"});
                return true;
            }
            if(environment.find(name)!=environment.end()) {
                result.diagnostics.push_back({source_line.number,"duplicate declaration of '"+name+"'"});
                return true;
            }
            if(expression.empty()) {
                result.diagnostics.push_back({source_line.number,"missing expression for '"+name+"'"});
                return true;
            }
            const auto evaluated=evaluate_expression(expression,environment);
            if(!evaluated.ok()) {
                result.diagnostics.push_back({source_line.number,"invalid initializer for '"+name+"': "+evaluated.error});
                return true;
            }
            environment.emplace(name,evaluated.value);
            result.program.variables.push_back({name,mutable_state,evaluated.value});
            return true;
        };

        auto change=[&]() -> bool {
            if(line.rfind("change ",0)!=0) return false;
            const auto assignment=line.find(":=",7);
            if(assignment==std::string::npos) {
                result.diagnostics.push_back({source_line.number,"expected ':=' in change statement"});
                return true;
            }
            const auto name=trim(line.substr(7,assignment-7));
            const auto found=environment.find(name);
            if(found==environment.end()) {
                result.diagnostics.push_back({source_line.number,"cannot change unknown state '"+name+"'"});
                return true;
            }
            auto variable=std::find_if(result.program.variables.begin(),result.program.variables.end(),
                [&](const Variable& item){return item.name==name;});
            if(variable==result.program.variables.end() || !variable->mutable_state) {
                result.diagnostics.push_back({source_line.number,"'"+name+"' is immutable; declare it with state to allow change"});
                return true;
            }
            const auto evaluated=evaluate_expression(trim(line.substr(assignment+2)),environment);
            if(!evaluated.ok()) {
                result.diagnostics.push_back({source_line.number,"invalid state update for '"+name+"': "+evaluated.error});
                return true;
            }
            if(found->second.index()!=evaluated.value.index()) {
                result.diagnostics.push_back({source_line.number,"state update for '"+name+"' changes its value type"});
                return true;
            }
            found->second=evaluated.value;
            variable->value=evaluated.value;
            return true;
        };

        if(declare("value",false) || declare("state",true) || change()) continue;

        if(line.rfind("+mozUihandler",0)==0) {
            if(handler) { result.diagnostics.push_back({source_line.number,"only one +mozUihandler declaration is allowed"}); continue; }
            if(!pipe_value(line,result.program.handler)) result.diagnostics.push_back({source_line.number,"expected handler path between pipe delimiters"});
            else handler=true;
        } else if(line.rfind("+#makeUicomp",0)==0) {
            if(block) result.diagnostics.push_back({source_line.number,"duplicate component block"});
            block=true;
        } else if(line.rfind("uiStructuredef:=",0)==0) {
            if(structure) { result.diagnostics.push_back({source_line.number,"uiStructuredef may only be declared once"}); continue; }
            result.program.structure_definition=trim(line.substr(16));
            structure=!result.program.structure_definition.empty();
            if(!structure) result.diagnostics.push_back({source_line.number,"uiStructuredef requires a value"});
        } else if(line.rfind("uiStat:=",0)==0) {
            if(state) { result.diagnostics.push_back({source_line.number,"uiStat may only be declared once"}); continue; }
            const auto evaluated=evaluate_expression(trim(line.substr(8)),environment);
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
