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
// TODO One production is not beign added to the CC list 
static unsigned int nonterminal_id = 0; // used for both iterations
static unsigned int last_nonterminal_id = 0; // used for both iterations
static unsigned int last_pointer_pos = 0; // used for the second iteration
static unsigned int pointer_pos_counter = 0; // used for the second iteration

class Parser
{
public:
    Parser(std::vector<RootNonterminal>p_source)
        :stack(p_source)
    {

    }
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
    void build_nonterminal(LR1Item nonterminal_set, bool from_source)
    {
        /* Explanation:
         * Consider the following nonterminal set a->*b | * [ b ] with a pointer pos of some signed integer w, ( represented as * )
         * Since the pointer pos is w, we need to increment the w so that it is exactly w+1 and we need to assign a 'label' which is
         * also a signed integer that represents the CC(I) list format. The label will be assigned to the collection. The collection
         * is currently [a->*b,s] and [[ *b,s]] (two differnet collections with the same label). For each production P, If P containts a nonterminal S with a lookahead symbol s
         * then all of S's productions will be cast into collections and labeled with the same label.
         *
         * Example:
         * input : build_nonterminal(<c>::=*<a>|[ ]) (<a> ::= *b)
         * output: (<c>::=<a>*, label=1, lookahead=b), (<a> ::= b*, label=1, lookahead=b), [<c>::= [* ], label=1, lookahead = [ )
         * 
         * Params:
         * from_source -> if true, builds from existing nonterminal_id paramater such as last_nonterminal_id
         * nonterminal_set -> nonterminal_set::LR1Item
         * */
        std::vector<std::string>temp_lookahead; // temporary lookahead stack
        bool finished = false;
        bool found_lookahead = false;
        unsigned int this_iter = 0;
        unsigned int new_pointer_pos_t = nonterminal_set.pointer_pos+1;
        if (from_source) {
            new_pointer_pos_t = pointer_pos_counter+1;
            nonterminal_set.pointer_pos = pointer_pos_counter;
            pointer_pos_counter++;
        }
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
                        found_lookahead = true;
                    }
                }
            }
        }
        if ( nonterminal_exists(nonterminal_set.derive) == false) // checks if the nonterminal is already 'registered'
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
                size_t _iter_ = 0;
                std::string nonterminal_value = nonterminal_set.production[i].value;

                for (unsigned int a = 0; a < CC_list.size(); a++)
                {
                    if ( CC_list[a].derive == nonterminal_value)
                    {
                        found = true;
                        bool appendable = false;
                        _iter_ = 0;
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
                        LR1Item temp1(nonterminal_set.derive, CC_list[a].production, ":list", nonterminal_id, CC_list[a].pointer_pos+1);
                        temp1.add_lookahead_list(temp_temp_lookahead);

                        temp_temp_lookahead.clear();
                        if (appendable)
                        {
                        Temp_CC_list.push_back(temp1);
                        }
                    }
                    if (not found)
                    {
                    }
                }

            }
        }
        unsigned int new_pointer_pos = nonterminal_set.pointer_pos + 1 ;
        LR1Item temp(nonterminal_set.derive, nonterminal_set.production, ":list", nonterminal_id, new_pointer_pos);
        temp.add_lookahead_list(temp_lookahead);
        Temp_CC_list.push_back(temp);
        last_nonterminal_id = nonterminal_id;
        if (from_source) { nonterminal_id++;}
        if (not from_source) { pointer_pos_counter = nonterminal_set.pointer_pos + 1; }
        last_pointer_pos = pointer_pos_counter;

    }
    void goto_()
    {
        for (unsigned int i = 0; i < CC_list.size(); i++)
        {
            build_nonterminal(CC_list[i], false);
        }
    }
    void goto_2()
    {
        bool running = false; 
        for (unsigned int x = 0; x < CC_list.size(); x++)
        {
            if (CC_list[x].list_id > 0)
            {
                running = true;
                unsigned int temp_pointer_pos = CC_list[x].pointer_pos;
                do 
                {
                    if (temp_pointer_pos == (CC_list[x].production.size() -1))
                        running = false;
                    build_nonterminal(CC_list[x], true);
                    temp_pointer_pos++;
                } while (running);
            }
            
        }
    }
    void build_CC_list()
    {
        goto_();
        // debug
        for (unsigned int x = 0; x < Temp_CC_list.size(); x++)
        {
            for (unsigned int y = 0; y < Temp_CC_list[x].production.size(); y++)
            {
                if (Temp_CC_list[x].production[y].value == "<emp>")
                {
                    Temp_CC_list[x].production[y].value = "";
                }
            }
        }
        for  (unsigned int vvx = 0; vvx < Temp_CC_list.size(); vvx++)
        {
            CC_list.push_back(Temp_CC_list[vvx]);
        }
        Temp_CC_list.clear(); 
        nonterminal_name_stack.clear();
        nonterminal_id = last_nonterminal_id;
        goto_2();
        for (unsigned int vvx = 0; vvx < Temp_CC_list.size(); vvx++)
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
                    if (stack[pos].productions[production_pos].value == "|") production_pos++;
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
                    if (production_pos != (stack[pos].productions.size() - 1))
                    {
                        if (stack[pos].productions[production_pos + 1].value == "|" || stack[pos].productions[production_pos + 1].type == Token::end)
                        {
                            index_numb = 1;
                        }
                    }
                    if (stack[pos].productions[production_pos].type == Token::terminal) {
                        run = false;
                    }
                    if (stack[pos].productions[production_pos].value == "|") {
                        run = false;
                    }
                    else if (stack[pos].productions[production_pos].type == Token::end) {
                        run = false;
                    }
                    else;
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
                            if (iter == Lookahead_list.size())
                                iter = iter - 1;
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
                            if (iter == Lookahead_list.size())
                                iter = iter - 1;
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
        build_CC_list();
    }

private:
    std::vector<std::string>nonterminal_name_stack;
    std::vector<RootNonterminal>stack;
    std::vector<std::map<std::string, std::string>>Lookahead_list;
    std::vector<std::string> FoundSymbols;
    std::vector<unsigned int> FoundSymbols_id;
    std::vector<LR1Item>CC_list; 
    std::vector<LR1Item>Temp_CC_list; // provides temporary shelter for the CC_list's new members such as CC1 CC2 CC3 etc..

    unsigned int pos = 0;

};
