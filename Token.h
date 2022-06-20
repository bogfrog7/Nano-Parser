#include <iostream>
#pragma once
class Token 
{
public:
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
		command // some command
	};
	std::string value ; unsigned int type ;
};