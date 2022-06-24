#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Root Nonterminal.h"
#include "Token.h"
#include "LR1Item.h"
#include <map>


/* Written by Bob.net Co. 6/21/2022 10:11 am
 * This parser is only able to parse Nano language
 *
 * */

class Parser
{
public:
    Parser(std::vector<RootNonterminal>p_source)
        :stack(p_source)
    {

    }
    void find_lookahead_symbol()
    {
        std::vector<std::map<std::string, std::string>>Lookahead_list;
        
        bool run = false; 
        for (pos; pos < stack.size(); pos++)
        {
            for (unsigned int production_pos = 0; production_pos < stack[pos].productions.size(); production_pos++)
            {
                if (stack[pos].productions[production_pos].type == Token::terminal && not run) {
                    run = true;
                    std::map<std::string, std::string> temp_list =
                    {
                        {stack[pos].name, stack[pos].productions[production_pos].value}
                    };
                    Lookahead_list.push_back(temp_list); 
                }
                do
                {
                    if (stack[pos].productions[production_pos].value == "|")
                        run = false;
                    else if (stack[pos].productions[production_pos].type == Token::end) {
                        run = false;
                    }
                    else
                        production_pos++;
                } while (run);
            }
        }
        pos = 0; // reset for next function (run) 
    }
    void run()
    {
        find_lookahead_symbol(); 
        bool production_parse = false;
        bool finished_root_parsing = true;


        for (pos; pos < stack.size(); pos++ )
        {
            for (unsigned int x = 0; x < stack[pos].productions.size(); x++)
            {
                if (stack[pos].productions[x].type == Token::root_nonterminal && stack[pos].productions[x+1].type == Token::equal)
                {
                    production_parse = true;
                    unsigned int root_pos = pos+1;

                    std::vector<Token>temp_production_stack;
                    Token derive_token = stack[pos].productions[x];
                    Token lookahead_symbol = Token();
                    temp_production_stack.push_back(Token("*", Token::pointer));

                    do
                    {
                        root_pos++ ;

                        if (stack[pos].productions[root_pos].value != "|" && stack[pos].productions[root_pos].type != Token::end){
                            temp_production_stack.push_back(stack[pos].productions[root_pos]);  
                        }

                        if (stack[pos].productions[root_pos].value == "|")
                        {
                            CC_list.push_back(LR1Item(derive_token, temp_production_stack, Token(), LR1Item::CC0));
                            temp_production_stack.clear();
                        }
                        if (stack[pos].productions[root_pos].type == Token::end)
                        {
                            x = 0;  
                            root_pos = 0;
                            production_parse = false;
                        }

                    }
                    while (production_parse);
                }
            }
        }
    }

private:
    std::vector<RootNonterminal>stack;
    std::vector<LR1Item>CC_list; 
    unsigned int pos = 0;

};
