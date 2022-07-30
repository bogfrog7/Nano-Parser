#include <iostream>
#pragma once
class Token
{
public:
	Token()
	{
		value = "NULL"; type = NULL;
	}
	Token(std::string pvalue, unsigned int ptype)
	{
		value = pvalue;
		type = ptype;
	}

	enum types
	{
		nonterminal, // <word>
		terminal, // word
		equal, // =
		undefined, // `
		command, // some command
		root_nonterminal, // <root>
		end, // end
		or_, // |
		pointer, // *
		derives, // -> 
		lookahead_symbol
	};
	std::string value; unsigned int type;
};
