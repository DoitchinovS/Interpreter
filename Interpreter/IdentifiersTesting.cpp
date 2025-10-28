#include "pch.h"
#include "../Interpreter/IdentifierValidation.h"

TEST(IdentifierValidation, ParameterConstructorTest) { // Positive test
	IdentifierValidation ident("Myident");
	ASSERT_TRUE(ident.getIdentName() == "Myident");
}


TEST(IdentifierValidation, DefaultConstructorTest) { //Positive test
	IdentifierValidation ident;
	ASSERT_TRUE(ident.getIdentName() == "\0");
}

TEST(IdentifierValidation, ValidIdent) { // Positive test
	IdentifierValidation ident("ABCDEFGzxyw");
	ASSERT_TRUE(ident.isValidIdent());
}

TEST(IdentifierValidation, ValidIdentUnderScore) {
	IdentifierValidation ident("_abcdas");
	ASSERT_TRUE(ident.isValidIdent());
}

TEST(IndentifierValidation, ValidIndentDigit) {
	IdentifierValidation ident("abcdasfg45aw");
	ASSERT_TRUE(ident.isValidIdent());
}

TEST(IdentifierValidation, CapitalFirstLetter) {
	IdentifierValidation ident("A1123_abcd");
	ASSERT_TRUE(ident.isValidIdent());
}

TEST(IdentifierValidation, IncorrectFirstSymbol) {
	IdentifierValidation ident("#abcd12_3");
	ASSERT_FALSE(ident.isValidIdent());
}

TEST(IdentifierValidation, IncorrectFirstSymbolDigit) {
	IdentifierValidation ident("4abcd12_3");
	ASSERT_FALSE(ident.isValidIdent());
}


TEST(IdentifierValidation, OverloadedSize) {
	IdentifierValidation ident("abcdefghithyedsafjghnfgig");
	ASSERT_FALSE(ident.isValidIdent());
}

TEST(IdentifierValidation, SlightlyOverloadedSize) {
	IdentifierValidation ident("abcdefghityrejias"); // Size = 16
	ASSERT_FALSE(ident.isValidIdent());
}


TEST(IdentifierValidation, ReservedWordIndent) {
	IdentifierValidation ident1("LET");
	ASSERT_FALSE(ident1.isValidIdent());

	IdentifierValidation ident2("READ");
	ASSERT_FALSE(ident2.isValidIdent());

	IdentifierValidation ident3("read");
	ASSERT_TRUE(ident3.isValidIdent());

	IdentifierValidation ident4("LET1");
	ASSERT_TRUE(ident4.isValidIdent());
}

TEST(IdentifierValidation, ContainIncorrectSymbol) {
	IdentifierValidation ident("abqwer!_");
	ASSERT_FALSE(ident.isValidIdent());
}

TEST(IdentifierValidation, ContainSpace) {
	IdentifierValidation ident("abcd ef");
	ASSERT_FALSE(ident.isValidIdent());
}

TEST(IdentifierValidation, EmptyIdentifier) {
	IdentifierValidation ident("");
	ASSERT_FALSE(ident.isValidIdent());
}

TEST(IdentifierValidation, SingleUnderscore) {
	IdentifierValidation ident("_");
	ASSERT_TRUE(ident.isValidIdent());
}

TEST(IdentifierValidation, SingleLetter) {
	IdentifierValidation ident("a");
	ASSERT_TRUE(ident.isValidIdent());
}

TEST(IdentifierValidation, SingleDigit) {
	IdentifierValidation ident("4");
	ASSERT_FALSE(ident.isValidIdent());
}

TEST(IdentifierValidation, AllCapitalLetters) {
	IdentifierValidation ident("INT");
	ASSERT_TRUE(ident.isValidIdent());
}

TEST(IdentifierValidation, IdentationName) {
	IdentifierValidation ident(" ");
	ASSERT_FALSE(ident.isValidIdent());
}

