#include <iostream>
#include <vector>
#include "Token.h"
#include <string>

class LR1Item
{
    public:
        LR1Item(std::string derive_token, std::vector<Token>p_production, std::string p_look_ahead, unsigned int Label, unsigned int pos)
    {
        production = p_production;
        look_ahead = p_look_ahead;
        derive = derive_token;
        pointer_pos = pos;
        list_id = Label;

    }

    void add_lookahead_list(std::vector<std::string>p_look_ahead_list)
    {
        look_ahead_list = p_look_ahead_list;
    }

    unsigned int pointer_pos;
    unsigned int list_id;
    std::vector<Token>production;
    std::string look_ahead;
    std::string derive;
    std::vector<std::string>look_ahead_list;


};

