#include "ProgramInterpreter.h"
#include <fstream>
#include <iostream>


bool IntPair::operator == (const IntPair& other) const {
	return (this->first == other.first) && (this->second == other.second);
}

ProgramInterpreter::ProgramInterpreter() { this->enterScope(); }

std::list<IntPair> ProgramInterpreter::Bind_IF_ENDIF(const std::vector<std::string>& program) {
	std::list<IntPair> binder;
	std::stack<int> if_stack; // keeps track of the rows where there is if
	IntPair pair;
	std::string command;
	for (size_t i = 0; i < program.size(); ++i) {
		std::istringstream curr(program[i]);
		if (!(curr >> command))
			continue;

		if (command == "IF")
			if_stack.push(i);
		else if (command == "ENDIF") {
			pair.first = if_stack.top(); //Cannot cause underflow_error, because we have ensured correct if-endif syntax
			if_stack.pop();
			pair.second = i;
			binder.push_back({ pair.first, pair.second });
		}
	}
	return binder; //Stack is needed due to nested if-statements
}

std::list<IntPair> ProgramInterpreter::Bind_While_Done(const std::vector<std::string>& program) {
	std::list<IntPair> binder;
	std::stack<int> while_stack; // keeps track of the rows where there is if
	IntPair pair;
	std::string command;
	for (size_t i = 0; i < program.size(); ++i) {
		std::istringstream curr(program[i]);
		if (!(curr >> command))
			continue;

		if (command == "WHILE")
			while_stack.push(i);
		else if (command == "DONE") {
			pair.first = while_stack.top();
			while_stack.pop();
			pair.second = i;
			binder.push_back({ pair.first, pair.second }); //Invariant: seconds are sorted in an increasing order
		}
	}
	return binder; //Stack is needed due to nested if-statements
}

bool ProgramInterpreter::ELSECorrectSyntax(const std::vector<std::string>& program) {
	std::string command;
	std::list  <IntPair> if_list = this->Bind_IF_ENDIF(program);
	std::list<IntPair>::iterator it = if_list.begin();

	for (size_t i = 0; i < program.size(); ++i) {
		std::istringstream curr(program[i]);
		if (!(curr >> command))
			continue;

		if (command == "[ELSE") {
			if (if_list.empty())
				return false;

			while ( !(if_list.front().first < i  && if_list.front().second > i) && it != if_list.end())
				++it;
		
			if (it == if_list.end())
				return false;
		
			it = if_list.erase(it);
		}
	}
	return true;
}

bool ProgramInterpreter::StatementsCorrectSyntax(const std::vector<std::string>& program, const std::string& comm1, const std::string& comm2) {
	int balance = 0;
	std::string command;
	for (size_t i = 0; i < program.size(); ++i) {
		if (balance < 0)
			return false;

		std::istringstream rowTracker(program[i]);
		if (!(rowTracker >> command))
			continue;

		if (command == comm1)
			balance++;
		else if (command == comm2)
			balance--;
	}
	return (balance == 0);
}

void ProgramInterpreter::runOnREPL() {
	std::string currentCommand;

	while (true) {

		std::getline(std::cin, currentCommand);
		if (currentCommand == "exit")
			break;

			executeCommand(currentCommand);
	}

}


void ProgramInterpreter::loadLabels() {
	for (size_t i = 0; i < program.size(); ++i) {
		std::istringstream reader(program[i]);
		std::string command;
		if (!(reader >> command))
			continue;
		if (command == "LABEL") {
			std::istringstream readerToExecute(program[i]);
			this->listedLabels.insertLabel(readerToExecute, i);
		}
	}
}

