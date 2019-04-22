//
//  parser.hpp
//  P2
//
//  Created by Eric Goodwin on 4/15/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "token.h"
#include "node.h"

extern ifstream fileToRead;
extern node * tree;



class Parser {
    
public:
    Parser();
    node* parser();


    
private:
    node* program();
    node* block();
    node* vars();
    node* expr();
    node* A();
    node* N();
    node* M();
    node* R();
    node* stats();
    node* mStat();
    node* stat();
    node* IN();      //xcode say in as a keyword.  workaround by captalizing
    node* OUT();     //xcode say out as a keyword.  workaround by captalizing
    node* IF();      //xcode say if as a keyword.  workaround by captalizing
    node* loop();
    node* assign();
    node* RO();
 //   void parseTokens(token_t token);
    void tkScanner();
    void Run();
    void error(string expected);
    void DEVMode();
    
    
};


#endif /* parser_h */
