#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Root Nonterminal.h"
#include "Token.h"
#include "LR1Item.h"
#include <map>


/* 
 *
 *
 * */

class Parser
{
public:
    Parser(std::vector<RootNonterminal>p_source)
        :stack(p_source)
    {

    }
    unsigned int nonterminal_id = 0;

    bool nonterminal_exists(std::string obj) // -> checks the nonterminal_name_stack to see if that nonterminal exists
    {
        for ( unsigned int xx = 0; xx < nonterminal_name_stack.size();)
        {
            if (obj == nonterminal_name_stack[xx])
            {
                return true;
            }
        }
        return false;

    }
    void build_nonterminal(LR1Item nonterminal_set)
    {
        std::vector<std::string>temp_lookahead; // temporary lookahead stack
        // set apropriate lookahead symbol for each single nonterminal set
        for (unsigned int i = 0; i < Lookahead_list.size(); i ++)
        {
            for ( auto const& table : Lookahead_list[i] )
            {
                if ( table.first ==  nonterminal_set.look_ahead)
                {
                    temp_lookahead.push_back(table.second);
                }

            }
        }

        for (unsigned int i = 0; i < nonterminal_set.production.size(); i ++)
        {
            if ( nonterminal_set.production[i].type == Token::nonterminal && nonterminal_set.derive != nonterminal_set.production[i].value)
            {
                // look it up
            }
        }
        unsigned int new_pointer_pos = nonterminal_set.pointer_pos + 1 ;
        if (not nonterminal_exists(nonterminal_set.derive)){
            nonterminal_id++;
            nonterminal_name_stack.push_back(nonterminal_set.derive);
        }
        LR1Item temp(nonterminal_set.derive, nonterminal_set.production, ":list", nonterminal_id, new_pointer_pos);
        temp.add_lookahead_list(temp_lookahead);

    }
    void goto_()
    {
        for (unsigned int i = 0; i < CC_list.size(); i++)
        {
            build_nonterminal(CC_list[i]);
        }
    }
    void build_CC_list()
    {
        goto_();
    }
    bool check_symbol(std::string symbol,unsigned int id_)
    {

        for (unsigned int c = 0; c < FoundSymbols.size(); c++)
        {
            if (symbol == FoundSymbols[c] && id_ == FoundSymbols_id[c])
                return true;
        }
        return false ;
    }
    void find_lookahead_symbol()
    {
        
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
    bool is_not_space(char character)
    {
        switch (character)
        {
        case '\r':
        case '\t':
        case ' ':
        case '\n':
            return true;
        default:
            return false;
        }
    }
    void run()
    {
        
        find_lookahead_symbol(); 
        bool production_parse = false;
        bool finished_root_parsing = true;
        unsigned int iter = 0;



        for (pos; pos < stack.size(); pos++ )
        {
            for (unsigned int x = 0; x < stack[pos].productions.size(); x++)
            {
                
                if (stack[pos].productions[x].type == Token::root_nonterminal && stack[pos].productions[x+1].type == Token::equal || production_parse)
                {
                    production_parse = true;
                    unsigned int root_pos = x;

                    std::vector<Token>temp_production_stack;
                    std::string derive_token = stack[pos].name;
                    std::string lookahead_symbol;
                    temp_production_stack.push_back(Token("*", Token::pointer));
                    do
                    {
                        root_pos++ ;

                        if (stack[pos].productions[root_pos].value != "|" && stack[pos].productions[root_pos].type != Token::end && stack[pos].productions[root_pos].type != Token::equal){
                            if (is_not_space(stack[pos].productions[root_pos].value[0]) == false)
                                temp_production_stack.push_back(stack[pos].productions[root_pos]);  
                        }

                        if (stack[pos].productions[root_pos].value == "|")
                        {
                            for (auto const& x : Lookahead_list[iter])
                            {
                                lookahead_symbol = x.second;
                            }
                            iter++;
                            CC_list.push_back(LR1Item(derive_token, temp_production_stack, lookahead_symbol, 0, 0));
                            temp_production_stack.clear();
                            temp_production_stack.push_back(Token("*", Token::pointer));
                            
                            
                        }
                        if (stack[pos].productions[root_pos].type == Token::end)
                        {
                            for (auto const& x : Lookahead_list[iter])
                            {
                                lookahead_symbol = x.second;
                            }
                            iter++;
                            CC_list.push_back(LR1Item(derive_token, temp_production_stack, lookahead_symbol, 0, 0));
                            production_parse = false;
                        }

                    }
                    while (production_parse);
                }
            }
        }
       /* for (int i = 0; i < CC_list.size(); i++)
        {
           std::cout << "LOOKAHEAD = " << CC_list[i].look_ahead << std::endl;

            for (int x = 0; x < CC_list[i].production.size(); x++) {
                std::cout << CC_list[i].derive << CC_list[i].production[x].value << std::endl;

                
            }
            std::cout << "\n";
        } */
        build_CC_list(); 
    }

private:
    std::vector<std::string>nonterminal_name_stack;
    std::vector<RootNonterminal>stack;
    std::vector<std::map<std::string, std::string>>Lookahead_list;
    std::vector<std::string> FoundSymbols;
    std::vector<unsigned int> FoundSymbols_id;

    std::vector<LR1Item>CC_list; 
    unsigned int pos = 0;

};
