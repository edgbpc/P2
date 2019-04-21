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



class parser {
    
public:
    parser();
    node* parse();


    
private:
    node* program();
    node* block();
    node* vars();
    void expr();
    void A();
    void N();
    void M();
    void R();
    void stats();
    void mStat();
    void stat();
    void IN();      //xcode say in as a keyword.  workaround by captalizing
    void OUT();     //xcode say out as a keyword.  workaround by captalizing
    void IF();      //xcode say if as a keyword.  workaround by captalizing
    void loop();
    void assign();
    void RO();
    void parseTokens(token_t token);
    void tkScanner();
    void Run();
    void error(string expected);
    void DEVMode();
    
    
};


#endif /* parser_h */
