//
//  node.h
//  P0
//
//  Created by Eric Goodwin on 2/9/19.
//  Copyright © 2019 umsl. All rights reserved.
//




#ifndef NODE_H
#define NODE_H


#include <string>
#include <stdio.h>
#include "token.h"


using namespace std;

struct node {
    string nodeLabel;
    node *child1, *child2, *child3, *child4;
    token_t token1;
    token_t token2;

};





#endif /* node_h */
