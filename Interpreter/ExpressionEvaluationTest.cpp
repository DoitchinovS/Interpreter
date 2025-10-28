#include "pch.h"
#include "../Interpreter/ExpressionEvaluater.h"


// There is only one public method to be tested

TEST(ExpressionEvaluation, SimpleArithmeticExpressionsWithLiterals) {
	ExpressionEvaluater eval;
	std::unordered_map<std::string, int> scope;
	ASSERT_EQ (eval.evaluate("2 + 3", scope), 5);
	ASSERT_EQ (eval.evaluate("6 - 3 + 1", scope),4);
	ASSERT_EQ(eval.evaluate("17 + 22 / 2", scope), 28);
	ASSERT_EQ(eval.evaluate("12 + 22 * 2", scope), 56);
	ASSERT_EQ(eval.evaluate("( 12 - 5 ) * ( 25 % 4 )", scope), 7); //TODO:Make it possible for negative numbers to participate in the expressions
	ASSERT_EQ(eval.evaluate("15 + 4 * 10", scope), 55);
	ASSERT_EQ(eval.evaluate("6 == 4", scope), 0);
	ASSERT_EQ(eval.evaluate("12 != 3", scope), 1);
	ASSERT_EQ(eval.evaluate("( 4 <= 12 ) && ( 5 >= 7 )", scope), 0);
	ASSERT_EQ(eval.evaluate("( 15 < 7 ) || ( 20 > 13 )", scope), 1);
	ASSERT_EQ (eval.evaluate("! ( ( 12 < 3 ) || ( 10 > 5 ) )", scope), 0);
	ASSERT_EQ(eval.evaluate("( ( 15 < 7 ) || ( 20 > 13 ) ) && ( ( 23 == 12 ) || ( 17 != 10 ) )", scope), 1);
	ASSERT_EQ(eval.evaluate("! 2 || ( 3 != 2 )", scope), 1);
	ASSERT_EQ(eval.evaluate("! 2", scope), 0);


	ASSERT_THROW(eval.evaluate("1 + 7 / 0", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate(" 15 % 0", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate("( 1 + 3 * 4", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate("( 1 + 3 * 4 ) )", scope), std::logic_error);
	ASSERT_THROW (eval.evaluate(" 34 ^ 12 ", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate(" 12 + * 23", scope), std::logic_error);
}

TEST(ExpressionEvaluation, ExpressionsWithVars) {
	ExpressionEvaluater eval;
	std::unordered_map<std::string, int> scope;
	EXPECT_FALSE(scope.count("x"));
	EXPECT_FALSE(scope.count("y"));
	EXPECT_FALSE(scope.count("z"));
	scope.insert({ "x", 10 });
	scope.insert({ "y", 3 });
	scope.insert({ "z", 5 });
	ASSERT_TRUE(scope.count("x") && scope.count("y") && scope.count("z"));

	ASSERT_EQ(eval.evaluate("x + y", scope), 13);
	ASSERT_EQ(eval.evaluate("x + y * z", scope), 25);
	ASSERT_EQ(eval.evaluate("( x + y ) * z", scope), 65);
	ASSERT_EQ(eval.evaluate("( x + 12 ) * y + 2", scope), 68);
	ASSERT_EQ(eval.evaluate("x + z % y", scope), 12);
	ASSERT_EQ(eval.evaluate("x + y % 2", scope), 11);
	ASSERT_EQ(eval.evaluate("x == y", scope), 0);
	ASSERT_EQ(eval.evaluate("y != z", scope), 1);
	ASSERT_EQ(eval.evaluate("( x == 7 ) || ( y <= z )", scope), 1);
	ASSERT_EQ(eval.evaluate("! x && ( z > y )", scope), 0);
	ASSERT_EQ(eval.evaluate("! ( ( x > 9 ) && ( y < 4 ) )", scope), 0);

	ASSERT_THROW(eval.evaluate(" x / 0 + y", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate(" x + y % 0", scope), std::logic_error);
	ASSERT_NO_THROW(eval.evaluate(" x / ( 0 + y )", scope));

	ASSERT_THROW(eval.evaluate("( ( x + 5 ) % 7 + ( y - 6)", scope), std::logic_error);
	ASSERT_THROW (eval.evaluate("( x + 5 ) / + 3", scope), std::logic_error);
	ASSERT_THROW (eval.evaluate(" ( x + z ) # 12", scope), std::logic_error);

}

TEST(ExpressionEvaluation, ExpressionWithNegatives) {
	ExpressionEvaluater eval;
	std::unordered_map<std::string, int> scope;
	EXPECT_FALSE(scope.count("x"));
	EXPECT_FALSE(scope.count("y"));
	EXPECT_FALSE(scope.count("z"));
	EXPECT_FALSE(scope.count("divisor"));

	scope.insert({ "x", 10 });
	scope.insert({ "y", 15 });
	scope.insert({ "z", 9 });
	scope.insert({ "var", 6 }); //multiletter variable
	scope.insert({ "divisor", 0 });

	ASSERT_TRUE(scope.count("x"));
	ASSERT_TRUE(scope.count("y"));
	ASSERT_TRUE(scope.count("z"));
	ASSERT_TRUE(scope.count("var"));
	ASSERT_TRUE(scope.count("divisor"));

	ASSERT_EQ(eval.evaluate("2 + -3", scope), -1);
	ASSERT_EQ(eval.evaluate("6 * -3 + 1 ", scope), -17);
	ASSERT_EQ(eval.evaluate("-8 - 7 * 3", scope), -29);
	ASSERT_EQ(eval.evaluate("12 - -5 + 11", scope), 28);
	ASSERT_EQ(eval.evaluate("12 - -5 * 7", scope), 47);


	ASSERT_EQ(eval.evaluate("15 / -7", scope), -2);
	ASSERT_EQ(eval.evaluate("-13 % 3", scope), -1);
	ASSERT_EQ(eval.evaluate("17 % -5", scope), 2);
	ASSERT_EQ(eval.evaluate("-23 % -3", scope), -2);

	ASSERT_EQ(eval.evaluate("-3 * x + y / 3", scope), -25);
	ASSERT_EQ(eval.evaluate("12 + -4 * z", scope), -24);
	ASSERT_EQ(eval.evaluate("15 * -x + ( z % 4 ) ", scope), -149);
	ASSERT_EQ(eval.evaluate("-y / 5", scope), -3);
	ASSERT_EQ(eval.evaluate("22 % -var", scope), 4);
	ASSERT_EQ(eval.evaluate("-z % 22 ", scope), -9);
	ASSERT_EQ(eval.evaluate("-y % -x", scope), -5 );
	ASSERT_EQ(eval.evaluate("! -x", scope), 0);

	ASSERT_EQ(eval.evaluate("15 * -x + ( var % 4 )", scope), -148);
	ASSERT_EQ(eval.evaluate("15 * -var + x / 3", scope), -87);

	ASSERT_EQ(eval.evaluate("! ( -15 + 13 + z % 7 )", scope), 1);

	ASSERT_THROW(eval.evaluate(" 13 - - 5", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate("-var - ( ( -5 * 6 ) + 9 ( )", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate(" 13 - - 5", scope), std::logic_error);

	ASSERT_THROW (eval.evaluate("16 + -3 * 10 )", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate(" -var + -x $ 2",scope), std::logic_error);

	ASSERT_THROW(eval.evaluate("( -var + 5 * 7 ) /", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate("* -var + 5 * 7 ", scope), std::logic_error);

	ASSERT_THROW(eval.evaluate("-var  / 0 ", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate("-var  / divisor ", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate("var  / -divisor ", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate("x  % divisor ", scope), std::logic_error);
	ASSERT_THROW(eval.evaluate("x  % -divisor ", scope), std::logic_error);
}

