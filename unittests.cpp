#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"

#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "expression.hpp"

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp" 
#include "tokenize.hpp"

static Expression run(const std::string& program) {

	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);
	if (!ok) {
		std::cerr << "[GRADER] Failed to parse: " << program << std::endl;
	}
	REQUIRE(ok == true);

	Expression result;
	REQUIRE_NOTHROW(result = interp.eval());

	return result;
}

TEST_CASE("Test Expression default constructor", "[types]") {

	Expression exp1;

	REQUIRE(exp1 == Expression());
}

TEST_CASE("Test Expression atom constructor", "[types]") {

	Atom a;

	std::string token = "True";
	REQUIRE(token_to_atom(token, a));

	Expression exp1 = Expression(a);

	REQUIRE(exp1.head.value.bool_value);
}

TEST_CASE("Test Expression value constructors", "[types]") {

	double num = 2.0;
	std::string sym = "sym";

	Expression exp1 = Expression(true);
	Expression exp2 = Expression(num);
	Expression exp3 = Expression(sym);

	REQUIRE(exp1.head.value.bool_value);
	REQUIRE(exp2.head.value.num_value == 2.0);
	REQUIRE(exp3.head.value.sym_value == "sym");

}


// TODO: add your unit tests here
TEST_CASE("Test Type Inference[1]", "[types]") {

	Atom a;

	std::string token = "True";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == BooleanType);
	REQUIRE(a.value.bool_value == true);

	token = "False";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == BooleanType);
	REQUIRE(a.value.bool_value == false);

	token = "1";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == NumberType);
	REQUIRE(a.value.num_value == 1);

	token = "-1";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == NumberType);
	REQUIRE(a.value.num_value == -1);

	token = "var";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == SymbolType);
	REQUIRE(a.value.sym_value == "var");

	token = "1abc";
	REQUIRE(!token_to_atom(token, a));

	token = "var1";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == SymbolType);
	REQUIRE(a.value.sym_value == token);

	token = "v2ar1";
	REQUIRE(token_to_atom(token, a));
	REQUIRE(a.type == SymbolType);
	REQUIRE(a.value.sym_value == token);

}


TEST_CASE("Test Tokenizer with expected input[1]", "[tokenize]") {

	std::string program = "(begin (define r 10) (* pi (* r r)))";

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);

	REQUIRE(tokens.size() == 17);
	REQUIRE(tokens[0] == "(");
	REQUIRE(tokens[1] == "begin");
	REQUIRE(tokens[2] == "(");
	REQUIRE(tokens[3] == "define");
	REQUIRE(tokens[4] == "r");
	REQUIRE(tokens[5] == "10");
	REQUIRE(tokens[6] == ")");
	REQUIRE(tokens[7] == "(");
	REQUIRE(tokens[8] == "*");
	REQUIRE(tokens[9] == "pi");
	REQUIRE(tokens[10] == "(");
	REQUIRE(tokens[11] == "*");
	REQUIRE(tokens[12] == "r");
	REQUIRE(tokens[13] == "r");
	REQUIRE(tokens[14] == ")");
	REQUIRE(tokens[15] == ")");
	REQUIRE(tokens[16] == ")");
}

TEST_CASE("Test Tokenizer with truncated input[2]", "[tokenize]") {

	std::string program = "(f";

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);

	REQUIRE(tokens.size() == 2);
	REQUIRE(tokens[0] == "(");
	REQUIRE(tokens[1] == "f");
}

TEST_CASE("Test Tokenizer with single non-keyword[1]", "[tokenize]") {

	std::string program = "hello";

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);

	REQUIRE(tokens.size() == 1);
	REQUIRE(tokens[0] == "hello");
}

TEST_CASE("Test Tokenizer with empty input[1]", "[tokenize]") {

	std::string program;

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);

	REQUIRE(tokens.size() == 0);
}

TEST_CASE("Test Tokenizer with empty expression[1]", "[tokenize]") {

	std::string program = "( )";

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);

	REQUIRE(tokens.size() == 2);
	REQUIRE(tokens[0] == "(");
	REQUIRE(tokens[1] == ")");
}

TEST_CASE("Test Interpreter parser and eval with expected input[1]", "[interpreter]") {

	std::string program = "(- (+ 3 10) (* 3 (* 2 2) ) )";

	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	Expression result = interp.eval();

	REQUIRE(result.head.value.num_value == 1);

	REQUIRE(ok == true);
}

TEST_CASE("Test eval with m-ary function[1]", "[interpreter]") {


	std::string program = "(+ (+ 3 10) (* 3 (* 2 2) ) 25)";

	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	Expression result = interp.eval();


	REQUIRE(result.head.value.num_value == 50);

	REQUIRE(ok == true);
}

TEST_CASE("Test eval with m-ary function[2]", "[interpreter]") {

	std::string program = "(+ (+ 3 10) 25 (* 3 (* 2 2) ))";

	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	Expression result = interp.eval();

	REQUIRE(result.head.value.num_value == 50);

	REQUIRE(ok == true);
}

TEST_CASE("Test eval with m-ary function[3]", "[interpreter]") {

	std::string program = "(+ 25 (+ 3 10) (* 3 (* 2 2) ) )";

	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	Expression result = interp.eval();

	REQUIRE(result.head.value.num_value == 50);

	REQUIRE(ok == true);
}

TEST_CASE("Simple test begin / define[1]", "[interpreter]") {

	std::string program = "( begin (define a 25) (+ 25 a) )";

	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	Expression result = interp.eval();

	REQUIRE(result.head.value.num_value == 50);

	REQUIRE(ok == true);
}

TEST_CASE("Test begin / define with symbols", "[interpreter]") {

	std::string program = "( begin (define a True) (define b a) (and a b))";

	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	Expression result = interp.eval();

	REQUIRE(result.head.value.bool_value);

	REQUIRE(ok == true);
}

TEST_CASE("Test arithmetic procedures", "[interpreter]") {

	{
		std::vector<std::string> programs = { "(+ 1 -2)",
						 "(+ -3 1 1)",
						 "(- 1)",
						 "(- 1 2)",
						 "(* 1 -1)",
						 "(* 1 1 -1)",
						 "(/ -1 1)",
						 "(/ 1 -1)" };

		for (auto s : programs) {
			Expression result = run(s);
			REQUIRE(result == Expression(-1.));
		}
	}
}

TEST_CASE("Test bad strings", "[interpreter]") {

	//std::string program = "( begin (define a True) (define b a) (and a b))";
	//Interpreter interp;

	std::vector<std::string> programs = { "( not True False)", "(not 1)", "(and 1 True)", "(or True 1)",
		"(< 1)", "(< nan)", "(<= 1)", "(<= nan)", "(> 1)", "(> nan)", "(>= 1)", "(>= nan)", "(= 1)", "(= nan)",
		"(add nan)", "(- 1 1 1)", "(- nan)", "(* nan 1)", "(/ nan 1)", "(/ 1)", "(pow 1)",  "(pow nan 2)", "(log10 1 1)", "(log10 nan)"
	};


	for (auto prg : programs)
	{
		std::istringstream iss(prg);

		Interpreter interp;

		bool ok = interp.parse(iss);
		if (!ok) {
			std::cerr << "[GRADER] Failed to parse: " << prg << std::endl;
		}
		REQUIRE(ok == true);
		Expression result;
		REQUIRE_THROWS(result = interp.eval());
		
	}

	


}
