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
    unsigned int last_nonterminal_id = 0;
    bool nonterminal_exists(std::string obj) // -> checks the nonterminal_name_stack to see if that nonterminal exists
    {
        for ( unsigned int xx = 0; xx < nonterminal_name_stack.size(); xx++)
        {
            //std::cout << obj << std::endl;
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
        bool finished = false;
        unsigned int this_iter = 0;
        unsigned int new_pointer_pos_t = nonterminal_set.pointer_pos+1;
        // set apropriate lookahead symbol for each single nonterminal set
        if (nonterminal_exists(nonterminal_set.derive))
        {
            this_iter++;
        }
        for (unsigned int i = 0; i < Lookahead_list.size(); i ++)
        {
            for ( auto const& table : Lookahead_list[i])
            {
                if (table.first == nonterminal_set.derive)
                {
                    if ('0' + new_pointer_pos_t == (int)table.second.back())
                    {
                        temp_lookahead.push_back(table.second);
                    }
                }
            }
        }
        if ( nonterminal_exists(nonterminal_set.derive) == false ) // checks if the nonterminal is already 'registered'
        {
            nonterminal_id++;
            nonterminal_name_stack.push_back(nonterminal_set.derive);
            this_iter = 0;
        }

        for (unsigned int i = 0; i < nonterminal_set.production.size(); i++)
        {
            if (nonterminal_set.production[i].type == Token::nonterminal && nonterminal_set.derive != nonterminal_set.production[i].value)
            {
                bool found = false;
                std::string nonterminal_value = nonterminal_set.production[i].value;

                for (unsigned int a = 0; a < CC_list.size(); a++)
                {
                    if ( CC_list[a].derive == nonterminal_value)
                    {
                        found = true;
                        bool appendable = false;
                        unsigned int _iter_ = 0;
                        unsigned int temp_nonterminal_id = nonterminal_id;
                        std::vector<std::string>temp_temp_lookahead; // note: shitty name

                        for (unsigned int i = 0; i < Lookahead_list.size(); i++)
                                {
                                    for ( auto const& table : Lookahead_list[i] )
                                    {
                                        if (table.first ==  CC_list[a].derive)
                                        {
                                                if ((int)table.second.back() == '0' + new_pointer_pos_t)
                                                {
                                                _iter_++;
                                                temp_temp_lookahead.push_back(table.second);
                                                appendable = true;
                                                }
                                        }

                                    }
                                }
                        std::cout << last_nonterminal_id << std::endl;
                        LR1Item temp1(CC_list[a].derive, CC_list[a].production, ":list", nonterminal_id, CC_list[a].pointer_pos+1);
                        temp1.add_lookahead_list(temp_temp_lookahead);

                        temp_temp_lookahead.clear();
                        if (appendable)
                        {
                        Temp_CC_list.push_back(temp1);
                        }
                    }
                    if ( not found ) ;
                        report_error();
                }

            }
        }
        unsigned int new_pointer_pos = nonterminal_set.pointer_pos + 1 ;
        LR1Item temp(nonterminal_set.derive, nonterminal_set.production, ":list", nonterminal_id, new_pointer_pos);
        temp.add_lookahead_list(temp_lookahead);

        Temp_CC_list.push_back(temp);
        last_nonterminal_id = nonterminal_id;
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
        // debug
        for  (unsigned int vvx = 0; vvx < Temp_CC_list.size(); vvx++)
        {
            CC_list.push_back(Temp_CC_list[vvx]);
        }
        for ( unsigned int xy = 0; xy < Lookahead_list.size(); xy++)
        {
            for (auto const& table: Lookahead_list[xy])
            {
                std::cout << table.first << "=" << table.second << std::endl;
            }
        }
        std::cout << "\n";
        for ( unsigned int xx = 0; xx < CC_list.size(); xx++ )
        {
            std::cout << "parent = " << CC_list[xx].derive << std::endl;
            std::cout << "id = " << CC_list[xx].list_id << std::endl;
            std::cout << "pointer pos = " << CC_list[xx].pointer_pos << std::endl;
            if (CC_list[xx].look_ahead == ":list")
            {
                std::cout << "lookahead list = \n";
                for (unsigned int l = 0; l < CC_list[xx].look_ahead_list.size(); l++)
                {
                    std::cout << CC_list[xx].look_ahead_list[l] << std::endl;
                }
            }
            else
            {
                std::cout << "lookahead symbol = " << CC_list[xx].look_ahead << std::endl;
            }
            std::cout << "productions = \n";
            for (unsigned int p = 0; p < CC_list[xx].production.size(); p++)
            {
                std::cout << CC_list[xx].production[p].value  << std::endl;
            }
            std::cout << "\n" << std::endl;


        }
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
        unsigned int index_numb = 1;
        for (pos; pos < stack.size(); pos++)
        {
            for (unsigned int production_pos = 0; production_pos < stack[pos].productions.size(); production_pos++)
            {
                if (stack[pos].productions[production_pos].type == Token::terminal && not run) {
                    run = true;
                    std::string index_numb_str = std::to_string(index_numb);
                    std::map<std::string, std::string> temp_list =
                    {
                        {stack[pos].name, stack[pos].productions[production_pos].value+" "+index_numb_str}
                    };
                    Lookahead_list.push_back(temp_list); 
                    if (nonterminal_exists(stack[pos].name))
                         index_numb++;
                    else{
                        nonterminal_name_stack.push_back(stack[pos].name);
                        index_numb++;
                    }
                }
                do
                {
                    if (stack[pos].productions[production_pos+1].value == "|" || stack[pos].productions[production_pos+1].type == Token::end) 
                    {
                        index_numb = 1;
                    }
                    if (stack[pos].productions[production_pos].type == Token::terminal)
                        run = false;
                    if (stack[pos].productions[production_pos].value == "|")
                        run=false;
                    else if (stack[pos].productions[production_pos].type == Token::end) {
                        run=false;
                    }
                    else
                        production_pos++;
                } while (run);
            }
        }
        nonterminal_name_stack.clear();
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
    std::vector<LR1Item>Temp_CC_list; // provides temporary shelter for the CC_list's new members such as CC1 CC2 etc..
    unsigned int pos = 0;

};
