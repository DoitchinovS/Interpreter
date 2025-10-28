#pragma once

#ifndef IDENTIFIER_VALIDATION_HEADER

#define IDENTIFIER_VALIDATION_HEADER

#include <string> 
#include <fstream>

#define MAX_SIZE 15

class IdentifierValidation {
private:
	const std::string ident;

	bool ContainCorrectSymbols()const; // function that verifies only letters, digits and underscore are used
	bool CorrectBeginning()const; // function that checks if the first symbol is a letter or an underscore
	bool isIdentNotReserved()const;

public:
	IdentifierValidation() = default;
	IdentifierValidation(const std::string& name): ident(name){}

	bool isValidIdent()const;
	const std::string& getIdentName()const;
};
#endif // IDENTIFICATOR_VALIDATION_HEADER
