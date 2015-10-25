//
//  Lexer.cpp
//  DevilScript
//
//  Created by Didrik Munther on 23/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <sstream>
#include <iostream>
#include <algorithm>

#include "Lexer.h"


Lexer::Lexer() {
    keywords = {
                {"=", t_equals},
                {"+", t_plus},
                {"-", t_minus},
                {"*", t_multi},
                {"**", t_raised},
                {"/", t_div},
                {"(", t_openpar},
                {")", t_closedpar}
               };
}

std::vector<std::pair<Tokens, std::string>> Lexer::lex(std::string input) {
    
    std::vector<std::string> allKeywords;
    for(auto& i: keywords)
        allKeywords.push_back(i.first);
    
    std::vector<std::string> splittedText;
    
    std::stringstream ss(input);
    std::string item;
    const char* delim = " ";
    
    while (std::getline(ss, item, *delim)) {
        if(item == "")
            continue;
        
        bool concatenated = false;
        for(auto& i: allKeywords) {
            auto pos = item.find(i);
            if(pos != std::string::npos) {
                std::cout << "here" << "\n";
                concatenated = true;
                
                item.insert(pos, " ");
                item.insert(pos + 2, " ");
            }
            
            std::stringstream operatorsInString(item);
            
            while(std::getline(<#basic_istream<_CharT, _Traits> &__is#>, <#basic_string<_CharT, _Traits, _Allocator> &__str#>, <#_CharT __dlm#>))
        }
        
        if(concatenated) {
            std::stringstream ss2(item);
            std::string item2;
            
            while(std::getline(ss2, item2, *delim)) {
                if(item2 == "")
                    continue;
                
                item2.erase(std::remove_if(item2.begin(), item2.end(), isspace), item2.end());
                splittedText.push_back(item2);
            }
            
            continue;
        }
        
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
    std::string numerals = "1234567890.-+";
    
    for(auto& i: token)
        if(numerals.find(i) == std::string::npos)
            return false;
    
    return true;
}



