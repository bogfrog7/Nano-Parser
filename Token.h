#include <iostream>
#pragma once
class Token
{
public:
	Token()
	{
		
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
		root_nonterminal,
		end,
		or_,
		pointer,
		derives, // -> 
		lookahead_symbol

	};

	std::string value ; unsigned int type ;
};
