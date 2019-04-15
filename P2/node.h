//
//  node.h
//  P0
//
//  Created by Eric Goodwin on 2/9/19.
//  Copyright © 2019 umsl. All rights reserved.
//




#ifndef node_h
#define node_h

#include <set>
#include <string>
#include <stdio.h>


using namespace std;

struct node {
    int key_value;
    set<string> words;
    node *left;
    node *right;
};





#endif /* node_h */
