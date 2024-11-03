#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <map>
#include <Lexer.hpp>

namespace AST {

  enum class Nodes {
  	Number,
  	Program,
    String,
    Identifier,
    Variable,
    Assignment,
    Binary,
    Call,
    When,
    While,
    If,
    Function,
    Array,
    ShellCmd,
    Element, 
    Object, 
    Member, 
    Equality,
    Return
  };

  std::string stringifyAST(Nodes type);

  class ExprAST {
    public:
      virtual ~ExprAST() = default;
      virtual Nodes getType() const = 0;
  };

  class NumberExpr : public ExprAST {
    public:
      double Value;
      NumberExpr(double Value) : Value(Value) {}
      Nodes getType() const override {
        return Nodes::Number;
      }
  };

    class StringExpr : public ExprAST {
    public:
      std::string Value;
      StringExpr(std::string Value) : Value(Value) {}
      Nodes getType() const override {
        return Nodes::String;
      }
    };

    class Program : public ExprAST {
    public:
      std::vector<std::unique_ptr<AST::ExprAST>> body;
      Program(std::vector<std::unique_ptr<AST::ExprAST>> body) : body(std::move(body)) {}

      Nodes getType() const override {
        return Nodes::Program;
      }
    };

    class Identifier : public ExprAST {
      public:
        std::string name;

        Identifier(std::string name) : name(name) {}

        Nodes getType() const override {
          return Nodes::Identifier;
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
      
      Nodes getType() const override {
        return Nodes::Variable;
      }
    };

    class AssignmentExpr : public ExprAST {
      public:
        std::unique_ptr<AST::ExprAST> assignee;
        std::unique_ptr<AST::ExprAST> value;

        AssignmentExpr(std::unique_ptr<AST::ExprAST> assignee, std::unique_ptr<AST::ExprAST> value) : assignee(std::move(assignee)), value(std::move(value)) {}

        Nodes getType() const override {
          return Nodes::Assignment;
        }
    };

    class BinaryExpr : public ExprAST {
    public:
      char Op;
      std::unique_ptr<ExprAST> LHS, RHS;
      BinaryExpr(char Op, std::unique_ptr<ExprAST> LHS,
                    std::unique_ptr<ExprAST> RHS)
          : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
      Nodes getType() const override {
        return Nodes::Binary;
      }
    };

    class CallExpr : public ExprAST {
    public:
      std::unique_ptr<ExprAST> Callee;
      std::vector<std::unique_ptr<ExprAST>> Args;

      CallExpr(std::unique_ptr<ExprAST> &Callee,
                  std::vector<std::unique_ptr<ExprAST>> Args)
          : Callee(std::move(Callee)), Args(std::move(Args)) {}
      Nodes getType() const override {
        return Nodes::Call;
      }
    };

    class WhenDeclaration : public ExprAST {
      public:
        std::unique_ptr<ExprAST> conditional;
        Lexer::TokenType operatorType;
        std::vector<std::unique_ptr<ExprAST>> consequent;

        WhenDeclaration(std::unique_ptr<ExprAST> conditional, Lexer::TokenType operatorType, std::vector<std::unique_ptr<ExprAST>> consequent)
            : conditional(std::move(conditional)), operatorType(operatorType), consequent(std::move(consequent)) {}
        Nodes getType() const override {
          return Nodes::When;
        }
    };

    class WhileDeclaration : public ExprAST {
      public:
        std::unique_ptr<ExprAST> conditional;
        Lexer::TokenType operatorType;
        std::vector<std::unique_ptr<ExprAST>> consequent;

        WhileDeclaration(std::unique_ptr<ExprAST> conditional, Lexer::TokenType operatorType, std::vector<std::unique_ptr<ExprAST>> consequent)
            : conditional(std::move(conditional)), operatorType(operatorType), consequent(std::move(consequent)) {}
        Nodes getType() const override {
          return Nodes::While;
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
        Nodes getType() const override {
          return Nodes::If;
        }
    };

    class Function : public ExprAST {
      public:
        std::vector<std::string> params;
        std::string name;
        std::string type;
        std::vector<std::unique_ptr<AST::ExprAST>> body;

        Function(std::vector<std::string> params, std::string name, std::vector<std::unique_ptr<AST::ExprAST>> body, std::string type)
            : params(params), name(name), body(std::move(body)), type(type) {}
        Nodes getType() const override {
          return Nodes::Function;
        }
    };

    class Array : public ExprAST {
      public:
        std::vector<std::unique_ptr<ExprAST>> elements;

        Array(std::vector<std::unique_ptr<ExprAST>> elements) : elements(std::move(elements)) {}

        Nodes getType() const override {
          return Nodes::Array;
        }
    };

    class ShellCMD : public ExprAST {
      public:
        std::string cmd;

        ShellCMD(std::string cmd) : cmd(cmd) {}

        Nodes getType() const override {
          return Nodes::ShellCmd;
        }
    };

    class Element : public ExprAST {
      public:
        std::string key;
        std::unique_ptr<AST::ExprAST> value;

        Element(std::string key, std::unique_ptr<AST::ExprAST> value) : key(key), value(std::move(value)) {}

        Nodes getType() const override {
          return Nodes::Element;
        }
    };

    class Object : public ExprAST {
      public:
        std::vector<std::unique_ptr<AST::ExprAST>> map;

        Object(std::vector<std::unique_ptr<AST::ExprAST>> map) : map(std::move(map)) {}

        Nodes getType() const override {
          return Nodes::Object;
        }
    };

    class MemberExpr : public ExprAST {
      public:
        std::unique_ptr<AST::ExprAST> object;
        std::unique_ptr<AST::ExprAST> property;
        bool computed;

        MemberExpr(std::unique_ptr<AST::ExprAST> object, std::unique_ptr<AST::ExprAST> property, bool computed) : object(std::move(object)), property(std::move(property)), computed(computed) {}

        Nodes getType() const override {
          return Nodes::Member;
        }
    };

    class ReturnExpr : public ExprAST {
      public:
        std::unique_ptr<AST::ExprAST> value;

        ReturnExpr(std::unique_ptr<AST::ExprAST> value) : value(std::move(value)) {}

        Nodes getType() const override {
          return Nodes::Return;
        }
    };

    class EquExpr : public ExprAST {
      public:
        std::unique_ptr<AST::ExprAST> left;
        std::unique_ptr<AST::ExprAST> right;
        Lexer::TokenType oper;

        EquExpr(std::unique_ptr<AST::ExprAST> left, std::unique_ptr<AST::ExprAST> right, Lexer::TokenType oper) : left(std::move(left)), right(std::move(right)), oper(oper) {}

        Nodes getType() const override {
          return Nodes::Equality;
        }
    };
}
