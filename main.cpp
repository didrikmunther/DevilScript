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

int programShell() {
    std::cout << "DevilScript started\n\n";
    
    
    Lexer lexer;
    Parser parser;
    Stack stack;
    
    while(true) {
        std::cout << "> ";
        std::string input = "";
        std::getline(std::cin, input);
        if(input == "quit!") return 0;
        if(input == "restart!") return 1;
        
        auto output = parser.parse(lexer.lex(input), &stack, &lexer);
        for(auto& i: output)
            std::cout << i << (i == " " ? "" : "\n");
    }
}

int executeProgram() {
    while(true) {   // A code of 1 means to restart
        int program = programShell();
        
        switch(program) {
            case 1:
                std::cout << "\n";
                break;
                
            default:
                return program;
        }
    }
}

int main(int argc, const char * argv[]) {
    return executeProgram();
}
