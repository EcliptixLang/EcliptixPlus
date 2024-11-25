#pragma once
#include "Values.hpp"
#include "AST.hpp"
#include <ENV.hpp>
#include <memory>

class Interpreter {
    public:
    std::unique_ptr<Values::Runtime> IProgram(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> INumber(std::unique_ptr<AST::ExprAST>& astNode);
    std::unique_ptr<Values::Runtime> ICall(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IVariable(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IString(std::unique_ptr<AST::ExprAST>& astNode);
    std::unique_ptr<Values::Runtime> IIdent(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IBinary(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IObject(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IArray(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IMember(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IFunction(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IAssignment(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IWhile(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IWhen(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IIf(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> IEqu(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
    std::unique_ptr<Values::Runtime> evaluate(std::unique_ptr<AST::ExprAST>& astNode, Environment& env);
};