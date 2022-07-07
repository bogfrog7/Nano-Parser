#pragma once
#include <iostream>

const char* c_error_types[] =
{
	"[404C]",

};

enum ErrorTypes 
{
	COMMAND_NOT_FOUND, 
	TERMINAL_COMMAND_NOT_FOUND,
	END_SYNTAX_ERROR,
	SYNTAX_ERROR,
};

void raise_error(unsigned int type, std::string args, unsigned int line, std::string origin)
{
	std::cout <<
		 origin << " \n" <<
		"|\n" <<
		"| " << args << "\n"
		"|\n" <<
		"Error " << c_error_types[type] << " occured on line " << line << std::endl;

}
