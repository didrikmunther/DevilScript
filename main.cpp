//
//  main.cpp
//  DevilScript
//
//  Created by Didrik Munther on 23/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <iostream>
#include <vector>

#include "Lexer.h"
#include "Parser.h"
#include "Stack.h"


void executeLine(Lexer* lexer, Parser* parser, Stack* stack, std::string input) {
    
    auto output = parser->parse(lexer->lex(input), stack, lexer);
    
    for(auto& i: output)
        std::cout << i << (i == "" ? "" : "\n");
    
}

int main(int argc, const char * argv[]) {
    
    std::cout << "Started;\n";
    
    Lexer lexer;
    Parser parser;
    Stack stack;
    
    while(true) {
        std::cout << "> ";
        std::string input = "";
        std::getline(std::cin, input);
        if(input == "quit!") break;
        
        executeLine(&lexer, &parser, &stack, input);
    }
    
    return 0;
}
