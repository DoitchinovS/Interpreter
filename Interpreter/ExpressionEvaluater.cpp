#include "ExpressionEvaluater.h"
#include <stdexcept>
#include <stack>
#include <sstream>

bool ExpressionEvaluater::isOperator(const std::string& oper) {
    return (applyableOperators.count(oper));
}

bool ExpressionEvaluater::isVariable(const std::string& var, std::unordered_map<std::string, int>& exprVars) {
    return (exprVars.count(var));
}

bool ExpressionEvaluater::isLiteral( std::string& num) {
    size_t index = 0;
    if (!isdigit(num[0]) && num[0] != '-' && num[0] != '0')
        return false;

        while (index < num.size()) {
            if (!isdigit(num[index]))
                return false;
            ++index;
        }
    
    return true;
}

const std::string ExpressionEvaluater::ParseNegativeNumbers(const std::string& expr) { //TO be examined in ExpressionEvaluationTest
    std::string result;
    std::string curr;
    std::istringstream operand(expr);
    while (operand>> curr) {

        if (curr[0] == '-' && curr.size() > 1) {

         result.append("( 0 - " + curr.substr(1) + " )");

        }
        else {
            result.append(curr);
        }
        result.append(" ");
    }
    return result;
}


//const std::string ExpressionEvaluater::ParseExpresionToInfix(const std::string& expr) {
//    std::string result = "\0";
//    std::string multipleDigitNum = "\0";
//    for (char c : expr) {
//        if (!isdigit(c)) {
//            if (multipleDigitNum != "\0") {
//                result.append(multipleDigitNum);
//                multipleDigitNum = "\0";
//                result.append(" ");
//            }
//            result.append(1, c);
//            result.append(" ");
//        }
//        else {
//            multipleDigitNum.append(1,c);
//        }
//        
//    }
//
//    result.append(multipleDigitNum);
//    return result;
//}



const int ExpressionEvaluater::getOperationPriority(const std::string& oper) {
	if (oper == "||") return 1; // if-else if chain. because switch cannot be used for std::string type
	else if (oper == "&&") return 2;
	else if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" || oper == ">" || oper == ">=") return 3;
	else if (oper == "+" || oper == "-") return 4;
	else if (oper == "*" || oper == "/" || oper == "%") return 5;
	else if (oper == "!") return 6;
	else return 0; 
}

int ExpressionEvaluater::applyOperator(int left_arg, int right_arg, const std::string& oper) {
    if (oper == "+") return left_arg + right_arg;
    if (oper == "-") return left_arg - right_arg;
    if (oper == "*") return left_arg * right_arg;
    if (oper == "/") {
        if (right_arg == 0) throw std::logic_error("Division by zero");
        return left_arg / right_arg;
    }
    if (oper == "%") {
        if (right_arg == 0) throw std::logic_error("Modulo by zero");
        return left_arg % right_arg;
    }
    if (oper == "==") return left_arg == right_arg;
    if (oper == "!=") return left_arg != right_arg;
    if (oper == "<") return left_arg < right_arg;
    if (oper == "<=") return left_arg <= right_arg;
    if (oper == ">") return left_arg > right_arg;
    if (oper == ">=") return left_arg >= right_arg;
    if (oper == "&&") return left_arg && right_arg;
    if (oper == "||") return left_arg || right_arg;
    throw std::invalid_argument("Cannot recognise operator " + oper);
}



std::vector<std::string> ExpressionEvaluater::convertPostfix(const std::string& expr, std::unordered_map<std::string, int>& exprVars) {
   
    std::string parsedNegative = this->ParseNegativeNumbers(expr);

    std::stack<std::string> operations;
    std::vector<std::string> output;
    std::istringstream converter(parsedNegative);
    std::string currentArg;
    while (converter >> currentArg) {

        if (this->isLiteral(currentArg))
            output.push_back(currentArg);

        else if (this->isVariable(currentArg, exprVars)) // identifier correctness is already ensured when creating a variable
            output.push_back(currentArg);

        else if (currentArg == "(")
            operations.push(currentArg); // brackets are special operators and thus have a different else-if statement

        else if (currentArg == ")") {
            while (!operations.empty() && operations.top() != "(") {
                output.push_back(operations.top());
                operations.pop(); // listing all operations that are above the '(', because they are to be executed first
            }
            if (operations.empty())
                throw std::logic_error("Wrong expression. Missing '('.");

            operations.pop();//removing the '('
        }
        else if (this->isOperator(currentArg)) {
            while (!operations.empty() && getOperationPriority(operations.top()) >= getOperationPriority(currentArg)
                && operations.top() != "(") { // bracket case is important, since it indicates priority for the current operation
                output.push_back(operations.top());
                operations.pop();
            } // All operators added to the string have higher priority, thus have to be executed prior to applying the current opr.
            operations.push(currentArg);
        }
        else {
            throw std::logic_error("Wrong expression. Cannot recognise " + currentArg);
        }
    }

    while (!operations.empty()) {
        if (operations.top() == "(") {
            throw std::logic_error("Wrong expression. Missing ')'");
        }
        output.push_back(operations.top());
        operations.pop();
    }
    return output;
}

int ExpressionEvaluater::EvalPostfixExpr(std::vector<std::string> postfixexpr, std::unordered_map<std::string, int>& exprVars) {
    std::stack<int> operands;
    for (std::string currentArg : postfixexpr) {
        if (this->isLiteral(currentArg))
            operands.push(std::stoi(currentArg));
        else if (this->isVariable(currentArg, exprVars))
            operands.push(exprVars.at(currentArg));

        else { // it is an operator
            if (currentArg == "!") { // ! is the only unary operator so it has a separate if statement
                if (operands.empty())
                    throw std::logic_error("Wrong expression. Insufficient number of operands.");

                int opr = operands.top();
                operands.pop();
                operands.push(!opr); // executing unary operation
            } 
            else {
                if (operands.size() < 2) {
                    throw std::logic_error("Wrong expression. Insufficient number of operands.");
                }
                int rightoper = operands.top();
                operands.pop();
                int leftoper = operands.top();
                operands.pop();
                operands.push(this->applyOperator(leftoper, rightoper, currentArg)); // Reversed order due to LIFO of the stack
            }

        }

    } //No need for expression validation since it has already been made during the Postfix convertion

    if (operands.size() != 1) // At the end we should have only one value in the stack
        throw std::logic_error("Wrong expression. Incorrect number of parameters");

    return operands.top();
}

int ExpressionEvaluater::evaluate (const std::string& expr,  std::unordered_map<std::string, int>& exprVariables) {
	std::vector<std::string> postfixExpr = convertPostfix(expr, exprVariables);
	return EvalPostfixExpr(postfixExpr, exprVariables);
}
