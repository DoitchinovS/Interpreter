#pragma once

#ifndef PROGRAM_INTERPRETER_HEADER

#define PROGRAM_INTERPRETER_HEADER
#include "ProgramLabels.h"
#include "ProgramVariables.h"
#include <stack>
#include "ExpressionEvaluater.h"
#include <queue>
#include <list>

struct IntPair {
	int first;
	int second;
	bool operator== (const IntPair& other)const;
	IntPair(const int& fr = 0, const int& scd = 0): first(fr), second(scd){}
};

class ProgramInterpreter {
private:
	ProgramLabels listedLabels;
	ProgramVariables variableManager;
	ExpressionEvaluater expressionManager;

	std::vector<std::string> program;
	size_t currentRow = 0;
	std::stack<size_t> whileStack; // a stack for keeping track of nested whiles

	
	std::stack<bool> ifStack; // keeping track of nested if-statements
	int else_bracket_balance = 0;

	void loadLabels();

	std::stack<std::unordered_map<std::string, int>> scopes;
	void enterScope();
	void exitScope();
	std::unordered_map<std::string, int>& currentScope();

	void executeCommand(const std::string& command);

	void terminateWhile(); // used to track where the last while is and continue from its done form

	void endIfstatement();
	void skipToelse();

	std::list <IntPair> Bind_IF_ENDIF(const std::vector<std::string>& program);

	 std::list<IntPair> Bind_While_Done(const std::vector<std::string>& program);

	bool ELSECorrectSyntax (const std::vector<std::string>& program);
	bool StatementsCorrectSyntax(const std::vector<std::string>& program, const std::string& command1, const std::string& command2);
	//To be used for while-done, if-endif, [else-]

public:
	ProgramInterpreter();
	~ProgramInterpreter() = default;


	void runFromFile(const std::string& filename);
	void runOnREPL();
};
#endif // !PROGRAM_INTERPRETER_HEADER
