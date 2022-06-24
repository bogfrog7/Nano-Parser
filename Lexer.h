#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Token.h"
#include "Root Nonterminal.h"
const char* Token_Types[] =
{
	"nonterminal", // <word>
	"terminal", // word
	"equal", // =
	"undefined", //
	"command"
};

class Lexer
{

public:
	Lexer(const std::string &path)
	{
		data = get_data(path);
	}

	std::vector<RootNonterminal> get_source()
	{
		return stack;
	}

	bool execute_command(std::string command)
	{
		std::string new_command;
		for (int i = 0; i < command.size(); i++)
		{
			if (command[i] != ' ' && command[i] != '\r' && command[i] != '\n' && command[i] != '\t' && (int)command[i] != 0)
				new_command = new_command + command[i];
		}
		if (new_command == "print->nano.terminals")
		{
			for (unsigned int i = 0; i < Tokens.size(); i++)
			{
				if (Tokens[i].type == Token::terminal)
					std::cout << Tokens[i].value << std::endl;
			}
		}
		else if (new_command == "print->nano.nonterminals")
		{
			for (unsigned int i = 0; i < Tokens.size(); i++)
			{
				if (Tokens[i].type == Token::nonterminal)
					std::cout << Tokens[i].value << std::endl;
			}
		}
		else if (new_command == "print->nano.symbols")
		{
			for (unsigned int i = 0; i < Tokens.size(); i++)
			{
				std::cout << Tokens[i].value << std::endl;
			}
		}
		else if (new_command == "end")
			return true;
		else {
			return false;
		}

	}
	void start()
	{
		std::string nonterminal_word;
		std::string terminal_word;
		std::string command_word;
		bool root_nonterminal = false;
		bool lex_command = false;
		unsigned int nonterminal_pos = 0;
		unsigned int command_pos = 0;
		for (pos; pos < data.size(); pos++)
		{
			do
			{
				nonterminal_word = nonterminal_word + data[nonterminal_pos];
				nonterminal_pos++;

				if (data[nonterminal_pos] == '>')
				{
					nonterminal_word = nonterminal_word + ">";
					Tokens.push_back(Token(nonterminal_word, Token::nonterminal));
					pos = nonterminal_pos+1;
					nonterminal = false;
					break;
				}

			}while (nonterminal);

			if (data[pos] == '<') {
				nonterminal_word = data[pos];
				nonterminal = true;
				nonterminal_pos = pos + 1;
			}
			else if (data[pos] == ':' && pos < (data.size() - 2))
			{
				if (data[pos + 1] == ':' && data[pos + 2] == '=')
					Tokens.push_back(Token("::=", Token::equal));
			}
			else if (data[pos] == '\'' && pos < (data.size() - 3))
			{
				if (data[pos + 2] == '\'')
					Tokens.push_back(Token(std::string(1, data[pos + 1]), Token::terminal));
				pos = pos + 3;
			}
			else if (data[pos] == '-' && pos < (data.size() - 2))
			{
				if (data[pos + 1] == '-')
				{
					lex_command = true;
					command_pos = pos+2;
					do
					{
						command_word = command_word + data[command_pos];
						command_pos++;
						if (data[command_pos] == '\n' || command_pos == (data.size() - 1))
						{
							if (command_pos == (data.size() -1 ))
								command_word = command_word + data[command_pos];
							execute_command(command_word);
							Tokens.push_back(Token(command_word, Token::command));

							command_word.clear();
							pos = command_pos;
							lex_command = false;
						}

					} while (lex_command);
				}
				else
				{
					Tokens.push_back(Token("-", Token::terminal));
				}
			}
			else
				if (data[pos] != '\r' && data[pos] != '\n' && data[pos] != '\t' && data[pos] != ' ')
				{
					terminal_word = terminal_word + data[pos];
				}
				if (pos == (data.size()))
					terminal_word = terminal_word + data[pos];
				else
				{
					if ((int)terminal_word[0] != 0)
					{
						Tokens.push_back(Token(terminal_word, Token::terminal));
					}
					terminal_word.clear();
				}
		}
		for (pos = 0; pos < Tokens.size(); pos++)
		{
			if (Tokens[pos].type == Token::nonterminal && root_nonterminal == false)
			{
				if (pos < (Tokens.size() - 1) && Tokens[pos + 1].type == Token::equal)
				{
					unsigned int root_pos = pos + 2;
					root_nonterminal = true;
					RootNonterminal x(std::vector<Token>(), Tokens[pos].value);
					x.productions.push_back(Token(Tokens[pos].value, Token::root_nonterminal));
					x.productions.push_back(Token("::=", Token::equal));
					do
					{
						root_pos++;
						if (Tokens[root_pos].type == Token::terminal || Tokens[root_pos].type == Token::nonterminal)
							x.productions.push_back(Tokens[root_pos]);
						if (Tokens[root_pos].type == Token::command)
						{
							if (execute_command(Tokens[root_pos].value)) {
								x.productions.push_back(Token("end", Token::end)); 
								root_nonterminal = false;
									break;
							}
						}
					} while (root_nonterminal);
					stack.push_back(x);
				}

			}
		}
		// debugging
		/*for (unsigned int i = 0; i < stack.size(); i++)
		{
			std::cout << stack[i].name << std::endl; std::cout << "\n";
			for (unsigned int b = 0; b < stack[i].productions.size(); b++)
			{
				std::cout << stack[i].productions[b].value << std::endl;
			}
			}
		*/ 
	}
private:
	std::string data;
	unsigned int pos = 0;
	bool nonterminal = false;
	bool terminal = false;
	std::vector<Token>Tokens;
	std::vector<RootNonterminal>stack;
	std::string get_data(const std::string& path)
	{
		std::ifstream input_file(path);
		if (!input_file.is_open()) {
			std::cerr << "Could not open the file - '"
				<< path << "'" << std::endl;
			exit(EXIT_FAILURE);
		}
		return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
	}
};