void ProgramInterpreter::runFromFile(const std::string& filename) {
	std::ifstream in(filename, std::ios::in);

	if (!in.is_open())
		throw std::runtime_error("Cannot open file " + filename);

	std::string currentCommand;

	while (std::getline(in, currentCommand)) {
		this->program.push_back(currentCommand);
	}
	if (!this->StatementsCorrectSyntax(program, "IF", "ENDIF"))
		throw std::runtime_error("Invalid IF-ENDIF syntax");

	if (!this->StatementsCorrectSyntax(program, "WHILE", "DONE"))
		throw std::runtime_error("Invalid WHILE-DONE syntax");

	if (!this->ELSECorrectSyntax(program) )
		throw std::runtime_error("Invalid ELSE syntax");

	if (!this->StatementsCorrectSyntax(program, "[ELSE", "]"))
		throw std::runtime_error ("Invalid ELSE brackets");

	this->loadLabels();

	currentRow = 0;
	while (currentRow < this->program.size()) {
		executeCommand(program[currentRow]);
		currentRow++;
	}
	
	in.close();
}


void ProgramInterpreter::enterScope() {
	if (this->scopes.empty())
		scopes.push(std::unordered_map <std::string, int>()); // in global scope we do not copy any variables

	else {
		std::unordered_map<std::string, int> tempScope = this->scopes.top();
		scopes.push(tempScope);
	}
}

void ProgramInterpreter::exitScope() {
	if (this->scopes.empty())
		throw std::underflow_error("Cannot exit scope");

	std::unordered_map <std::string, int> tempScope = this->scopes.top();
	this->scopes.pop();

	for (std::pair <std::string, int> variable : tempScope) {
		if (scopes.top().count(variable.first)) {
			scopes.top().at(variable.first) = variable.second;
		}
	}
}

std::unordered_map<std::string, int>& ProgramInterpreter::currentScope() {
	if (this->scopes.empty())
		throw std::underflow_error("No scopes in the program");

	return this->scopes.top();
}


void ProgramInterpreter::skipToelse() {
	int if_endif_balance = 1;
	currentRow++; // starting from the line after the IF <condition>

	while (if_endif_balance > 0 && currentRow < program.size()) { // if balance equals 0, this means there is no else statement
		std::istringstream statement(program[currentRow]);
		std::string if_else_endif;
		if (!(statement >> if_else_endif)) {
			currentRow++;
			continue;
		}
		if (if_else_endif == "IF")
			if_endif_balance++;
		else if (if_else_endif == "ENDIF")
			if_endif_balance--;
		else if (if_else_endif == "[ELSE" && if_endif_balance == 1) {
			currentRow--; // so that we can land on the line with [ELSE, not the next one

			return; // this means that we have found the corresponding else
		}
		currentRow++;
	}
	currentRow -= 2; //After the execute command iteration it will land exactly on ENDIF
}

void ProgramInterpreter::endIfstatement() {
	int if_endif_balance = 1;
	currentRow++; // starting from the line after the else statement

	while (if_endif_balance > 0 && currentRow < program.size()) {
		std::istringstream if_endif_s(program[currentRow]);
		std::string if_else_endif;
		
		if (!(if_endif_s >> if_else_endif)) {
			currentRow++;
			continue;
		}

		if (if_else_endif == "IF")
			if_endif_balance++;
		else if (if_else_endif == "ENDIF")
			if_endif_balance--;

		currentRow++;
	}
	currentRow-=2; // by doing so after the execute command iteration we will land exactly on ENDIF
}


void ProgramInterpreter::terminateWhile () {
	int while_balance = 0;
	currentRow++; // move to the row after the while statement

	while (currentRow < this->program.size()) { // No exceptions needed, we have ensured while-done syntax is correct in the program
		std::istringstream rowCommand(program[currentRow]);
		std::string statement;
		if (!(rowCommand >> statement)) {
			currentRow++;
			continue;
		}
		if (statement == "WHILE")
			while_balance++;

		else if (statement == "DONE" && while_balance == 0)
			return; // no need to increment, RunFromFile method does it automatically

		else if (statement == "DONE")
			while_balance--;

		currentRow++;
	}
}



