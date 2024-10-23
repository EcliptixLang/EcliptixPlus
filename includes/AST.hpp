#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <optional>
#include <map>
#include <Lexer.hpp>

namespace AST {

    class ExprAST {
    public:
      virtual ~ExprAST() = default;
      virtual std::string getType() const = 0;
    };

    class NumberExpr : public ExprAST {
    public:
      double Value;
      NumberExpr(double Value) : Value(Value) {}
      std::string getType() const override {
        return "Number";
      }
    };

    class Program : public ExprAST {
    public:
      std::vector<std::unique_ptr<AST::ExprAST>> body;
      Program(std::vector<std::unique_ptr<AST::ExprAST>> body) : body(std::move(body)) {}

      std::string getType() const override {
        return "Program";
      }
    };

    class Identifier : public ExprAST {
      public:
        std::string name;

        Identifier(std::string name) : name(name) {}

        std::string getType() const override {
          return "Identifier";
        }
    };

    class VariableExpr : public ExprAST {
    public:
      std::string Name;
      std::string Type;
      std::unique_ptr<ExprAST> Value;
      bool constant;
      VariableExpr(const std::string &Name, const std::string &Type, std::unique_ptr<ExprAST> Value, bool constant)
       : Name(Name), Type(Type), Value(std::move(Value)), constant(constant) {}
      
      std::string getType() const override {
        return "VarExpr";
      }
    };

    class AssignmentExpr : public ExprAST {
      public:
        std::unique_ptr<AST::ExprAST> assignee;
        std::unique_ptr<AST::ExprAST> value;

        AssignmentExpr(std::unique_ptr<AST::ExprAST> assignee, std::unique_ptr<AST::ExprAST> value) : assignee(std::move(assignee)), value(std::move(value)) {}

        std::string getType() const override {
          return "AssignmentExpr";
        }
    };

    class BinaryExpr : public ExprAST {
    public:
      char Op;
      std::unique_ptr<ExprAST> LHS, RHS;
      BinaryExpr(char Op, std::unique_ptr<ExprAST> LHS,
                    std::unique_ptr<ExprAST> RHS)
          : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
      std::string getType() const override {
        return "BinExpr";
      }
    };

    class CallExpr : public ExprAST {
    public:
      std::unique_ptr<ExprAST> Callee;
      std::vector<std::unique_ptr<ExprAST>> Args;

      CallExpr(std::unique_ptr<ExprAST> &Callee,
                  std::vector<std::unique_ptr<ExprAST>> Args)
          : Callee(std::move(Callee)), Args(std::move(Args)) {}
      std::string getType() const override {
        return "CallExpr";
      }
    };

    class WhenDeclaration : public ExprAST {
      public:
        std::unique_ptr<ExprAST> conditional;
        Lexer::TokenType operatorType;
        std::vector<std::unique_ptr<ExprAST>> consequent;

        WhenDeclaration(std::unique_ptr<ExprAST> conditional, Lexer::TokenType operatorType, std::vector<std::unique_ptr<ExprAST>> consequent)
            : conditional(std::move(conditional)), operatorType(operatorType), consequent(std::move(consequent)) {}
        std::string getType() const override {
          return "WhenDeclaration";
        }
    };

    class WhileDeclaration : public ExprAST {
      public:
        std::unique_ptr<ExprAST> conditional;
        Lexer::TokenType operatorType;
        std::vector<std::unique_ptr<ExprAST>> consequent;

        WhileDeclaration(std::unique_ptr<ExprAST> conditional, Lexer::TokenType operatorType, std::vector<std::unique_ptr<ExprAST>> consequent)
            : conditional(std::move(conditional)), operatorType(operatorType), consequent(std::move(consequent)) {}
        std::string getType() const override {
          return "WhileDeclaration";
        }
    };

    class IfStatement : public ExprAST {
      public:
        std::unique_ptr<ExprAST> conditional;
        Lexer::TokenType operatorType;
        std::vector<std::unique_ptr<ExprAST>> consequent;
        std::vector<std::unique_ptr<ExprAST>> alternate;

        IfStatement(std::unique_ptr<ExprAST> conditional, Lexer::TokenType operatorType, std::vector<std::unique_ptr<ExprAST>> consequent, std::vector<std::unique_ptr<ExprAST>> alternate)
            : conditional(std::move(conditional)), operatorType(operatorType), consequent(std::move(consequent)), alternate(std::move(alternate)) {}
        std::string getType() const override {
          return "IfStatement";
        }
    };

    class Function : public ExprAST {
      public:
        std::vector<std::string> params;
        std::string name;
        std::vector<std::unique_ptr<AST::ExprAST>> body;

        Function(std::vector<std::string> params, std::string name, std::vector<std::unique_ptr<AST::ExprAST>> body)
            : params(params), name(name), body(std::move(body)) {}
        std::string getType() const override {
          return "Function";
        }
    };

    class Array : public ExprAST {
      public:
        std::vector<std::unique_ptr<ExprAST>> elements;

        Array(std::vector<std::unique_ptr<ExprAST>> elements) : elements(std::move(elements)) {}

        std::string getType() const override {
          return "Array";
        }
    };

    class ShellCMD : public ExprAST {
      public:
        std::string cmd;

        ShellCMD(std::string cmd) : cmd(cmd) {}

        std::string getType() const override {
          return "ShellCMD";
        }
    };

    class Element : public ExprAST {
      public:
        std::string key;
        std::unique_ptr<AST::ExprAST> value;

        Element(std::string key, std::unique_ptr<AST::ExprAST> value) : key(key), value(std::move(value)) {}

        std::string getType() const override {
          return "Element";
        }
    };

    class Object : public ExprAST {
      public:
        std::vector<std::unique_ptr<AST::ExprAST>> map;

        Object(std::vector<std::unique_ptr<AST::ExprAST>> map) : map(std::move(map)) {}

        std::string getType() const override {
          return "Object";
        }
    };

    class MemberExpr : public ExprAST {
      public:
        std::unique_ptr<AST::ExprAST> object;
        std::unique_ptr<AST::ExprAST> property;
        bool computed;

        MemberExpr(std::unique_ptr<AST::ExprAST> object, std::unique_ptr<AST::ExprAST> property, bool computed) : object(std::move(object)), property(std::move(property)), computed(computed) {}

        std::string getType() const override {
          return "MemberExpr";
        }
    };
}
