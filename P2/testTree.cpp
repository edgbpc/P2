//
//  testTree.cpp
//  P2
//
//  Created by Eric Goodwin on 4/21/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "testTree.h"
#include "token.h"


using namespace std;

void printTree(node *tree, int depth) {
    if (tree == NULL) {
        return;
    }
    else {
        for (int i = 0; i <= depth; i++) {
            cout << "  ";
        }
        depth++;
        
        cout << tree->nodeLabel;
        
        token_t token1 = tree -> token1;
        token_t token2 = tree -> token2;
        
        if (token1.tokenInstance != "") {
            cout << " Token 1: " << tokenTypes[token1.tokenID] << ", element: " << token1.tokenInstance << " ";
        }
        if (token2.tokenInstance != "") {
            cout << " Token 2: " << tokenTypes[token2.tokenID] << ", element: " << token2.tokenInstance << " ";
        }
      
        
        cout << endl;
        
        if(tree->child1 != NULL){
            printTree(tree->child1, depth);
        }
        if(tree->child2 != NULL){
            printTree(tree->child2, depth);
        }
        if(tree->child3 != NULL){
            printTree(tree->child3, depth);
        }
        if(tree->child4 != NULL){
            printTree(tree->child4, depth);
        }
    }
}

//int tree::getLevelUtil(struct node * node, int key_value, int level){
//    if (node == NULL)
//        return 0;
//    if (node->key_value == key_value)
//        return level;
//    int downlevel = getLevelUtil(node->left, key_value, level + 1);
//    if (downlevel != 0)
//        return downlevel;
//    downlevel = getLevelUtil(node->right, key_value, level + 1);
//    return downlevel;
//}
//
//int tree::getLevel(struct node *node, int key_value){
//    return getLevelUtil(node, key_value, 0);
//}
