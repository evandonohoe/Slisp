#include "environment.hpp"

#include <cassert>
#include <cmath>
#include <algorithm>

#include "interpreter_semantic_error.hpp"

Expression not_exp(const std::vector<Atom>& args) {
	Boolean result = false;
	if (args.size() != 1) {
		throw InterpreterSemanticError("Error in call to not, too many arguments");
	}
	for (auto& a : args) {
		if (a.type != BooleanType) {
			throw InterpreterSemanticError("Error in call to not, argument not a boolean");
		}
	}
	result = !(args[0].value.bool_value);
	return Expression(result);

}

Expression and_exp(const std::vector<Atom>& args) {
	for (auto& a : args) {
		if (a.type != BooleanType) {
			throw InterpreterSemanticError("Error in call to and, argument not a boolean");

		}
	}
	//Boolean result = std::all_of(args.begin(), args.end(), [](bool b) { return b; });
	std::vector<bool> bools(args.size());
	std::transform(args.begin(), args.end(), bools.begin(),
		[](const Atom& s) { return s.value.bool_value; });
	Boolean result = std::all_of(bools.begin(), bools.end(), [](bool b) { return b; });
	return Expression(result);

}

Expression or_exp(const std::vector<Atom>& args) {
	for (auto& a : args) {
		if (a.type != BooleanType) {
			throw InterpreterSemanticError("Error in call to or, argument not a boolean");
		}
	}
	//Boolean result = std::any_of(args.begin(), args.end(), [](bool b) { return b; });
	std::vector<bool> bools(args.size());
	std::transform(args.begin(), args.end(), bools.begin(),
		[](const Atom& s) { return s.value.bool_value; });
	Boolean result = std::any_of(bools.begin(), bools.end(), [](bool b) { return b; });
	return Expression(result);

}

Expression lessthan(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error in call to lessthan, wrong number of arguments");
	}
	for (auto& a : args) {
		if (a.type != NumberType) {
			throw InterpreterSemanticError("Error in call to lessthan, argument not a number");
		}


	}
	Number first = args[0].value.num_value;
	Number second = args[1].value.num_value;
	Boolean result = std::isless(first, second);
	return Expression(result);

}

Expression lessthanequal(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error in call to lessthanequal, wrong number of arguments");
	}
	for (auto& a : args) {
		if (a.type != NumberType) {
			throw InterpreterSemanticError("Error in call to lessthanequal, argument not a number");
		}


	}
	Number first = args[0].value.num_value;
	Number second = args[1].value.num_value;
	Boolean result = std::islessequal(first, second);
	return Expression(result);
}

Expression greaterthan(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error in call to greaterthan, wrong number of arguments");
	}
	for (auto& a : args) {
		if (a.type != NumberType) {
			throw InterpreterSemanticError("Error in call to greaterthan, argument not a number");
		}


	}
	Number first = args[0].value.num_value;
	Number second = args[1].value.num_value;
	Boolean result = std::isgreater(first, second);
	return Expression(result);

}

Expression greaterthanequal(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error in call to greaterthanequal, wrong number of arguments");
	}
	for (auto& a : args) {
		if (a.type != NumberType) {
			throw InterpreterSemanticError("Error in call to greaterthanequal, argument not a number");
		}


	}
	Number first = args[0].value.num_value;
	Number second = args[1].value.num_value;
	Boolean result = std::isgreaterequal(first, second);
	return Expression(result);

}

Expression equal_exp(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error in call to equal, wrong number of arguments");
	}
	for (auto& a : args) {
		if (a.type != NumberType) {
			throw InterpreterSemanticError("Error in call to equal, argument not a number");
		}


	}
	Number first = args[0].value.num_value;
	Number second = args[1].value.num_value;
	Boolean result = (first == second);
	return Expression(result);

}

Expression add_exp(const std::vector<Atom>& args) {
	Number result = 0;
	for (auto& a : args) {
		if (a.type != NumberType) {
			throw InterpreterSemanticError("Error in call to add, argument not a number:");

		}
		result += a.value.num_value;
	}
	return Expression(result);

}

