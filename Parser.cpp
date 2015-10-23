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
    expected = {t_name, t_numeral, t_equals};
    Tokens currentOperator = t_end;
    bool equals = false;
    std::string currentVariable = "";
    
    for(auto& i: tokens) {
        
        if(!isExpected(i, lexer))
            return error(stack);
        
        switch(i.first) {
                
            case t_name:
                if(currentVariable == "" && tokens[1].first == t_equals)
                    currentVariable = i.second;
                else if(!stack->hasVariable(i.second))
                    return "Error: Variable not yet instantiated: " + i.second;
                    
                stack->pushElement(new Element(stack->variables[i.second.c_str()]));
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
                
            case t_empty:
                stack->clearStack();
                return "";
                break;
                
            case t_end:
                break;
                
            case t_name_error:
                std::cout << "Error at lexing: " << i.second;
                return error(stack);
                break;
                
            default:
                std::cout << "Error: No such command (" << i.first << ", \"" << i.second << "\")";
                return error(stack);
                break;
        }
        
//        if(previousVariable != "" && !stack->hasVariable(previousVariable) && currentOperator != t_equals) {
//            previousVariable = "";
//            //return "Error (2): Variable not yet instantiated: " + i.second;
//        }
        
    }
    
    stack->variables[currentVariable.c_str()] = stack->toString(0);
    stack->popTopElement();
    stack->pushElement(new Element(stack->variables[currentVariable.c_str()]));
    
    auto toReturn = stack->toString(0);
    stack->clearStack();
    return toReturn;
}

void Parser::pushCalculatedElement(Element* element, Stack* stack) {
    stack->popTopElement();
    stack->popTopElement();
    stack->pushElement(element);
}

void Parser::pushAritmethic(Stack* stack,Tokens currentOperator) {
    int result = 0;
    
    if(currentOperator == t_plus) {
        int other = stack->toInt(-1);
        result = stack->toInt(0) + other;
        pushCalculatedElement(new Element(std::to_string(result)), stack);
    }
}

void Parser::expError(std::pair<Tokens, std::string> token, std::vector<Tokens> expTokens, Lexer* lexer) {
    std::cout << "Error: Did not expect keyword of type: " << token.first << " (\"" << token.second << "\")";
    
    std::string errorReason = "";
    
    switch(token.first) {
        case t_name_error:
            errorReason = "naming error";
            break;
            
        default:
            break;
    }
    
    errorReason = errorReason == "" ? "" : " \"" + errorReason + "\"";
    
    std::cout << errorReason << ", expected: ";
    
    for(auto& i: expTokens) {
        std::string keyword = "";
        for(auto& j: lexer->keywords) {
            if(j.second == i)
                keyword = j.first;
        }
        
        switch(i) {
            case t_name:
                keyword = "name";
                break;
                
            case t_numeral:
                keyword = "numeral";
                break;
                
            default:
                break;
        }
        
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
    else if(token.first == t_end || token.first == t_empty)
        return true;
    else if(hasToken(token.first, expected))
        return true;
    
    expError(token, expected, lexer);
    return false;
}