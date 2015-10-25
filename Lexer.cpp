//
//  Lexer.cpp
//  DevilScript
//
//  Created by Didrik Munther on 23/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <sstream>
#include <iostream>

#include "Lexer.h"


Lexer::Lexer() {
    keywords = {
                {"=", Tokens::t_equals},
                {"+", Tokens::t_plus},
                {"-", Tokens::t_minus},
                {"*", Tokens::t_multi},
                {"/", Tokens::t_divide},
                {"++", Tokens::t_plus_tt},
                {"--", Tokens::t_minus_tt},
               };
}

std::vector<std::pair<Tokens, std::string>> Lexer::lex(std::string input) {
    
    std::vector<std::string> splittedText;
    
    std::stringstream ss(input);
    std::string item;
    const char* delim = " ";
    while (std::getline(ss, item, *delim)) {
        if(item == "")
            continue;
        
        splittedText.push_back(item);
    }
    
    std::vector<std::pair<Tokens, std::string>> tokens;
    
    if(input.find_first_not_of(' ') == std::string::npos) // if only spaces
        tokens.push_back(std::make_pair(t_empty, ""));
    else {
        for(auto& i: splittedText) {
            if(hasToken(i))
                tokens.push_back(std::make_pair(keywords[i], i));
            
            else if(tokenIsName(i))
                tokens.push_back(std::make_pair(t_name, i));
            
            else if(tokenIsNumeral(i))
                tokens.push_back(std::make_pair(t_numeral, i));
            
            else
                tokens.push_back(std::make_pair(t_name_error, i));
        }
    }
    tokens.push_back(std::make_pair(t_end, "\n"));
    
    return tokens;
    
}

bool Lexer::hasToken(std::string token) {
    return keywords.find(token) != keywords.end();
}

bool Lexer::tokenIsName(std::string token) {
    std::string allowedCharacters = "_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
    
    for(auto& i: token)
        if(allowedCharacters.find(i) == std::string::npos)
            return false;
    
    return true;
}

bool Lexer::tokenIsNumeral(std::string token) {
    std::string numerals = "1234567890.";
    
    for(auto& i: token)
        if(numerals.find(i) == std::string::npos)
            return false;
    
    return true;
}



