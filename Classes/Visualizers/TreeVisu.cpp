#include "stdafx.h"
#include "TreeVisu.h"

TreeVisu::TreeVisu(const Tree& tree) {
    root_ = DrawNode::create();
    
    Size w_size = Director::getInstance()->getVisibleSize();
    Vec2 w_origin = Director::getInstance()->getVisibleOrigin();
    
    // трава
    const float grass_h = w_size.height / 10.f;
    Size grass_size(w_size.width, grass_h);
    root_->drawSolidRect(w_origin, w_origin + grass_size, Color4F::GREEN);
    
    // рисуем дерево
    Vec2 tree_p = w_origin + Vec2(w_size.width / 2.f, grass_h);
    for (auto& s : tree.segments())
        root_->drawSegment(tree_p + s.first, tree_p + s.second, 10, Color4F::ORANGE);
}

Node* TreeVisu::root() {
    return root_;
}


