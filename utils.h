#pragma once
#include <iostream>
#include <vector>

class Error
{
	public:
	Error(unsigned int p_type, std::string p_args, unsigned int p_line, std::string p_origin)
	{
		type = p_type; args = p_args; line = p_line; origin = p_origin;

	}
	unsigned int type; std::string args; unsigned int line; std::string origin;

};

static std::vector<Error>Errors;

static unsigned int error_count = 0;

const char* c_error_types[] =
{
	"[404C]",

};

enum ErrorTypes
{
	COMMAND_NOT_FOUND,
	TERMINAL_COMMAND_NOT_FOUND,
	SYNTAX_ERROR,
	NONTERMINAL_NOT_FOUND
};

const char* ErrorNames[] =
{
	"Command Not Found",
	"Terminal Command Not Found",
	"Syntax Error",
	"Nonterminal Not Found"
};

void raise_error(unsigned int type, std::string args, unsigned int line, std::string origin)
{
	Errors.push_back(Error(type, args, line, origin));
}
void print_errors()
{
	if (error_count != 0)
	{
	std::cout << "Build Failed...\n";
	std::cout << ">>>>>>>>>>>>>>>\n";
	std::cout << "Details: " << error_count << " errors" << "and 0 warnings detected\n";
	std::cout << "Build Failed at <time>\n";
	std::cout << ">>>>>>>>>>>>>>>>\n";
	std::cout << "Errors Detected: -> \n";
	for (unsigned int i = 0; i < Errors.size(); i++)
	{
		std::cout << "nano:"<< Errors[i].origin <<": " << ErrorNames[Errors[i].type] << "occured on line " << Errors[i].line << std::endl;
		std::cout << "Details:" << Errors[i].args << std::endl;
	}
	}
}
