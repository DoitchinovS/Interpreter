
#include "IdentifierValidation.h"

const std::string& IdentifierValidation::getIdentName()const { // Using const reference in order to avoid copying heavy objects
	return this->ident;
}

bool IdentifierValidation::isValidIdent()const {
	return (CorrectBeginning() && ContainCorrectSymbols()
		&& isIdentNotReserved() && this->ident.size() <= MAX_SIZE); // All the necessarry conditions
}

bool IdentifierValidation::isIdentNotReserved() const {
	return !(ident == "LABEL" || ident == "GOTO" ||
		ident == "LET" || ident == "READ" ||
		ident == "PRINT" || ident == "IF" ||
		ident == "ENDIF" || ident == "ELSE" ||
		ident == "WHILE" || ident == "DONE");
}

bool IdentifierValidation::CorrectBeginning() const {
	return (ident[0] >= 'a' && ident[0] <= 'z') || (ident[0] >= 'A' && ident[0] <= 'Z') || ident[0] == '_';
}


bool IdentifierValidation::ContainCorrectSymbols()const {

	for (char current : ident) {
		if (!((current >= 'a' && current <= 'z') ||
			(current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9') || current == '_'))
			return false;
	}

	return true;
}