Expression minusorneg(const std::vector<Atom>& args) {
	if (args.size() != 2 && args.size() != 1) {
		throw InterpreterSemanticError("Error in call to minus, wrong number of arguments");
	}
	for (auto& a : args) {
		if (a.type != NumberType) {
			throw InterpreterSemanticError("Error in call to minus, argument not a number");
		}
	}
	Number result = 0;
	if (args.size() == 2) {
		Number first = args[0].value.num_value;
		Number second = args[1].value.num_value;
		result = (first - second);
	}
	else if (args.size() == 1) {
		result = (-1 * args[0].value.num_value);
	}
	return Expression(result);
}

Expression multiply_exp(const std::vector<Atom>& args) {
	for (auto& a : args) {
		if (a.type != NumberType) {
			throw InterpreterSemanticError("Error in call to multiply, argument not a number");
		}
	}
	Number first = args[0].value.num_value;
	Number second = args[1].value.num_value;
	Number result = (first * second);
	return Expression(result);
}

Expression divide_exp(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error in call to divide, wrong number of arguments");
	}
	for (auto& a : args) {
		if (a.type != NumberType) {
			throw InterpreterSemanticError("Error in call to divide, argument not a number");
		}
	}
	Number first = args[0].value.num_value;
	Number second = args[1].value.num_value;
	if (second == 0) {
		throw InterpreterSemanticError("Error in call to divide, argument not a number");
	}
	Number result = (first / second);
	return Expression(result);
}

Expression pow_exp(const std::vector<Atom>& args) {
	if (args.size() != 2) {
		throw InterpreterSemanticError("Error in call to pow, wrong number of arguments");
	}
	for (auto& a : args) {
		if (a.type != NumberType) {
			throw InterpreterSemanticError("Error in call to pow, argument not a number");
		}
	}

	Number base = args[0].value.num_value;
	Number exponent = args[1].value.num_value;
	Number result = std::pow(base, exponent);
	return Expression(result);
}


Expression log10_exp(const std::vector<Atom>& args) {
	Number result = 0;
	if (args.size() != 1) {
		throw InterpreterSemanticError("Error in call to log10, too many arguments");
	}
	for (auto& a : args) {
		if (a.type != NumberType) {
			throw InterpreterSemanticError("Error in call to log10, argument not a number");
		}
	}
	result = log10(args[0].value.num_value);
	return Expression(result);

}

void Environment::expToEnvmap(const std::string input, const Expression exp) {
	envmap[input];
	envmap[input].type = ExpressionType;
	envmap[input].exp = exp;
}


void Environment::reset() {
	// for Procedure "add" case:

	envmap["not"];
	envmap["not"].type = ProcedureType;
	envmap["not"].proc = not_exp;

	envmap["and"];
	envmap["and"].type = ProcedureType;
	envmap["and"].proc = and_exp;

	envmap["or"];
	envmap["or"].type = ProcedureType;
	envmap["or"].proc = or_exp;


	envmap["<"];
	envmap["<"].type = ProcedureType;
	envmap["<"].proc = lessthan;

	envmap["<="];
	envmap["<="].type = ProcedureType;
	envmap["<="].proc = lessthanequal;

	envmap[">"];
	envmap[">"].type = ProcedureType;
	envmap[">"].proc = greaterthan;

	envmap[">="];
	envmap[">="].type = ProcedureType;
	envmap[">="].proc = greaterthanequal;

	envmap["="];
	envmap["="].type = ProcedureType;
	envmap["="].proc = equal_exp;

	envmap["+"];
	envmap["+"].type = ProcedureType;
	envmap["+"].proc = add_exp;

	envmap["-"];
	envmap["-"].type = ProcedureType;
	envmap["-"].proc = minusorneg;

	envmap["*"];
	envmap["*"].type = ProcedureType;
	envmap["*"].proc = multiply_exp;

	envmap["/"];
	envmap["/"].type = ProcedureType;
	envmap["/"].proc = divide_exp;

	envmap["pow"];
	envmap["pow"].type = ProcedureType;
	envmap["pow"].proc = pow_exp;

	envmap["log10"];
	envmap["log10"].type = ProcedureType;
	envmap["log10"].proc = log10_exp;

	envmap["log10"];
	envmap["log10"].type = ProcedureType;
	envmap["log10"].proc = log10_exp;
}

Environment::Environment() {
	reset();
}