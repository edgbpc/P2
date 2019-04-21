//
//  parser.cpp
//  P2
//
//  Created by Eric Goodwin on 4/15/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "parser.h"
#include "node.h"
#include "token.h"
#include "scanner.h"



using namespace std;

token_t receivedToken;

int DEV = 1;


parser::parser() {
}

node* getNode(string label){
    node* node = new struct node;
    node->nodeLabel = label;
    return node;
}

node * parser::parse() {
    node *root;
    
    root = program();
   
    tkScanner();
    if (receivedToken.tokenInstance == "EOF") {
        cout << "Parsing Complete.  No Errors" << endl;;
    }
    
    else{
        error("parse() - eof");

    }
    return root;
    
}
node* parser::program() {
    node *node = getNode("program");
    
    
    node->child1 = vars();
    node->child2 = block();
    
    return node;
    
    
}
node* parser::block() {
    node *node = getNode("block");
    if (receivedToken.tokenInstance == "void"){
        vars();
        stats();
        if (receivedToken.tokenInstance == "return"){
            tkScanner();
            return node;
        } else {
            error("block() - return");
        }
    } else {
        error("block() - void");
    }
    return node;
}

node* parser::vars() {
    node *node = getNode("vars");
    tkScanner();
    if (receivedToken.tokenInstance == "var") {  //predict <vars> -> var Identifier : Integer <vars>
        tkScanner();
        if (receivedToken.tokenID == identifierToken) {
            tkScanner();
            if (receivedToken.tokenInstance == ":") {
                tkScanner();
                if (receivedToken.tokenID == digitToken) {
                    vars();
                    return node;
                } else {
                    error("digit");
                }
            } else {
                error(":");
            }
        } else {
            error("identifier");
        }
    } else { //predict <vars> -> empty
        return NULL;
        
    }
    return NULL;
}
void parser::expr() {
    A();
    if (receivedToken.tokenInstance == "+" ){  //predict A-> A + <expr>
        tkScanner();
        expr();
        return;
    } else if (receivedToken.tokenInstance == "-") { //predict A-> A - <expr>
        tkScanner();
        expr();
        return;
    } else {
        return;
    }

}

void parser::A() {
    N();
    if (receivedToken.tokenInstance == "/") {
        A();
        tkScanner();
        return;
    } else {
        return;
    }
    return;
    
    
}
void parser::N() {
    M();
    if (receivedToken.tokenInstance == "*") {
        N();
        tkScanner();
        return;
    } else {
        return;
    }
    return;
}
void parser::M() {
    if (receivedToken.tokenInstance == "%") { //predict M-> % <M>
       tkScanner();
        M();
        return;
    } else {              //predict M->R()
        R();
        return;
    }
    return;
    
    
}
void parser::R() {
    if (receivedToken.tokenInstance == "(" ) {   // predict R-> ( <expr> )
        tkScanner();
        expr();
        tkScanner();
        if (receivedToken.tokenInstance != ")") {
            error("R() - )");
        }
        tkScanner();
    } else if (receivedToken.tokenID == identifierToken || receivedToken.tokenID == digitToken) {
        tkScanner();
        return;
    } else if (receivedToken.tokenInstance == "*" || receivedToken.tokenInstance == "/" || receivedToken.tokenInstance == "%"){
        tkScanner();
        return;
    } else {
        error("R() - (, identifier, or digit");
    }
    
}
void parser::stats() {
    stat();
    if (receivedToken.tokenInstance != ";"){
        error("stats() - ;");
    }
    mStat();
    return;
    
    
}
void parser::mStat() {
    tkScanner();
    if (receivedToken.tokenInstance == "scan" || receivedToken.tokenInstance == "print" || receivedToken.tokenInstance == "void" || receivedToken.tokenInstance == "cond" || receivedToken.tokenInstance == "iter") {
        stat();
        if (receivedToken.tokenInstance == ";") {
             mStat();
            return;
        } else {
            error("mStat - ;");
        }
    } else {
        return;
    }
}
void parser::stat() {
    if (receivedToken.tokenInstance == "scan") {
        tkScanner();
        IN();
        return;
    } else if (receivedToken.tokenInstance == "print") {
        tkScanner();
        OUT();
        return;
    } else if (receivedToken.tokenInstance == "void") {
      //  tkScanner();
        block();
      //  tkScanner();
        return;
    } else if (receivedToken.tokenInstance == "cond") {
        tkScanner();
        IF();
        return;
    } else if (receivedToken.tokenInstance == "iter") {
        tkScanner();
        loop();
        return;
    }else if (receivedToken.tokenID == identifierToken) {
        tkScanner();
        assign();
        return;
    } else {
        error("stat() - scan, print, void, cond, or iter");
    }
}

