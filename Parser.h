//
//  Parser.h
//  DevilScript
//
//  Created by Didrik Munther on 23/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __DevilScript__Parser__
#define __DevilScript__Parser__

#include <stdio.h>
#include <string>

#include "Lexer.h"
#include "Stack.h"


class Parser {
    
public:
    Parser();
    
    std::string parse(std::vector<std::pair<Tokens, std::string>> tokens, Stack* stack, Lexer* lexer);
    
    void expError(std::pair<Tokens, std::string> token, std::vector<Tokens> expTokens, Lexer* lexer);
    std::string error(Stack* stack);
    
    void pushAritmethic(Stack* stack, Tokens currentOperator);
    
    std::vector<Tokens> expected;
    
private:
    bool hasToken(Tokens token, std::vector<Tokens> tokens);
    bool isExpected(std::pair<Tokens, std::string> token, Lexer* lexer);
    
    void setExpected(std::vector<Tokens> expected) {
        this->expected = expected;
    }
    
};

#endif /* defined(__DevilScript__Parser__) */
