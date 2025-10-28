#include "ProgramVariables.h"
#include "IdentifierValidation.h"
#include <stdexcept>
#include <iostream>
#include <limits.h>
#include "ExpressionEvaluater.h"

void ProgramVariables::insertArray(const std::string& arr_ident, const size_t& size, std::unordered_map<std::string, int>& scope) {

	if (scope.count(arr_ident))
		throw std::runtime_error("This identifier already exists");

	IdentifierValidation idn(arr_ident);
	if (!idn.isValidIdent())
		throw std::invalid_argument("Invalid identifier name");

	for (size_t i = 0; i < size; ++i) {
		scope.insert({ arr_ident + "[" + std::to_string(i) + "]", INT_MAX });
	}
}


void ProgramVariables::insertVariable(std::istringstream& in, std::unordered_map<std::string, int>& scope) {
	std::string let_form, ident;
	in >> let_form >> ident;
	if (let_form != "LET")
		throw std::invalid_argument("Wrong variable declaration"); // Ensuring that the form is correct

	if (scope.count(ident))
		throw std::runtime_error("Variable " + ident + " already exists");

	IdentifierValidation idn(ident);
	if (!idn.isValidIdent())
		throw std::invalid_argument("Invalid identifier name");
	
	scope.insert({ ident, INT_MAX }); // giving initial garbage value during declaration
}

void ProgramVariables::PrintValue(std::istringstream& in, const std::unordered_map<std::string, int>& scope, std::ostream& out)const {
	std::string print_form, var_name;
	in >> print_form >> var_name;
	if (print_form != "PRINT")
		throw std::invalid_argument("Invalid print syntax");

	if (!scope.count(var_name))
		throw std::runtime_error("Variable " + var_name + " not found");

	out << scope.at(var_name);
}

void ProgramVariables::AssignValue(std::istringstream& in, std::unordered_map<std::string, int>& scope) {
	std::string var_name, opr, expression;
	ExpressionEvaluater evaluater;
	int value = INT_MAX; // initial value before evaluating the expression
	in >> var_name >> opr;
	if (!scope.count(var_name))
		throw std::runtime_error("Variable " + var_name + " not found");

	if (opr != "=")
		throw std::invalid_argument("Inappropriate operator use. Operator for assigning is '=' ");

	std::getline(in,expression); // Using the same stream so that we can read the RHS
	value = evaluater.evaluate(expression, scope);
	scope.at(var_name) = value;
}

void ProgramVariables::ReadValue(std::istringstream& input, std::unordered_map<std::string, int>& scope, std::istream& in) {
	std::string read_form, var_name;
	int value;
	input >> read_form >> var_name;
	if (read_form != "READ")
		throw std::invalid_argument("Unable to read value");

	if (!scope.count(var_name))
		throw std::runtime_error("Variable " + var_name + " not found");

	in >> value;
	scope.at(var_name) = value;
}
