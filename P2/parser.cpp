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

node *tree = getNode("root");

node * parser::parse() {
    
    tree = program();
   
    tkScanner();
    if (receivedToken.tokenInstance == "EOF") {
        cout << "Parsing Complete.  No Errors" << endl;;
    }
    
    else{
        error("parse() - eof");

    }
    return tree;
    
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
        node->child1 = vars();
        node->child2 = stats();
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
node*  parser::expr() {
    node* node = getNode("expr");
    node->child1 = A();
    if (receivedToken.tokenInstance == "+" ){  //predict A-> A + <expr>
        tkScanner();
        node->child1 = expr();
        return node;
    } else if (receivedToken.tokenInstance == "-") { //predict A-> A - <expr>
        tkScanner();
        node->child1 = expr();
        return node;
    } else {
        return node;
    }

}

node*  parser::A() {
    node* node = getNode("A");
    node->child1 = N();
    if (receivedToken.tokenInstance == "/") {
        node->child1 = A();
        tkScanner();
        return node;
    } else {
        return node;
    }
    return node;
    
    
}
node*  parser::N() {
    node* node = getNode("N");
    node->child1 = M();
    if (receivedToken.tokenInstance == "*") {
        node->child1 = N();
        tkScanner();
        return node;
    } else {
        return node;
    }
    return node;
}
node*  parser::M() {
    node* node = getNode("M");
    if (receivedToken.tokenInstance == "%") { //predict M-> % <M>
       tkScanner();
        node->child1 = M();
        return node;
    } else {              //predict M->R()
        node->child1 = R();
        return node;
    }
    return node;
    
    
}
node*  parser::R() {
    node* node = getNode("R");
    if (receivedToken.tokenInstance == "(" ) {   // predict R-> ( <expr> )
        tkScanner();
        node->child1 = expr();
        tkScanner();
        if (receivedToken.tokenInstance != ")") {
            error("R() - )");
        }
        tkScanner();
    } else if (receivedToken.tokenID == identifierToken || receivedToken.tokenID == digitToken) {
        node->token1 = receivedToken;
        tkScanner();
        return node;
    } else if (receivedToken.tokenInstance == "*" || receivedToken.tokenInstance == "/" || receivedToken.tokenInstance == "%"){
        tkScanner();
        return node;
    } else {
        error("R() - (, identifier, or digit");
    }
    return NULL;
}
node*  parser::stats() {
    node* node = getNode("stats");
    node->child1 = stat();
    if (receivedToken.tokenInstance != ";"){
        error("stats() - ;");
    }
    node->child2 = mStat();
    return node;
    
    
}
node*  parser::mStat() {
    node* node = getNode("mStats");
    tkScanner();
    if (receivedToken.tokenInstance == "scan" || receivedToken.tokenInstance == "print" || receivedToken.tokenInstance == "void" || receivedToken.tokenInstance == "cond" || receivedToken.tokenInstance == "iter") {
        node->child1 = stat();
        if (receivedToken.tokenInstance == ";") {
            node->child2 = mStat();
            return node;
        } else {
            error("mStat - ;");
        }
    } else {
        return node;
    }
    return NULL;
}
node*  parser::stat() {
    node* node = getNode("stat");
    if (receivedToken.tokenInstance == "scan") {
        tkScanner();
        node->child1 = IN();
        return node;
    } else if (receivedToken.tokenInstance == "print") {
        tkScanner();
        node->child1 = OUT();
        return node;
    } else if (receivedToken.tokenInstance == "void") {
      //  tkScanner();
        node->child2 = block();
      //  tkScanner();
        return node;
    } else if (receivedToken.tokenInstance == "cond") {
        tkScanner();
        node->child1 = IF();
        return node;
    } else if (receivedToken.tokenInstance == "iter") {
        tkScanner();
        node->child1 = loop();
        return node;
    }else if (receivedToken.tokenID == identifierToken) {
        tkScanner();
        node->child1 =  assign();
        return node;
    } else {
        error("stat() - scan, print, void, cond, or iter");
    }
    
    return NULL;
}

//xcode say in as a keyword.  workaround by captalizing
node*  parser::IN() {
    node* node = getNode("In");
    if (receivedToken.tokenID == identifierToken) {
        tkScanner();
    } else {
        error("IN() - identifier");
    }
    
    return node;
}

//xcode say out as a keyword.  workaround by captalizing
node* parser::OUT() {
    node* node = getNode("Out");
    node->child1 = expr();
    
    return node;
    
}
//xcode say if as a keyword.  workaround by captalizing
node* parser::IF() {
    node* node = getNode("If");
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
    
    return node;
}
node* parser::loop() {
    node* node = getNode("loop");
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
    return node;
}
node* parser::assign() {
    node* node = getNode("assign");
    if (receivedToken.tokenID == identifierToken){
        tkScanner();
        if (receivedToken.tokenInstance == "=") {
            node->child1 = expr();
            return node;
        } else {
            error("assign() - =");
        }
    } else {
        error("assign() - identifer");
    }
    return node;
}
node* parser::RO() {
    node* node = getNode("RO");
    // <   < >
    if (receivedToken.tokenInstance == "<"){
        tkScanner();
        if (receivedToken.tokenInstance == ">"){
            tkScanner();
            return node;
        }
        return node;
    } else if (receivedToken.tokenInstance == "=") {
        tkScanner();
        if (receivedToken.tokenInstance == ">"){
            tkScanner();
            return node;
        } else if (receivedToken.tokenInstance == "<"){
            tkScanner();
            return node;
        } else {
            return node;
        }
    } else if (receivedToken.tokenInstance == ">"){
        tkScanner();
        return node;
    } else {
        error("RO() - <, => , > , => , < >, or =");
    }
    return node;
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

