#pragma once
#include "Tree.h"

class TreeVisu {
public:
    TreeVisu(const Tree& tree);
    
    Node* root();
    
private:
    DrawNode* root_;
};
