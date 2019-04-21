//
//  parser.hpp
//  P2
//
//  Created by Eric Goodwin on 4/15/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#ifndef parser_h
#define parser_h

#include <stdio.h>
#include "token.h"
#include "tree.h"

extern ifstream fileToRead;
extern tree parseTree;


class parser {
    
public:
    parser();
    void parse();


    
private:
    void program();
    void block();
    void vars();
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
