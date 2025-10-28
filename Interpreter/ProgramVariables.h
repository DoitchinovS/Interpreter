#pragma once

#ifndef PROGRAM_VARIABLES_HEADER

#define PROGRAM_VARIABLES_HEADER

#include <unordered_map>
#include <sstream>


class ProgramVariables {
private:
	

public:

	void insertVariable(std::istringstream& in, std::unordered_map<std::string, int>& scope); //LET form
	void AssignValue(std::istringstream& input, std::unordered_map<std::string, int>& scope); // operator = form

	void PrintValue(std::istringstream& input, const std::unordered_map<std::string, int>& scope, std::ostream& out)const; // PRINT form
	void ReadValue(std::istringstream& input, std::unordered_map<std::string, int>& scope, std::istream& in); // READ form

	void insertArray(const std::string& arr_ident,const size_t& size, std::unordered_map<std::string, int>& scope);
};

#endif // !PROGRAM_VARIABLES_HEADER