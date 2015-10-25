//
//  main.cpp
//  DevilScript
//
//  Created by Didrik Munther on 23/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <iostream>
<<<<<<< HEAD
#include <fstream>
=======
#include <vector>

>>>>>>> Malaxiz/master
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
    
    std::ifstream file;
    std::string line;
    
    while(true) {
        std::cout << "> ";
        std::string input = "";
        std::getline(std::cin, input);
<<<<<<< HEAD
        
        if(input == "quit") break;
        
        if (input == "import") {
            std::cout << "[open]: ";
            std::getline(std::cin, input);
            
            file.open(input);
            
            if (!file) {
                std::cout << "[Error]: could not execute script" << "\n";
                continue;
            }
            
            if (file.is_open()) {
                std::cout << "[Success]: opened script" << "\n\n";
                while (std::getline(file,line)) {
                    std::string output = parser.parse(lexer.lex(line), &stack, &lexer);
                    std::cout << output << (output == "" ? "" : "\n");
                }
            }
        } else {
            std::string output = parser.parse(lexer.lex(input), &stack, &lexer);
            std::cout << output << (output == "" ? "" : "\n");
        }
=======
        if(input == "quit!") break;
        
        executeLine(&lexer, &parser, &stack, input);
>>>>>>> Malaxiz/master
    }
    file.close();
    return 0;
}