void ProgramInterpreter::executeCommand(const std::string& command) {
	std::istringstream first_word(command); // used only to check the first command

	std::istringstream commandToExecute(command);
	std::string keyword;
	first_word >> keyword;
	if (keyword == "LET") {
		std::string ident;
		first_word >> ident;

		if (ident.find('[') != std::string::npos && ident[ident.size() - 1] == ']') {
			size_t bracket_distance = (ident.size() - 1) - ident.find('[');
			std::string length = ident.substr(ident.find('['), bracket_distance);
			if (!this->currentScope().count(length) && std::stoi(length) < 0)
				throw std::invalid_argument("Incorrect array length");

			std::string arr_idn = ident.substr(0, ident.find('['));
			size_t size;
			if (this->currentScope().count(length))
				size = this->currentScope().at(length);
			else {
				size = std::stoi(length);
			}

			if (size > 15) {
				throw std::overflow_error("Array size exceeds the limit (15)");
			}

			this->variableManager.insertArray(arr_idn, size, this->currentScope());
		}
		else {
			variableManager.insertVariable(commandToExecute, currentScope()); // error handling is provided in the method
		}
	}
	else if (keyword == "READ") {
		variableManager.ReadValue(commandToExecute, currentScope(), std::cin);
	}
	else if (keyword == "PRINT") {
		std::string toPrint;
		bool isNumber = true;
		first_word >> toPrint;
		for (size_t i = 0; i < toPrint.size(); ++i) {
			if (!(toPrint[i] <= '9' && toPrint[i] >= '0'))
				isNumber = false;
		}
		if(!isNumber)
			variableManager.PrintValue(commandToExecute, currentScope(), std::cout);
		else {
			std::cout << toPrint;
		}
	}

	else if (keyword == "LABEL") { // All labels are loaded prior to execution
		
	}

	else if (keyword == "GOTO") {
		this->currentRow = listedLabels.GOTOLabel(commandToExecute) - 1; //subtract one, because of the iteration in runFromFile
	}

	else if (keyword == "IF") {
		std::string if_statement, condition;
		commandToExecute >> if_statement;
		std::getline(commandToExecute, condition);

		if (if_statement != "IF")
			throw std::runtime_error("Incorrect if statement syntax");

		this->ifStack.push(this->expressionManager.evaluate(condition, this->currentScope()));

		this->enterScope();

		if (!ifStack.top())
			this->skipToelse();

	}
	else if (keyword == "[ELSE") {

		if (this->ifStack.empty())
			throw std::runtime_error("Incorrect IF-ENDIF statement"); // should be covered already, just to be safe

		if (this->ifStack.top())
		this->endIfstatement();
	}
	else if (keyword == "]") {
		this->endIfstatement();
	}
	else if (keyword == "ENDIF") {
		if (this->ifStack.empty())
			throw std::underflow_error("Incorrect IF-ENDIF statement");
																			//Supposed to be covered (both) just for safety
		

		this->ifStack.pop();
		this->exitScope();
	}
	else if (keyword == "WHILE") {
		std::string while_form, condition;
		commandToExecute >> while_form;
		if (while_form != "WHILE")
			throw std::invalid_argument("Wrong while form");
		std::getline(commandToExecute, condition);

		if (this->expressionManager.evaluate(condition, this->currentScope())) {
			whileStack.push(currentRow);
			this->enterScope();
		}
		else {
			terminateWhile();
		}

	}
	else if (keyword == "DONE") {
		if (whileStack.empty())
			throw std::runtime_error("Incorrect while-done form");

		this->currentRow = whileStack.top() - 1; // returning to the beginning of the while-done form
		whileStack.pop();
		this->exitScope();
	}

	else if (this->currentScope().count(keyword)) { // assigning a value to a variable
		this->variableManager.AssignValue(commandToExecute, this->currentScope());
	}

	else if(keyword=="")
	return; // skip empty lines

	else throw std::invalid_argument("Cannot recognise command on line " + this->currentRow);

}



