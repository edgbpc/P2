//
//  main.cpp
//  P1
//
//  Created by Eric Goodwin on 3/28/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "parser.h"
#include "node.h"
#include "testTree.h"

using namespace std;

ifstream fileToRead; //get the data to be processed
Parser myParser;

int main(int argc, const char * argv[]) {
    ofstream fileToWrite;
    
    string fileName;
    string outFileNameBase;
    string outFileName;
    string data; //variable that will be used in the tree
    string word;
    
 
 
  
    switch (argc)
    {
            //keyboard input or redirection input
        case 1:
            cout << "Keyboard or Redirection Input Mode" << endl;
            fileToWrite.open("temp.dat");
            //hitting enter will end input.  simulated end of file?
            while (getline(cin, data)){
                fileToWrite << data << endl;
            }
            outFileNameBase = "out";
            
            //will need to work on redirection on delmar
            fileToRead.open("temp.dat");
            
            break;
            //file input
        case 2:
            cout << "File Input Mode" << endl;
            fileName = argv[1];
            fileName.append(".sp19");

	    fileToRead.open(fileName.c_str());
            outFileNameBase = argv[1];
            break;
            
        default:
            cout << "Error:  Incorrect Parameters.  Exiting" << endl;
            return 0;
            break;
    }



  if (fileToRead.good()){
 //    myTestScanner.runTestScanner(fileToRead);
     // node *root = new node;
      tree = myParser.parser();
            
 }
    printTree(tree, 0);
    
    fileToRead.close();
    
    //clean up the temporary file created
    remove("temp.dat");

    return 0;
    
//} else {
 //   cout << "Error: Could not read file " << fileName <<  ". Exiting." << endl;
 //       return 0;
//    }
}
