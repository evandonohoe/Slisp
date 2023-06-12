#include "expression.hpp"

#include <cmath>
#include <limits>
#include <cctype>
#include <regex>

// system includes
#include <sstream>

Expression::Expression(bool tf){
	head.type = BooleanType;
	head.value.bool_value = tf;
}

Expression::Expression(double num){
	head.type = NumberType;
	head.value.num_value = num;
}

Expression::Expression(const std::string & sym){
	head.type = SymbolType;
	head.value.sym_value = sym;
}

bool Expression::operator==(const Expression & exp) const noexcept{
	bool ret = false;
	if ((head == exp.head) && (tail == exp.tail)) {
		ret = true;
	}
	return ret;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp){
  // TODO: implement this function
	out << "(";

	int rounded_double = 0;
		switch (exp.head.type) {
		case BooleanType:
			if (exp.head.value.bool_value) {
				out << "True";
			}
			else {
				out << "False";
			}
			break;
		case NumberType:
			rounded_double = static_cast<int>(round(exp.head.value.num_value));
			out << std::to_string(rounded_double);
			break;
		case SymbolType:
			out << exp.head.value.sym_value;
			break;
		default:
			out << "NoneTypeVal";
		}

		for (auto& a : exp.tail) {
			out << " " << a;
		}

	out << ")";
	return out;
}

bool isValidNumber(const std::string& str) {
	std::regex numberRegex("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$");
	return std::regex_match(str, numberRegex);
}

bool isValidSymbol(const std::string& str) {
	std::regex symbolRegex("^[^0-9[:space:]].*$");
	return std::regex_match(str, symbolRegex);
}

bool token_to_atom(const std::string & token, Atom & atom){
	 // TODO: implement this function
	 // return true if it a token is valid. otherwise, return false.
	//c_str to turn 1e-4 to 0.0004 strtod stack overflow has the answer involving

	bool isNumber = isValidNumber(token);
	bool isBool = (token == "True" || token == "False");
	bool isSymbol = isValidSymbol(token);

	if (isBool) {
		atom.type = BooleanType;
		if (token == "True") {
			atom.value.bool_value = true;
		}
		else {
			atom.value.bool_value = false;
		}
	} else if (isNumber) {
		double NumValue = std::stof(token);
		atom.type = NumberType;
		atom.value.num_value = NumValue;
	} else if (isSymbol) {
		std::string SymValue = token;

		atom.type = SymbolType;
		atom.value.sym_value = SymValue;
		if (SymValue == "pi") {
			atom.type = NumberType;
			atom.value.num_value = atan2(0, -1);
		}
	}
	else {
		return false;
	}
	
	
  return true;
}

