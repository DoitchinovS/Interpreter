#include "pch.h"
#include "../Interpreter/ProgramLabels.h"
#include <sstream>

TEST(InsertLabelTest, CorrectLabelForm) {
	ProgramLabels lb;
	std::string Labelform = "LABEL Simeon";
	std::istringstream command(Labelform);

	ASSERT_NO_THROW(lb.insertLabel(command, 1));
}

TEST(InsertLabelTest, CorrectInsertion) {
	ProgramLabels lb;
	std::string Labelform = "LABEL Simeon";
	std::istringstream command(Labelform);
	lb.insertLabel(command, 1);
	ASSERT_TRUE (lb.Contains("Simeon"));
}

TEST(InsertLabelTest, SmallLettersForm) {
	ProgramLabels lb;
	std::string Labelform = "label Simeon";
	std::istringstream command(Labelform);

	ASSERT_THROW(lb.insertLabel(command, 1), std::runtime_error);
}

TEST(InsertLabelTest, WrongKeyWord) {
	ProgramLabels lb;
	EXPECT_TRUE(!lb.Contains("Simeon"));
	std::string Labelform = "LABLE Simeon";
	std::istringstream command(Labelform);

	ASSERT_THROW(lb.insertLabel(command, 1), std::runtime_error);
}

TEST(InsertLabelTest, WrongLabelIdentifierStart) {
	ProgramLabels lb;
	EXPECT_TRUE (!lb.Contains("1Simeon"));
	std::string Labelform = "LABEL 1Simeon";
	std::istringstream command(Labelform);

	ASSERT_THROW(lb.insertLabel(command, 1), std::invalid_argument);
}

TEST(InsertLabelTest, CorrectLabelUnderscore) {
	ProgramLabels lb;
	EXPECT_TRUE(!lb.Contains("_Simeon"));
	std::string Labelform = "LABEL _Simeon";
	std::istringstream command(Labelform);

	ASSERT_NO_THROW(lb.insertLabel(command, 1));
}

TEST(InsertLabelTest, LabelNameDuplicate) {
	ProgramLabels lb;
	EXPECT_TRUE(!lb.Contains("Simeon"));
	std::string Labelform = "LABEL Simeon";
	std::string Labelform2 = "LABEL Simeon";
	std::istringstream command(Labelform);
	std::istringstream command2(Labelform2);

	ASSERT_NO_THROW(lb.insertLabel(command, 1));
	ASSERT_TRUE(lb.Contains("Simeon"));
	ASSERT_THROW(lb.insertLabel(command2, 2), std::runtime_error);
}

TEST(InsertLabelTest, LabelNameAndRowDuplicate) {
	ProgramLabels lb;
	EXPECT_TRUE(!lb.Contains("Simeon"));
	std::string Labelform = "LABEL Simeon";
	std::string Labelform2 = "LABEL Simeon";
	std::istringstream command(Labelform);
	std::istringstream command2(Labelform2);

	ASSERT_NO_THROW(lb.insertLabel(command, 1));
	ASSERT_TRUE(lb.Contains("Simeon"));
	ASSERT_NO_THROW(lb.insertLabel(command2, 1));
}

TEST(InsertLabelTest, LabelFormWithWhitespaces) {
	ProgramLabels lb;
	ASSERT_TRUE(!lb.Contains(" Simeon"));
	ASSERT_TRUE(!lb.Contains("  Simeon"));
	EXPECT_TRUE(!lb.Contains("Simeon"));

	std::string Labelform = "LABEL   Simeon";
	std::istringstream command(Labelform);

	ASSERT_NO_THROW(lb.insertLabel(command, 1));
	ASSERT_TRUE(lb.Contains("Simeon"));
	ASSERT_FALSE(lb.Contains("  Simeon"));
	ASSERT_FALSE(lb.Contains(" Simeon"));
}

TEST(InsertLabelTest, EmptyLabelName) { // ensures the label name cannot be put on the next line (because of the code in the interpreter)
	ProgramLabels lb;
	std::string Labelform = "LABEL     ";
	std::istringstream command(Labelform);
	ASSERT_THROW(lb.insertLabel(command, 1), std::invalid_argument);
}

// ------------- GOTO Method Testing ---------------------------------------

TEST(GOTOLabelTest, WrongGotoSyntax) {
	ProgramLabels lb;
	std::string GOTOForm = "GOwgTO Simeon";
	std::istringstream command(GOTOForm);
	ASSERT_THROW(lb.GOTOLabel(command), std::runtime_error);
}

TEST(GOTOLabelTest, WrongSyntaxSmallLetters) {
	ProgramLabels lb;
	std::string GOTOForm = "goto Simeon";
	std::istringstream command(GOTOForm);
	ASSERT_THROW(lb.GOTOLabel(command), std::runtime_error);
}

TEST(GOTOLabelTest, AccessingUndefinedLabel) {
	ProgramLabels lb;
	ASSERT_FALSE (lb.Contains("Simeon"));
	std::string GOTOForm = "GOTO Simeon";
	std::istringstream command(GOTOForm);
	ASSERT_THROW(lb.GOTOLabel(command), std::invalid_argument);
}

TEST(GOTOLabelTest, AccessingUndefinedLabelIfThereAreOther) {
	ProgramLabels lb;
	ASSERT_FALSE(lb.Contains("Simeon"));
	std::string firstLabel = "LABEL Simo";
	std::istringstream command1(firstLabel);
	ASSERT_NO_THROW(lb.insertLabel(command1, 1));
	ASSERT_TRUE(lb.Contains("Simo"));

	std::string GOTOForm = "GOTO Simeon";
	std::istringstream command(GOTOForm);
	ASSERT_THROW(lb.GOTOLabel(command), std::invalid_argument);
}

TEST(GOTOLabelTest, CorrectGOTOUsage) {
	ProgramLabels lb;
	ASSERT_FALSE(lb.Contains("Simeon"));
	std::string firstLabel = "LABEL Simeon";
	std::istringstream command1(firstLabel);
	ASSERT_NO_THROW (lb.insertLabel(command1, 1));
	ASSERT_TRUE(lb.Contains("Simeon"));

	std::string GOTOForm = "GOTO Simeon";
	std::istringstream command2(GOTOForm);
	std::istringstream command3(GOTOForm);
	ASSERT_NO_THROW (lb.GOTOLabel(command2));
	ASSERT_TRUE(lb.GOTOLabel(command3) == 1);
}
