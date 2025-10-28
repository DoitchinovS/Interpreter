#pragma once

#ifndef PROGRAM_LABELS_HEADER

#define PROGRAM_LABELS_HEADER

#include <unordered_map>
#include <string>

class ProgramLabels {
private:
	std::unordered_map <std::string, size_t> labels; // represents the connection between the label and its row

public:

	void insertLabel(std::istringstream& input, const size_t& rowIndex);
	const size_t& GOTOLabel(std::istringstream& input);

	bool Contains(const std::string& label)const;
};
#endif // !PROGRAM_LABELS_HEADER