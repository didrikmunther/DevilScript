//
//  Stack.cpp
//  DevilScript
//
//  Created by Didrik Munther on 23/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "Stack.h"


Stack::Stack() {
    
}

Stack::~Stack() {
    clearStack();
}

Element* Stack::getElement(int index) {
    index = (int)elements.size() + index - 1;
    if(index <= elements.size() || index >= 0)
        return elements[index];
    
    return nullptr;
}