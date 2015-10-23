//
//  main.cpp
//  DevilScript
//
//  Created by Didrik Munther on 23/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <iostream>

#include "Lexer.h"
#include "Parser.h"
#include "Stack.h"


int main(int argc, const char * argv[]) {
    
    std::cout << "Started;\n";
    
    Lexer lexer;
    Parser parser;
    Stack stack;
    
    //for(auto& i: lexer.lexString("var asdf ="))
        //std::cout << i.second << " | " << i.first << "\n";
    
    while(true) {
        std::cout << ">> ";
        std::string input = "";
        std::getline(std::cin, input);
        if(input == "quit") break;
        
        std::cout << parser.parse(lexer.lex(input), &stack, &lexer) << "\n";
    }
    
    return 0;
}
