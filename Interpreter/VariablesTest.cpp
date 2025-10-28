#include "pch.h"
#include "../Interpreter/ProgramVariables.h"

TEST(LETForm, ValidLETForm) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	std::string letForm = "LET a";
	std::istringstream command(letForm);

	ASSERT_NO_THROW (vr.insertVariable(command, scope));
	ASSERT_TRUE(scope.count("a"));
}

TEST(LETform, SmallLettersLetForm) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	std::string letForm = "let a";
	std::istringstream command(letForm);
	ASSERT_THROW(vr.insertVariable(command, scope), std::invalid_argument);
}

TEST(LETform, InvalidLETName) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	std::string letForm = "LE1T a";
	std::istringstream command(letForm);
	ASSERT_THROW(vr.insertVariable(command, scope), std::invalid_argument);
}

TEST(LETform, DeclaringExistingVar) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	EXPECT_TRUE(!scope.count("a"));
	scope.insert({ "a" , 1 });
	ASSERT_TRUE(scope.count("a"));

	std::string letForm = "LET a";
	std::istringstream command(letForm);

	ASSERT_THROW(vr.insertVariable(command, scope), std::runtime_error);
}

TEST(LETform, InvalidIdentifier) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;

	std::string letForm = "LET 11a";
	std::istringstream command(letForm);
	ASSERT_THROW(vr.insertVariable(command, scope), std::invalid_argument);
}

TEST(LETform, VarBelongingInScope) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	EXPECT_TRUE(!scope.count("a"));

	std::string letForm = "LET a";
	std::istringstream command(letForm);

	vr.insertVariable(command, scope);
	ASSERT_TRUE(scope.count("a"));
}

// -----------------PRINT Form -----------------------------------

TEST(PrintForm, SmallLetterPrint) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	std::string Printform = "print var";
	std::istringstream command(Printform);
	std::ostringstream out;
	ASSERT_THROW (vr.PrintValue(command, scope, out), std::invalid_argument);
}

TEST(PrintForm, IncorrectPrint) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	std::string Printform = "PR_1INT var";
	std::istringstream command(Printform);
	std::ostringstream out;
	ASSERT_THROW(vr.PrintValue(command, scope, out), std::invalid_argument);
}

TEST(PrintForm, UndeclaredVariable) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	ASSERT_FALSE(scope.count("var"));

	std::string Printform = "PRINT var";
	std::istringstream command(Printform);
	std::ostringstream out;
	ASSERT_THROW(vr.PrintValue(command, scope, out), std::runtime_error);
}

TEST(PrintForm, ValidPrintSyntax) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	EXPECT_FALSE (scope.count("var"));

	scope.insert({ "var", 15 });
	ASSERT_TRUE(scope.count("var"));

	std::string Printform = "PRINT var";
	std::istringstream command(Printform);
	std::ostringstream out;
	vr.PrintValue(command, scope, out);
	ASSERT_EQ(out.str(), "15");
}

//-------------------------READ form-----------------------
TEST(ReadForm, SmallLetterForm) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	std::string Printform = "read var";
	std::istringstream command(Printform);
	std::istringstream in;

	ASSERT_THROW(vr.ReadValue(command, scope, in), std::invalid_argument);
}

TEST(ReadForm, IncorrectReadForm) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	std::string Printform = "REA1_D var";
	std::istringstream command(Printform);
	std::istringstream in;

	ASSERT_THROW(vr.ReadValue(command, scope, in), std::invalid_argument);
}

TEST(ReadForm, UndeclaredVariable) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	ASSERT_FALSE(scope.count("var"));
	std::string Printform = "READ var";
	std::istringstream command(Printform);
	std::istringstream in("15");

	ASSERT_THROW(vr.ReadValue(command, scope, in), std::runtime_error);
}

TEST(ReadForm, ValidReadForm) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	ASSERT_FALSE(scope.count("var"));
	scope.insert({ "var", 12 });
	ASSERT_TRUE(scope.count("var"));

	std::string Printform = "READ var";
	std::istringstream command(Printform);
	std::istringstream in ("12");
	vr.ReadValue(command, scope, in);
	ASSERT_EQ(scope.at("var"), 12);
}


