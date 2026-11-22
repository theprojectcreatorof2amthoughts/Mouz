#include "mouz/expression.hpp"
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>
namespace mouz {
namespace {
enum class Kind { Number, String, Identifier, True, False, Operator, Left, Right, End };
struct Token { Kind kind; std::string text; };
std::vector<Token> tokenize(const std::string& s) {
  std::vector<Token> out;
  std::size_t i=0;
  while(i<s.size()) {
    if(std::isspace(static_cast<unsigned char>(s[i]))) { ++i; continue; }
    if(std::isdigit(static_cast<unsigned char>(s[i])) || (s[i]=='.' && i+1<s.size() && std::isdigit(static_cast<unsigned char>(s[i+1])))) {
      auto start=i++;
      while(i<s.size() && (std::isdigit(static_cast<unsigned char>(s[i])) || s[i]=='.')) ++i;
      out.push_back({Kind::Number,s.substr(start,i-start)}); continue;
    }
    if(s[i]=='"') {
      ++i; std::string value; bool closed=false;
      while(i<s.size()) {
        char c=s[i++];
        if(c=='"') { closed=true; break; }
        if(c=='\\' && i<s.size()) {
          char e=s[i++];
          if(e=='n') value+='\n'; else if(e=='t') value+='\t'; else value+=e;
        } else value+=c;
      }
      if(!closed) throw std::runtime_error("unterminated string literal");
      out.push_back({Kind::String,value}); continue;
    }
    if(std::isalpha(static_cast<unsigned char>(s[i])) || s[i]=='_') {
      auto start=i++;
      while(i<s.size() && (std::isalnum(static_cast<unsigned char>(s[i])) || s[i]=='_')) ++i;
      auto word=s.substr(start,i-start);
      if(word=="true") out.push_back({Kind::True,word});
      else if(word=="false") out.push_back({Kind::False,word});
      else out.push_back({Kind::Identifier,word});
      continue;
    }
    if(s[i]=='(') { out.push_back({Kind::Left,"("}); ++i; continue; }
    if(s[i]==')') { out.push_back({Kind::Right,")"}); ++i; continue; }
    std::string op(1,s[i++]);
    if(i<s.size() && ((op=="=" && s[i]=='=') || (op=="!" && s[i]=='=') ||
       (op=="<" && s[i]=='=') || (op==">" && s[i]=='=') ||
       (op=="&" && s[i]=='&') || (op=="|" && s[i]=='|'))) op+=s[i++];
    if(op=="+"||op=="-"||op=="*"||op=="/"||op=="%"||op=="=="||op=="!="||
       op=="<"||op=="<="||op==">"||op==">="||op=="&&"||op=="||"||op=="!") {
      out.push_back({Kind::Operator,op}); continue;
    }
    throw std::runtime_error("unexpected character '" + op + "'");
  }
  out.push_back({Kind::End,""});
  return out;
}
bool truth(const Value& v) {
  if(auto p=std::get_if<bool>(&v)) return *p;
  if(auto p=std::get_if<double>(&v)) return *p!=0.0;
  return !std::get<std::string>(v).empty();
}
double number(const Value& v) {
  if(auto p=std::get_if<double>(&v)) return *p;
  throw std::runtime_error("numeric operator requires numbers");
}
class Parser {
  std::vector<Token> t; const Environment& environment; std::size_t p=0;
  const Token& peek() const { return t[p]; }
  Token take() { return t[p++]; }
  bool accept(const std::string& op) { if(peek().kind==Kind::Operator && peek().text==op) { ++p; return true; } return false; }
  Value primary() {
    auto x=take();
    if(x.kind==Kind::Number) {
      char* end=nullptr; double v=std::strtod(x.text.c_str(),&end);
      if(!end || *end!='\0' || !std::isfinite(v)) throw std::runtime_error("invalid numeric literal");
      return v;
    }
    if(x.kind==Kind::String) return x.text;
    if(x.kind==Kind::Identifier) { auto found=environment.find(x.text); if(found==environment.end()) throw std::runtime_error("unknown identifier '" + x.text + "'"); return found->second; }
    if(x.kind==Kind::True) return true;
    if(x.kind==Kind::False) return false;
    if(x.kind==Kind::Left) { auto v=logical_or(); if(take().kind!=Kind::Right) throw std::runtime_error("expected ')'"); return v; }
    if(x.kind==Kind::Operator && (x.text=="!"||x.text=="-"||x.text=="+")) {
      auto v=primary();
      if(x.text=="!") return !truth(v);
      return x.text=="-" ? Value(-number(v)) : Value(number(v));
    }
    throw std::runtime_error("expected a value");
  }
  Value mul() { auto v=primary(); while(peek().text=="*"||peek().text=="/"||peek().text=="%") { auto op=take().text; auto b=number(primary()),a=number(v); if((op=="/"||op=="%")&&b==0) throw std::runtime_error("division by zero"); v=op=="*"?Value(a*b):op=="/"?Value(a/b):Value(std::fmod(a,b)); } return v; }
  Value add() { auto v=mul(); while(peek().text=="+"||peek().text=="-") { auto op=take().text; auto rhs=mul(); if(op=="+" && (std::holds_alternative<std::string>(v)||std::holds_alternative<std::string>(rhs))) v=value_to_string(v)+value_to_string(rhs); else v=op=="+"?Value(number(v)+number(rhs)):Value(number(v)-number(rhs)); } return v; }
  Value compare() { auto v=add(); while(peek().text=="<"||peek().text=="<="||peek().text==">"||peek().text==">=") { auto op=take().text; auto b=number(add()),a=number(v); v=op=="<"?Value(a<b):op=="<="?Value(a<=b):op==">"?Value(a>b):Value(a>=b); } return v; }
  Value equality() { auto v=compare(); while(peek().text=="=="||peek().text=="!=") { auto op=take().text; auto b=compare(); bool equal; if(v.index()==b.index()) { if(auto numeric=std::get_if<double>(&v)) equal=*numeric==std::get<double>(b); else if(auto boolean=std::get_if<bool>(&v)) equal=*boolean==std::get<bool>(b); else equal=std::get<std::string>(v)==std::get<std::string>(b); } else equal=false; v=(op=="=="?equal:!equal); } return v; }
  Value logical_and() { auto v=equality(); while(accept("&&")) { auto b=equality(); v=truth(v)&&truth(b); } return v; }
  Value logical_or() { auto v=logical_and(); while(accept("||")) { auto b=logical_and(); v=truth(v)||truth(b); } return v; }
public:
  explicit Parser(const std::string& s, const Environment& env):t(tokenize(s)),environment(env){}
  Value run() { auto v=logical_or(); if(peek().kind!=Kind::End) throw std::runtime_error("unexpected token '" + peek().text + "'"); return v; }
};
}
ExpressionResult evaluate_expression(const std::string& source, const Environment& environment) {
  try { return {Parser(source,environment).run(),{}}; }
  catch(const std::exception& e) { return {0.0,e.what()}; }
}
std::string value_to_string(const Value& value) {
  if(auto p=std::get_if<std::string>(&value)) return *p;
  if(auto p=std::get_if<bool>(&value)) return *p?"true":"false";
  std::ostringstream out; out.precision(15); out<<std::get<double>(value); return out.str();
}
}
