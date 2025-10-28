#pragma once

#ifndef EXPRESSION_EVALUATER_HEADER

#define EXPRESSION_EVALUATER_HEADER

#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <cctype>

const std::unordered_set <std::string> applyableOperators = { "+", "-", "*", "/", "%", "==", "!=",
"<", "<=", ">", ">=", "&&", "||", "!", "=" }; //set of all operators that the Interpreter recognises

class ExpressionEvaluater {
private:
	const int getOperationPriority(const std::string& oper);
	int applyOperator(int left_arg, int right_arg, const std::string& oper);

	std::vector<std::string> convertPostfix(const std::string& expr, std::unordered_map<std::string, int>& exprVars);

	//const std::string ParseExpresionToInfix(const std::string& expr);

	const std::string ParseNegativeNumbers(const std::string& expr);


	int EvalPostfixExpr(std::vector<std::string> postfixexpr, std::unordered_map<std::string, int>& exprVars);

	bool isOperator(const std::string& oper);
	bool isVariable(const std::string& var, std::unordered_map<std::string, int>& exprVars);
	bool isLiteral(std::string& num);

public:
	int evaluate(const std::string& expr,  std::unordered_map<std::string, int>& exprVariables);

};



#endif // !EXPRESSION_EVALUATER_HEADER






