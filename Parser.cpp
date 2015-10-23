//
//  Parser.cpp
//  DevilScript
//
//  Created by Didrik Munther on 23/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <iostream>

#include "Parser.h"


Parser::Parser() {
    
}

std::string Parser::parse(std::vector<std::pair<Tokens, std::string>> tokens, Stack* stack, Lexer* lexer) {
    
    //std::vector<Tokens> expTokens = {(Tokens)-1}; // Expect everything in the beginning
    expected = {t_name, t_numeral};
    Tokens currentOperator = t_end;
    bool equals = false;
    std::string currentVariable = "";
    
    for(auto& i: tokens) {
        
        if(!isExpected(i, lexer))
            return error(stack);
        
        switch(i.first) {
                
            case t_name:
                if(currentVariable == "")
                    currentVariable = i.second;
                else if(!stack->hasVariable(i.second))
                    return "Error: Variable not yet instantiated: " + i.second + "\n";
                    
                stack->pushElement(new Element(stack->variables[i.second]));
                pushAritmethic(stack, currentOperator);
                setExpected({t_equals, t_plus});
                
                break;
                
            case t_numeral:
                stack->pushElement(new Element(i.second));
                pushAritmethic(stack, currentOperator);
                setExpected({t_plus});
                
                break;
                
            case t_plus:
                currentOperator = t_plus;
                
                setExpected({t_numeral, t_name});
                break;
                
            case t_equals:
                equals = true;
                
                setExpected({t_name, t_numeral});
                break;
                
            case t_end:
                
                break;
                
            case t_err:
                std::cout << "Error at lexing: " << i.second << "\n";
                return error(stack);
                break;
                
            default:
                std::cout << "Error: No such command (" << i.first << ", \"" << i.second << "\")\n";
                return error(stack);
                break;
        }
        
    }
    
    if(equals) {
        stack->variables[currentVariable] = stack->toString(0);
        stack->popTopElement();
        stack->pushElement(new Element(stack->variables[currentVariable]));
    }
    
    auto toReturn = stack->toString(0);
    stack->clearStack();
    return toReturn;
}

void Parser::pushAritmethic(Stack* stack,Tokens currentOperator) {
    if(currentOperator == t_plus) {
        int other = stack->toInt(-1);
        int result = stack->toInt(0) + other;
        stack->popTopElement();
        stack->popTopElement();
        stack->pushElement(new Element(std::to_string(result)));
    }
}

void Parser::expError(std::pair<Tokens, std::string> token, std::vector<Tokens> expTokens, Lexer* lexer) {
    std::cout << "Error: Did not expect keyword of type: " << token.first << " (" << token.second << "), expected: ";
    
    for(auto& i: expTokens) {
        std::string keyword = "";
        for(auto& j: lexer->keywords) {
            if(j.second == i)
                keyword = j.first;
        }
        
        if(i == Tokens::t_name)
            keyword = "name";
        
        std::cout << i << " (" << keyword << "), ";
    }
    
    std::cout << "\n";
}

std::string Parser::error(Stack* stack) {
    stack->clearStack();
    return "";
}

bool Parser::hasToken(Tokens token, std::vector<Tokens> tokens) {
    return std::find(tokens.begin(), tokens.end(), token) != tokens.end();
}

bool Parser::isExpected(std::pair<Tokens, std::string> token, Lexer* lexer) {
    if(hasToken((Tokens)-1, expected)) // If everything is expected
        return true;
    else if(hasToken(t_end, expected))
        return true;
    else if(hasToken(token.first, expected))
        return true;
    
    expError(token, expected, lexer);
    return false;
}