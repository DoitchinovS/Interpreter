#include "ProgramLabels.h"
#include <sstream>
#include "IdentifierValidation.h"

void ProgramLabels::insertLabel(std::istringstream& input, const size_t& rowIndex) { //rowIndex is Rvalue, so it can be given as a const refr
	std::string label_form, label_name;
	input >> label_form >> label_name; // istringstream skips whitespaces so there is no space sensetivity

	if (label_form != "LABEL")
		throw std::runtime_error("Wrong label syntax!");

	if (labels.count(label_name)) {
		if (labels.at(label_name) != rowIndex)
			throw std::runtime_error("Label " + label_name + " already exists");

		else return; // Distinguish between visiting and inserting label (No need to throw error, just don't insert)
	}

	IdentifierValidation idn(label_name);
	if (!idn.isValidIdent())
		throw std::invalid_argument("Invalid label name on row " + rowIndex);

	this->labels.insert({ label_name, rowIndex });
}

const size_t& ProgramLabels::GOTOLabel(std::istringstream& input) {
	std::string goto_form, label;
	input >> goto_form >> label;

	if (goto_form != "GOTO")
		throw std::runtime_error("Wrong goto syntax!");

	if (!labels.count(label))
		throw std::invalid_argument("Cannot find " + label + " label");

	return labels.at(label);
}

bool ProgramLabels::Contains(const std::string& label)const {
	return (this->labels.count(label));
}


