#pragma once
#include "Values.hpp"
#include "AST.hpp"
#include <ENV.hpp>
#include <memory>

class Interpreter {
    public:
    std::shared_ptr<Values::Runtime> IProgram(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> INumber(std::shared_ptr<AST::ExprAST>& astNode);
    std::shared_ptr<Values::Runtime> ICall(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IVariable(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IString(std::shared_ptr<AST::ExprAST>& astNode);
    std::shared_ptr<Values::Runtime> IIdent(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IBinary(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IObject(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IArray(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IMember(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IFunction(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IAssignment(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IWhile(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IWhen(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IIf(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> IEqu(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
    std::shared_ptr<Values::Runtime> evaluate(std::shared_ptr<AST::ExprAST>& astNode, Environment& env);
};