//xcode say in as a keyword.  workaround by captalizing
void parser::IN() {
    if (receivedToken.tokenID == identifierToken) {
        tkScanner();
    } else {
        error("IN() - identifier");
    }
}

//xcode say out as a keyword.  workaround by captalizing
void parser::OUT() {
    expr();
    
}
//xcode say if as a keyword.  workaround by captalizing
void parser::IF() {
    if (receivedToken.tokenInstance == "["){
        tkScanner();
        expr();
    //    tkScanner();
        RO();
        expr();
        if (receivedToken.tokenInstance != "]"){
            error("IF() - ]");
        }
        tkScanner();
        stat();
    } else {
        error("IF() - [");
    }
    
    
}
void parser::loop() {
    if (receivedToken.tokenInstance == "["){
        tkScanner();
        expr();
        tkScanner();
        RO();
        expr();
        if (receivedToken.tokenInstance != "]"){
            error("IF() - ]");
        }
        tkScanner();
        stat();
    } else {
        error("IF() - [");
    }
    
}
void parser::assign() {
    if (receivedToken.tokenID == identifierToken){
        tkScanner();
        if (receivedToken.tokenInstance == "=") {
            expr();
            return;
        } else {
            error("assign() - =");
        }
    } else {
        error("assign() - identifer");
    }
    
}
void parser::RO() {
    // <   < >
    if (receivedToken.tokenInstance == "<"){
        tkScanner();
        if (receivedToken.tokenInstance == ">"){
            tkScanner();
            return;
        }
        return;
    } else if (receivedToken.tokenInstance == "=") {
        tkScanner();
        if (receivedToken.tokenInstance == ">"){
            tkScanner();
            return;
        } else if (receivedToken.tokenInstance == "<"){
            tkScanner();
            return;
        } else {
            return;
        }
    } else if (receivedToken.tokenInstance == ">"){
        tkScanner();
        return;
    } else {
        error("RO() - <, => , > , => , < >, or =");
    }
    
}



void parser::Run(){
    
    // do while isTokenComplete = false
    // get a character
    // allow scanner to process character
    // when completed token, give the token to the parser
 
    do {
        if (fileToRead.eof()) {
        // end of file reached, done
            return;
      
        }  else {
           
            char character = fileToRead.get();
                executeScanner(character);
        
            if (isTokenComplete == true) {
                receivedToken = getFinalToken();
            DEVMode();
            }
            
        }
        
    } while (isTokenComplete == false);
    
}

void parser::DEVMode() {
    if (DEV) {
        cout << "Dev - Working token: " << receivedToken.tokenInstance << "\nDev - Type: " << tokenTypes[receivedToken.tokenID] << endl;
    }
}

void parser::error(string expected) {
    cout << "Error: On line number: " << receivedToken.lineNumber << ". Expected '" << expected <<  "' Received: '" << receivedToken.tokenInstance  << "'.  Exiting." << endl;
    exit(EXIT_SUCCESS);
}

void parser::tkScanner(){
    Run();
    
}

