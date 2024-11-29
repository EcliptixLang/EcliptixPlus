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
      virtual std::shared_ptr<ExprAST> clone() const = 0;
  };

  class NumberExpr : public ExprAST {
    public:
      double Value;
      NumberExpr(double Value) : Value(Value) {}
      Nodes getType() const override {
        return Nodes::Number;
      }

      std::shared_ptr<ExprAST> clone() const override {
        return std::make_shared<NumberExpr>(NumberExpr(Value));
      }
  };

    class StringExpr : public ExprAST {
    public:
      std::string Value;
      StringExpr(std::string Value) : Value(Value) {}
      Nodes getType() const override {
        return Nodes::String;
      }
      std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<StringExpr>(StringExpr(Value));
      }
    };

    class Program : public ExprAST {
    public:
      std::vector<std::shared_ptr<AST::ExprAST>> body;
      Program(std::vector<std::shared_ptr<AST::ExprAST>> body) : body(body) {}

      Nodes getType() const override {
        return Nodes::Program;
      }

      std::shared_ptr<ExprAST> clone() const override {
          std::vector<std::shared_ptr<ExprAST>> clonedbody;
          for(auto& thing : body){
            clonedbody.push_back(thing.get()->clone());
          }
          return std::make_shared<Program>(Program(clonedbody));
      }
    };

    class Identifier : public ExprAST {
      public:
        std::string name;

        Identifier(std::string name) : name(name) {}

        Nodes getType() const override {
          return Nodes::Identifier;
        }

        std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<Identifier>(Identifier(name));
        }
    };

    class VariableExpr : public ExprAST {
    public:
      std::string Name;
      std::string Type;
      std::shared_ptr<ExprAST> Value;
      bool constant;
      VariableExpr(const std::string &Name, const std::string &Type, std::shared_ptr<ExprAST> Value, bool constant)
       : Name(Name), Type(Type), Value(Value), constant(constant) {}
      
      Nodes getType() const override {
        return Nodes::Variable;
      }

      std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<VariableExpr>(VariableExpr(Name, Type, Value.get()->clone(), constant));
        }
    };

    class AssignmentExpr : public ExprAST {
      public:
        std::shared_ptr<AST::ExprAST> assignee;
        std::shared_ptr<AST::ExprAST> value;

        AssignmentExpr(std::shared_ptr<AST::ExprAST> assignee, std::shared_ptr<AST::ExprAST> value) : assignee(assignee), value(value) {}

        Nodes getType() const override {
          return Nodes::Assignment;
        }

        std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<AssignmentExpr>(AssignmentExpr(assignee.get()->clone(), value.get()->clone()));
        }
    };

    class BinaryExpr : public ExprAST {
    public:
      char Op;
      std::shared_ptr<ExprAST> LHS, RHS;
      BinaryExpr(char Op, std::shared_ptr<ExprAST> LHS,
                    std::shared_ptr<ExprAST> RHS)
          : Op(Op), LHS(LHS), RHS(RHS) {}
      Nodes getType() const override {
        return Nodes::Binary;
      }

      std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<BinaryExpr>(BinaryExpr(Op, LHS.get()->clone(), RHS.get()->clone()));
        }
    };

    class CallExpr : public ExprAST {
    public:
      std::shared_ptr<ExprAST> Callee;
      std::vector<std::shared_ptr<ExprAST>> Args;

      CallExpr(std::shared_ptr<ExprAST> Callee,
                  std::vector<std::shared_ptr<ExprAST>> Args)
          : Callee(Callee), Args(Args) {}
      Nodes getType() const override {
        return Nodes::Call;
      }

      std::shared_ptr<ExprAST> clone() const override {
          std::vector<std::shared_ptr<ExprAST>> clonedbody;
          for(auto& thing : Args){
            clonedbody.push_back(thing.get()->clone());
          }
          return std::make_shared<CallExpr>(CallExpr(Callee.get()->clone(), clonedbody));
        }
    };

    class WhenDeclaration : public ExprAST {
      public:
        std::shared_ptr<ExprAST> conditional;
        Lexer::TokenType operatorType;
        std::vector<std::shared_ptr<ExprAST>> consequent;

        WhenDeclaration(std::shared_ptr<ExprAST> conditional, Lexer::TokenType operatorType, std::vector<std::shared_ptr<ExprAST>> consequent)
            : conditional(conditional), operatorType(operatorType), consequent(consequent) {}
        Nodes getType() const override {
          return Nodes::When;
        }

        std::shared_ptr<ExprAST> clone() const override {
          std::vector<std::shared_ptr<ExprAST>> clonedbody;
          for(auto& thing : consequent){
            clonedbody.push_back(thing.get()->clone());
          }
          return std::make_shared<WhenDeclaration>(conditional.get()->clone(), operatorType, clonedbody);
        }
    };

    class WhileDeclaration : public ExprAST {
      public:
        std::shared_ptr<ExprAST> conditional;
        Lexer::TokenType operatorType;
        std::vector<std::shared_ptr<ExprAST>> consequent;

        WhileDeclaration(std::shared_ptr<ExprAST> conditional, Lexer::TokenType operatorType, std::vector<std::shared_ptr<ExprAST>> consequent)
            : conditional(conditional), operatorType(operatorType), consequent(consequent) {}
        Nodes getType() const override {
          return Nodes::While;
        }

        std::shared_ptr<ExprAST> clone() const override {
          std::vector<std::shared_ptr<ExprAST>> clonedbody;
          for(auto& thing : consequent){
            clonedbody.push_back(thing.get()->clone());
          }
          return std::make_shared<WhileDeclaration>(conditional.get()->clone(), operatorType, clonedbody);
        }
    };

    class IfStatement : public ExprAST {
      public:
        std::shared_ptr<ExprAST> conditional;
        Lexer::TokenType operatorType;
        std::vector<std::shared_ptr<ExprAST>> consequent;
        std::vector<std::shared_ptr<ExprAST>> alternate;

        IfStatement(std::shared_ptr<ExprAST> conditional, Lexer::TokenType operatorType, std::vector<std::shared_ptr<ExprAST>> consequent, std::vector<std::shared_ptr<ExprAST>> alternate)
            : conditional(conditional), operatorType(operatorType), consequent(consequent), alternate(alternate) {}
        Nodes getType() const override {
          return Nodes::If;
        }

        std::shared_ptr<ExprAST> clone() const override {
          std::vector<std::shared_ptr<ExprAST>> clonedbody;
          for(auto& thing : consequent){
            clonedbody.push_back(thing.get()->clone());
          }
          std::vector<std::shared_ptr<ExprAST>> clonedbody1;
          for(auto& thing : alternate){
            clonedbody1.push_back(thing.get()->clone());
          }
          return std::make_shared<IfStatement>(conditional.get()->clone(), operatorType, clonedbody, clonedbody1);
        }
    };

    class Function : public ExprAST {
      public:
        std::vector<std::string> params;
        std::string name;
        std::string type;
        std::vector<std::shared_ptr<AST::ExprAST>> body;

        Function(std::vector<std::string> params, std::string name, std::vector<std::shared_ptr<AST::ExprAST>> body, std::string type)
            : params(params), name(name), body(body), type(type) {}
        Nodes getType() const override {
          return Nodes::Function;
        }

        std::shared_ptr<ExprAST> clone() const override {
          std::vector<std::shared_ptr<ExprAST>> clonedbody;
          for(auto& thing : body){
            clonedbody.push_back(thing.get()->clone());
          }
          return std::make_shared<Function>(Function(params, name, clonedbody, type));
        }
    };

    class Array : public ExprAST {
      public:
        std::vector<std::shared_ptr<ExprAST>> elements;

        Array(std::vector<std::shared_ptr<ExprAST>> elements) : elements(elements) {}

        Nodes getType() const override {
          return Nodes::Array;
        }

        std::shared_ptr<ExprAST> clone() const override {
          std::vector<std::shared_ptr<ExprAST>> clonedelm;
          for(auto& thing : elements){
            clonedelm.push_back(thing.get()->clone());
          }
          return std::make_shared<Array>(Array(clonedelm));
        }
    };

    class ShellCMD : public ExprAST {
      public:
        std::string cmd;

        ShellCMD(std::string cmd) : cmd(cmd) {}

        Nodes getType() const override {
          return Nodes::ShellCmd;
        }

        std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<ShellCMD>(ShellCMD(cmd));
        }
    };

    class Element : public ExprAST {
      public:
        std::string key;
        std::shared_ptr<AST::ExprAST> value;

        Element(std::string key, std::shared_ptr<AST::ExprAST> value) : key(key), value(value) {}

        Nodes getType() const override {
          return Nodes::Element;
        }

        std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<Element>(Element(key, value.get()->clone()));
        }
    };

    class Object : public ExprAST {
      public:
        std::vector<std::shared_ptr<AST::ExprAST>> map;

        Object(std::vector<std::shared_ptr<AST::ExprAST>> map) : map(map) {}

        Nodes getType() const override {
          return Nodes::Object;
        }

        std::shared_ptr<ExprAST> clone() const override {
          std::vector<std::shared_ptr<AST::ExprAST>> clonedmap;
          for(auto& thing : map){
            clonedmap.push_back(thing.get()->clone());
          }
          return std::make_shared<Object>(Object(clonedmap));
        }
    };

    class MemberExpr : public ExprAST {
      public:
        std::shared_ptr<AST::ExprAST> object;
        std::shared_ptr<AST::ExprAST> property;
        bool computed;

        MemberExpr(std::shared_ptr<AST::ExprAST> object, std::shared_ptr<AST::ExprAST> property, bool computed) 
        : object(object), property(property), computed(computed) {}

        Nodes getType() const override {
          return Nodes::Member;
        }

        std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<MemberExpr>(MemberExpr(object.get()->clone(), property.get()->clone(), computed));
        }
    };

    class ReturnExpr : public ExprAST {
      public:
        std::shared_ptr<AST::ExprAST> value;

        ReturnExpr(std::shared_ptr<AST::ExprAST> value) : value(value) {}

        Nodes getType() const override {
          return Nodes::Return;
        }

        std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<ReturnExpr>(ReturnExpr(value.get()->clone()));
        }
    };

    class EquExpr : public ExprAST {
      public:
        std::shared_ptr<AST::ExprAST> left;
        std::shared_ptr<AST::ExprAST> right;
        Lexer::Token oper;

        EquExpr(std::shared_ptr<AST::ExprAST> left, std::shared_ptr<AST::ExprAST> right, Lexer::Token oper) 
        : left(left), right(right), oper(oper) {}

        Nodes getType() const override {
          return Nodes::Equality;
        }

        std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<EquExpr>(EquExpr(left.get()->clone(), right.get()->clone(), oper));
        }
    };

    class Break : public ExprAST {
      public:
        Nodes getType() const override {
          return Nodes::Break;
        }
        std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<Break>(Break());
        }
    };

    class Skip : public ExprAST {
      public:
        Nodes getType() const override {
          return Nodes::Skip;
        }
        std::shared_ptr<ExprAST> clone() const override {
          return std::make_shared<Skip>(Skip());
        }
    };
}
