//
//  Parser.cpp
//  DevilScript
//
//  Created by Didrik Munther on 23/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <fstream>

#include "Parser.h"


Parser::Parser() {
    
}

std::vector<std::string> Parser::parse(std::vector<std::pair<Tokens, std::string>> tokens, Stack* stack, Lexer* lexer) {
    expected = {t_name, t_numeral, t_equals, t_m_open_from_file, t_m_print};
    Tokens currentOperator = t_end;
    std::string currentVariable = "";
    
    auto i = tokens.begin();
    while(i != tokens.end()) {
        
        if(!isExpected(*i, lexer))
            return error(stack);
        
        switch(i->first) {
                
            case t_name:
                if(currentVariable == "" && tokens[1].first == t_equals)
                    currentVariable = i->second;
                else if(!stack->hasVariable(i->second))
                    return {"Error: Variable not yet instantiated: " + i->second};
                
                stack->pushElement(new Element(stack->variables[i->second.c_str()]));
                pushAritmethic(stack, currentOperator);
                
                setExpected(lexer->getArithmeticOperators());
                expected.push_back(t_equals);
                expected.push_back(t_plus_equal);
                
                break;
                
            case t_numeral:
                stack->pushElement(new Element(i->second));
                pushAritmethic(stack, currentOperator);
                setExpected(lexer->getArithmeticOperators());
                break;
                
            case t_div:
            case t_multi:
            case t_minus:
            case t_raised:
            case t_plus:
                currentOperator = i->first;
                
                setExpected({t_numeral, t_name, t_openpar});
                break;
                
            case t_equals:
                setExpected({t_name, t_numeral, t_openpar});
                break;
                
            case t_empty:
                stack->clearStack();
                return {""};
                break;
                
            case t_end:
                break;
                
            case t_openpar:
                break;
                
            case t_closedpar:
                break;
                
            case t_m_open_from_file:
            {
                std::string file;
                if((i + 2) == tokens.end()) {
                    std::cout << "[Open file]> ";
                    std::getline(std::cin, file);
                } else {
                    file = (i + 1)->second;
                }
                
                return openFromFile(file, stack, lexer);
            }
                break;
                
            case t_m_print:
            {
                if((i + 1) == tokens.end())
                    return {""};
                
                std::string toReturn = "";
                auto i2 = i + 1;
                while(i2 != tokens.end() - 1) {
                    toReturn += i2->second;
                    toReturn += " ";
                    i2++;
                }
                
                return {toReturn};
            }
                break;
                
            case t_name_error:
                std::cout << "Error at lexing: " << i->second;
                return error(stack);
                break;
                
            default:
                std::cout << "Error: No such command (" << i->first << ", \"" << i->second << "\")\n";
                return error(stack);
                break;
        }
        
        i++;
    }
    
    stack->variables[currentVariable.c_str()] = stack->toString(0);
    stack->popTopElement();
    stack->pushElement(new Element(stack->variables[currentVariable.c_str()]));
    
    auto toReturn = stack->toString(0);
    stack->clearStack();
    
    return {toReturn};
}

std::vector<std::string> Parser::openFromFile(std::string file, Stack* stack, Lexer* lexer) {
    std::ifstream ifile;
    std::string line;
    
    ifile.open(file);
    if(!ifile || !ifile.is_open()) {
        std::cout << "Error: could not open file: " << file << "\n";
        return {""};
    }
    
    std::vector<std::string> toReturn;
    while(std::getline(ifile, line)) {
        std::vector<std::string> output = parse(lexer->lex(line), stack, lexer);
        toReturn.insert(toReturn.end(), output.begin(), output.end());
    }
    
    ifile.close();
    
    return toReturn;
}

void Parser::pushCalculatedElement(Element* element, Stack* stack) {
    stack->popTopElement();
    stack->popTopElement();
    stack->pushElement(element);
}

void Parser::pushAritmethic(Stack* stack,Tokens currentOperator) {
    if(currentOperator == t_end) return;
    
    float result = INT_MIN;
    float a1 = stack->toFloat(-1);
    float a2 = stack->toFloat(0);
    
    switch(currentOperator) {
            
        case t_plus:
            result = a1 + a2;
            break;
            
        case t_minus:
            result = a1 - a2;
            break;
            
        case t_multi:
            result = a1 * a2;
            break;
            
        case t_raised:
            result = pow(a1, a2);
            break;
            
        case t_div:
            result = a1 / a2;
            break;
            
        default:
            break;
    }
    
    pushCalculatedElement(new Element(std::to_string(result)), stack);
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

std::vector<std::string> Parser::error(Stack* stack) {
    stack->clearStack();
    return {""};
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