//--------------Assignment Testing -------------------------

TEST(AssignmentTesting, UndeclaredVar) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	ASSERT_FALSE(scope.count("x"));
	std::istringstream command("x = x + 1");
	ASSERT_THROW(vr.AssignValue(command, scope), std::runtime_error);
}

TEST(AssignmentTesting, UndeclaredVarExistingAnother) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	ASSERT_FALSE(scope.count("x"));
	scope.insert({ "y", 12 });
	std::istringstream command("x = x + 1");
	EXPECT_TRUE(scope.count("y"));
	ASSERT_THROW(vr.AssignValue(command, scope), std::runtime_error);
}

TEST(AssignmentTesting, IncorrectAssignmentOperator) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	EXPECT_FALSE(scope.count("x"));
	scope.insert({ "x" , 10 });
	ASSERT_TRUE(scope.count("x"));

	std::istringstream command("x * x + 1");

	ASSERT_THROW(vr.AssignValue(command, scope), std::invalid_argument);
}

TEST(AssignmentTesting, IncorrectAssignmentOperatorWithExistingAnotherVar) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	scope.insert({ "y", 10 });
	EXPECT_TRUE(scope.count("y"));
	ASSERT_FALSE(scope.count("x"));
	scope.insert({ "x" , 12 });
	ASSERT_TRUE(scope.count("x"));

	std::istringstream command("x == x + 1");

	ASSERT_THROW(vr.AssignValue(command, scope), std::invalid_argument);
}

TEST(AssignmentTesting, CorrectValueAssigning) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	scope.insert({ "y", 10 });
	scope.insert({ "x", 20 });
	ASSERT_TRUE(scope.count("y"));
	ASSERT_TRUE(scope.count("x"));
	std::istringstream command("y = y + 1");
	std::istringstream command2("x = ( 6 * 5 + 1 ) * 2"); // Warning: Expressions are space sensitive
	vr.AssignValue(command, scope);
	vr.AssignValue(command2, scope);
	ASSERT_TRUE(scope.at("y") == 11);
	ASSERT_TRUE(scope.at("x") == 62 );
}

// ------------------------------------------insertingArrayTesting------------

TEST(InsertArray, IdentifierDuplicate) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	EXPECT_TRUE(!scope.count("arr"));
	scope.insert({ "arr", 12 });
	ASSERT_TRUE(scope.count("arr"));
	size_t size = 6;
	ASSERT_THROW(vr.insertArray("arr", 6, scope), std::runtime_error);
}

TEST(InsertArray, InvalidArrayName) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	size_t size = 6;
	ASSERT_FALSE(scope.count("1arr"));
	ASSERT_THROW(vr.insertArray("1arr", 6, scope), std::invalid_argument);
}

TEST (InsertArray, ValidArrayNameCapitalLetters) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	size_t size = 6;
	EXPECT_FALSE(scope.count("ARR"));
	scope.insert({ "ARR", 10 });

	ASSERT_TRUE(scope.count("ARR"));
	ASSERT_FALSE(scope.count("arr"));

	ASSERT_NO_THROW(vr.insertArray("arr", 6, scope));
}

TEST(InsertArray, ValidInsertion) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	size_t size = 6;
	ASSERT_FALSE(scope.count("arr"));

	vr.insertArray("arr", size, scope);

	for (size_t i = 0; i < size; ++i) {
		ASSERT_TRUE(scope.count("arr[" + std::to_string(i) + "]"));
		ASSERT_EQ(scope.at("arr[" + std::to_string(i) + "]"), INT_MAX);
	}
}

TEST(InsertArray, ArraySizeZero) {
	ProgramVariables vr;
	std::unordered_map<std::string, int> scope;
	size_t size = 0;
	ASSERT_FALSE(scope.count("arr"));
	vr.insertArray("arr", size, scope);

	ASSERT_FALSE(scope.count("arr[0]"));

	ASSERT_FALSE(scope.count("arr"));
}
