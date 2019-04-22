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

//flag is used to trigger a visibility mode for development purposes
int DEV = 0;

Parser::Parser() {
}

node* getNode(string label){
    node* node = new struct node;
    node->nodeLabel = label;
    return node;
}

node *tree = getNode("root");

node * Parser::parser() {
    
    tree = program();
   
 //   tkScanner();
    
    if (receivedToken.tokenInstance == "EOF") {
        cout << "Parsing Complete.  No Errors" << endl;;
    }
    
    else{
        error("EOF");

    }
    return tree;
    
}
node* Parser::program() {
    node *node = getNode("program");
    
    node->child1 = vars();
    node->child2 = block();
    
    return node;
    
    
}
node* Parser::block() {
    node *node = getNode("block");
    if (receivedToken.tokenInstance == "void"){
        node->child1 = vars();
        node->child2 = stats();
        if (receivedToken.tokenInstance == "return"){
            tkScanner();
            return node;
        } else {
            error("return");
        }
    } else {
        error("void");
    }
    tkScanner();
    return node;
}

node* Parser::vars() {
    node *node = getNode("vars");
    tkScanner();
    if (receivedToken.tokenInstance == "var") {  //predict <vars> -> var Identifier : Integer <vars>
        tkScanner();
        if (receivedToken.tokenID == identifierToken) {
            node->token1 = receivedToken;
            tkScanner();
            if (receivedToken.tokenInstance == ":") {
                tkScanner();
                if (receivedToken.tokenID == digitToken) {
                    node->token2 = receivedToken;
                    node->child1 = vars();
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
    return node;
}
node*  Parser::expr() {
    node* node = getNode("expr");
    node->child1 = A();
    if (receivedToken.tokenInstance == "+" ){  //predict A-> A + <expr>
        node->token1 = receivedToken;
        tkScanner();
        node->child2 = expr();
        return node;
    } else if (receivedToken.tokenInstance == "-") { //predict A-> A - <expr>
        node->token1 = receivedToken;
        tkScanner();
        node->child2 = expr();
        return node;
    } else {
        return node;
    }
}

node*  Parser::A() {
    node* node = getNode("A");
    node->child1 = N();
    if (receivedToken.tokenInstance == "/") {
        node->child1 = A();
        node->token1 = receivedToken;
        tkScanner();
        return node;
    } else {
        return node;
    }
    return node;
    
    
}
node*  Parser::N() {
    node* node = getNode("N");
    node->child1 = M();
    if (receivedToken.tokenInstance == "*") {
        node->child1 = N();
        node->token1 = receivedToken;
        tkScanner();
        return node;
    } else {
        return node;
    }
    return node;
}
node*  Parser::M() {
    node* node = getNode("M");
    if (receivedToken.tokenInstance == "%") { //predict M-> % <M>
        node->token1 = receivedToken;
        tkScanner();
        node->child1 = M();
        return node;
    } else {              //predict M->R()
        node->child1 = R();
        return node;
    }
    return node;
    
    
}
node*  Parser::R() {
    node* node = getNode("R");
    if (receivedToken.tokenInstance == "(" ) {   // predict R-> ( <expr> )
        tkScanner();
        node->child1 = expr();
        tkScanner();
        if (receivedToken.tokenInstance != ")") {
            error(")");
        }
       tkScanner();
    } else if (receivedToken.tokenID == identifierToken || receivedToken.tokenID == digitToken) {
        node->token1 = receivedToken;
        tkScanner();
        return node;
    } else if (receivedToken.tokenInstance == "*" || receivedToken.tokenInstance == "/" || receivedToken.tokenInstance == "%"){
        node->token1 = receivedToken;
        tkScanner();
        return node;
    } else {
        error("(, identifier, or digit");
    }
    return node;
}
node*  Parser::stats() {
    node* node = getNode("stats");
    node->child1 = stat();
    if (receivedToken.tokenInstance != ";"){
        error(";");
    }
    node->child2 = mStat();
    return node;
    
    
}
node*  Parser::mStat() {
    node* node = getNode("mStats");
    tkScanner();
    if (receivedToken.tokenInstance == "scan" || receivedToken.tokenInstance == "print" || receivedToken.tokenInstance == "void" || receivedToken.tokenInstance == "cond" || receivedToken.tokenInstance == "iter") {
        node->child1 = stat();
        if (receivedToken.tokenInstance == ";") {
            node->child2 = mStat();
            return node;
        } else {
            error(";");
        }
    } else {
        return node;
    }
    return node;
}
node*  Parser::stat() {
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
        node->child1 = assign();
        return node;
    } else {
        error("scan, print, void, cond, or iter");
    }
    
    return node;
}

//xcode say in as a keyword.  workaround by captalizing
node*  Parser::IN() {
    node* node = getNode("In");
    if (receivedToken.tokenID == identifierToken) {
        node->token1 = receivedToken;
        tkScanner();
    } else {
        error("identifier");
    }
    
    return node;
}

//xcode say out as a keyword.  workaround by captalizing
node* Parser::OUT() {
    node* node = getNode("Out");
    node->child1 = expr();
    
    return node;
    
}
//xcode say if as a keyword.  workaround by captalizing
node* Parser::IF() {
    node* node = getNode("If");
    if (receivedToken.tokenInstance == "["){
        tkScanner();
        node->child1 = expr();
    //    tkScanner();
        node->child2 = RO();
        node->child3 = expr();
        if (receivedToken.tokenInstance != "]"){
            error("]");
        }
        tkScanner();
        node->child4 = stat();
    } else {
        error("[");
    }
    
    return node;
}
node* Parser::loop() {
    node* node = getNode("loop");
    if (receivedToken.tokenInstance == "["){
        tkScanner();
        node->child1 = expr();
  //      tkScanner();
        node->child2 = RO();
        node->child3 = expr();
        if (receivedToken.tokenInstance != "]"){
            error("]");
        }
        tkScanner();
        node->child4 = stat();
    } else {
        error("[");
    }
    return node;
}
node* Parser::assign() {
    node* node = getNode("assign");
    if (receivedToken.tokenID == identifierToken){
        tkScanner();
        if (receivedToken.tokenInstance == "=") {
            node->child1 = expr();
            return node;
        } else {
            error("=");
        }
    } else {
        error("identifer");
    }
    return node;
}
node* Parser::RO() {
    node* node = getNode("RO");
    // <   < >
    if (receivedToken.tokenInstance == "<"){
        node->token1 = receivedToken;
        tkScanner();
        if (receivedToken.tokenInstance == ">"){
            node->token2 = receivedToken;
            tkScanner();
            return node;
        }
        return node;
    } else if (receivedToken.tokenInstance == "=") {
        node->token1 = receivedToken;
        tkScanner();
        if (receivedToken.tokenInstance == ">"){
            node->token2 = receivedToken;
            tkScanner();
            return node;
        } else if (receivedToken.tokenInstance == "<"){
            node->token2 = receivedToken;
            tkScanner();
            return node;
        } else {
            return node;
        }
    } else if (receivedToken.tokenInstance == ">"){
        node->token1 = receivedToken;
        tkScanner();
        return node;
    } else {
        error("<, => , > , => , < >, or =");
    }
    return node;
}



void Parser::Run(){
    
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

void Parser::DEVMode() {
    if (DEV) {
        cout << "Dev - Working token: " << receivedToken.tokenInstance << "\nDev - Type: " << tokenTypes[receivedToken.tokenID] << endl;
    }
}

void Parser::error(string expected) {
    cout << "Error: On line number: " << receivedToken.lineNumber << ". Expected '" << expected <<  "' Received: '" << receivedToken.tokenInstance  << "'.  Exiting." << endl;
    exit(EXIT_SUCCESS);
}

void Parser::tkScanner(){
    Run();
    
}

