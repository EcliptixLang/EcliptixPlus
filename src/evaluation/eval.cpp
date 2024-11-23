#include <Interpreter.hpp>

using Nodes = AST::Nodes; 
using string = std::string;

void runCommand(const string& command) {
	system(command.c_str());
}

std::unique_ptr<Values::Runtime> Interpreter::evaluate(std::unique_ptr<AST::ExprAST>& astNode, Environment& env){

		AST::Nodes type = astNode->getType();
		switch (type){
			case Nodes::Program:
				return IProgram(astNode, env);
			case Nodes::Call:
				return ICall(astNode, env);
			case Nodes::String:
				return IString(astNode);
			case Nodes::Identifier:
				return IIdent(astNode, env);
			case Nodes::Number:
				return INumber(astNode);
			case Nodes::Variable:
				return IVariable(astNode, env);
			case Nodes::Binary:
				return IBinary(astNode, env);
			case Nodes::Array:
				return IArray(astNode, env);
			case Nodes::Member:
				return IMember(astNode, env);
			case Nodes::If:
				return IIf(astNode, env);
			case Nodes::Equality:
				return IEqu(astNode, env);
			case Nodes::Skip:
				return std::make_unique<Values::Skip>(Values::Skip());
			case Nodes::ShellCmd:{
				AST::ShellCMD* cmd = dynamic_cast<AST::ShellCMD*>(astNode.get());
				runCommand(cmd->cmd);
				return std::make_unique<Values::Null>(Values::Null());
			} break;
			case Nodes::Break:
				return std::make_unique<Values::Break>(Values::Break());
			case Nodes::Function:
				return IFunction(astNode, env);
			case Nodes::Assignment:
				return IAssignment(astNode, env);
			break;
			case Nodes::While:
				return IWhile(astNode, env);
			case Nodes::Object:
				return IObject(astNode, env);
			break;
			case Nodes::Return: {
				AST::ReturnExpr* number = dynamic_cast<AST::ReturnExpr*>(astNode.get());
				return std::make_unique<Values::ReturnedValue>(Values::ReturnedValue(std::move(Interpreter::evaluate(number->value, env))));
			} break;
			default:
				std::cerr 
					<< "This AST Node is not yet set up for interpretation.\n" 
					<< "- Type: " << AST::stringifyAST(type) << "\n";
				exit(1);
		}
}