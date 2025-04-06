#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#include <algorithm>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int checkHeight(Node* root){
    if(root == nullptr){
        return 0;
    }
    int left_height = checkHeight(root->left);
    int right_height = checkHeight(root->right);

    if(left_height == -1 || right_height == -1){
        return -1;
    }

    if(root->left != nullptr && root->right != nullptr && left_height != right_height){
        return -1;
    }

    return max(left_height, right_height) + 1;
}

bool equalPaths(Node* root)
{
    if(root == nullptr){
        return true;
    }
    return checkHeight(root) != -1;
}

