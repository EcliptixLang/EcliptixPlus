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
    Skip,
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
    Break,
    Return
  };

  std::string stringifyAST(Nodes type);

  class ExprAST {
    public:
      virtual ~ExprAST() = default;
      virtual Nodes getType() const = 0;
      virtual std::unique_ptr<ExprAST> clone() const = 0;
  };

  class NumberExpr : public ExprAST {
    public:
      double Value;
      NumberExpr(double Value) : Value(Value) {}
      Nodes getType() const override {
        return Nodes::Number;
      }

      std::unique_ptr<ExprAST> clone() const override {
        return std::make_unique<NumberExpr>(NumberExpr(Value));
      }
  };

    class StringExpr : public ExprAST {
    public:
      std::string Value;
      StringExpr(std::string Value) : Value(Value) {}
      Nodes getType() const override {
        return Nodes::String;
      }
      std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<StringExpr>(StringExpr(Value));
      }
    };

    class Program : public ExprAST {
    public:
      std::vector<std::unique_ptr<AST::ExprAST>> body;
      Program(std::vector<std::unique_ptr<AST::ExprAST>> body) : body(std::move(body)) {}

      Nodes getType() const override {
        return Nodes::Program;
      }

      std::unique_ptr<ExprAST> clone() const override {
          std::vector<std::unique_ptr<ExprAST>> clonedbody;
          for(auto& thing : body){
            clonedbody.push_back(thing.get()->clone());
          }
          return std::make_unique<Program>(Program(std::move(clonedbody)));
      }
    };

    class Identifier : public ExprAST {
      public:
        std::string name;

        Identifier(std::string name) : name(name) {}

        Nodes getType() const override {
          return Nodes::Identifier;
        }

        std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<Identifier>(Identifier(name));
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

      std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<VariableExpr>(VariableExpr(Name, Type, Value.get()->clone(), constant));
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

        std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<AssignmentExpr>(AssignmentExpr(assignee.get()->clone(), value.get()->clone()));
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

      std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<BinaryExpr>(BinaryExpr(Op, LHS.get()->clone(), RHS.get()->clone()));
        }
    };

    class CallExpr : public ExprAST {
    public:
      std::unique_ptr<ExprAST> Callee;
      std::vector<std::unique_ptr<ExprAST>> Args;

      CallExpr(std::unique_ptr<ExprAST> Callee,
                  std::vector<std::unique_ptr<ExprAST>> Args)
          : Callee(std::move(Callee)), Args(std::move(Args)) {}
      Nodes getType() const override {
        return Nodes::Call;
      }

      std::unique_ptr<ExprAST> clone() const override {
          std::vector<std::unique_ptr<ExprAST>> clonedbody;
          for(auto& thing : Args){
            clonedbody.push_back(thing.get()->clone());
          }
          return std::make_unique<CallExpr>(CallExpr(Callee.get()->clone(), std::move(clonedbody)));
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

        std::unique_ptr<ExprAST> clone() const override {
          std::vector<std::unique_ptr<ExprAST>> clonedbody;
          for(auto& thing : consequent){
            clonedbody.push_back(thing.get()->clone());
          }
          return std::make_unique<WhenDeclaration>(WhenDeclaration(conditional.get()->clone(), operatorType, std::move(clonedbody)));
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

        std::unique_ptr<ExprAST> clone() const override {
          std::vector<std::unique_ptr<ExprAST>> clonedbody;
          for(auto& thing : consequent){
            clonedbody.push_back(thing.get()->clone());
          }
          return std::make_unique<WhileDeclaration>(WhileDeclaration(conditional.get()->clone(), operatorType, std::move(clonedbody)));
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

        std::unique_ptr<ExprAST> clone() const override {
          std::vector<std::unique_ptr<ExprAST>> clonedbody;
          for(auto& thing : consequent){
            clonedbody.push_back(thing.get()->clone());
          }
          std::vector<std::unique_ptr<ExprAST>> clonedbody1;
          for(auto& thing : alternate){
            clonedbody1.push_back(thing.get()->clone());
          }
          return std::make_unique<IfStatement>(IfStatement(std::move(conditional.get()->clone()), operatorType, std::move(clonedbody), std::move(clonedbody1)));
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

        std::unique_ptr<ExprAST> clone() const override {
          std::vector<std::unique_ptr<ExprAST>> clonedbody;
          for(auto& thing : body){
            clonedbody.push_back(thing.get()->clone());
          }
          return std::make_unique<Function>(Function(params, name, std::move(clonedbody), type));
        }
    };

    class Array : public ExprAST {
      public:
        std::vector<std::unique_ptr<ExprAST>> elements;

        Array(std::vector<std::unique_ptr<ExprAST>> elements) : elements(std::move(elements)) {}

        Nodes getType() const override {
          return Nodes::Array;
        }

        std::unique_ptr<ExprAST> clone() const override {
          std::vector<std::unique_ptr<ExprAST>> clonedelm;
          for(auto& thing : elements){
            clonedelm.push_back(thing.get()->clone());
          }
          return std::make_unique<Array>(Array(std::move(clonedelm)));
        }
    };

    class ShellCMD : public ExprAST {
      public:
        std::string cmd;

        ShellCMD(std::string cmd) : cmd(cmd) {}

        Nodes getType() const override {
          return Nodes::ShellCmd;
        }

        std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<ShellCMD>(ShellCMD(cmd));
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

        std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<Element>(Element(key, value.get()->clone()));
        }
    };

    class Object : public ExprAST {
      public:
        std::vector<std::unique_ptr<AST::ExprAST>> map;

        Object(std::vector<std::unique_ptr<AST::ExprAST>> map) : map(std::move(map)) {}

        Nodes getType() const override {
          return Nodes::Object;
        }

        std::unique_ptr<ExprAST> clone() const override {
          std::vector<std::unique_ptr<AST::ExprAST>> clonedmap;
          for(auto& thing : map){
            clonedmap.push_back(thing.get()->clone());
          }
          return std::make_unique<Object>(Object(std::move(clonedmap)));
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

        std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<MemberExpr>(MemberExpr(object.get()->clone(), property.get()->clone(), computed));
        }
    };

    class ReturnExpr : public ExprAST {
      public:
        std::unique_ptr<AST::ExprAST> value;

        ReturnExpr(std::unique_ptr<AST::ExprAST> value) : value(std::move(value)) {}

        Nodes getType() const override {
          return Nodes::Return;
        }

        std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<ReturnExpr>(ReturnExpr(value.get()->clone()));
        }
    };

    class EquExpr : public ExprAST {
      public:
        std::unique_ptr<AST::ExprAST> left;
        std::unique_ptr<AST::ExprAST> right;
        Lexer::Token oper;

        EquExpr(std::unique_ptr<AST::ExprAST> left, std::unique_ptr<AST::ExprAST> right, Lexer::Token oper) : left(std::move(left)), right(std::move(right)), oper(oper) {}

        Nodes getType() const override {
          return Nodes::Equality;
        }

        std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<EquExpr>(EquExpr(left.get()->clone(), right.get()->clone(), oper));
        }
    };

    class Break : public ExprAST {
      public:
        Nodes getType() const override {
          return Nodes::Break;
        }
        std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<Break>(Break());
        }
    };

    class Skip : public ExprAST {
      public:
        Nodes getType() const override {
          return Nodes::Skip;
        }
        std::unique_ptr<ExprAST> clone() const override {
          return std::make_unique<Skip>(Skip());
        }
    };
}
