#pragma once
#include <iostream>
#include "Token.h"

/* The term 'Root Nonterminal' implies that the nonterminal of some production is the exact nonterminal on the right hand side of that same production or
* that the nonterminal references syntax definitions in that exact sentence. 
In other words that nonterminal contains syntax definitions that can be used in other productions and it is the same nonterminal providing those definitions in
that same exact sentence
*/

class RootNonterminal 
{
public:
	RootNonterminal(std::vector<Token>p_productions, std::string p_name)
	{
		productions = p_productions; 
		name = p_name;
	}
	std::string name;
	std::vector<Token>productions;
};