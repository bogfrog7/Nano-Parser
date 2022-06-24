#include <iostream>
#include <vector>
#include "Token.h"

class LR1Item
{
    public:
    LR1Item(Token derived,std::vector<Token>p_production, Token p_look_ahead, unsigned int Label)
    {
        production = p_production;
        look_ahead = p_look_ahead;

    }

    enum Labels 
    {
        CC0, 
    };

    std::vector<Token>production;
    Token look_ahead;


};

