#include "stdafx.h"
#include "TreeVisu.h"
#include "Utils.h"
using namespace cocos2d::ui;


Layer* TreeVisu::CreateLayer(const Tree& tree) {
    TreeVisu* layer = TreeVisu::create();
    layer->Build(tree);
    return layer;
}

bool TreeVisu::init() {
    if (!Layer::init())
        return false;
    
    log("TreeVisu::init()");
    this->scheduleUpdate();
    return true;
}

void TreeVisu::Build(const Tree& tree) {
    this->scheduleUpdate();
    tree_ = tree;
    
    draw_node_ = DrawNode::create();
    addChild(draw_node_);
    gui_root_ = Node::create();
    addChild(gui_root_);
    grow_buttons_ = Node::create();
    gui_root_->addChild(grow_buttons_);

}

void TreeVisu::update(float delta){
//    log("TreeVisu::update: %lf", delta);
    
    draw_node_->clear();
    
    // рисуем ветки
    for (auto& s : tree_.segments())
        draw_node_->drawSegment(s.first, s.second, 10, Color4F::ORANGE);
    
    DrawGrowButtons(delta);
}

void TreeVisu::DrawGrowButtons(float delta) {
    // пока перерисоываем при изменении количества, потом надо будет подумать
    if (grow_buttons_->getChildrenCount() == tree_.grow_points().size())
        return;
    
    grow_buttons_->removeAllChildren();
    
    // рисуем кнопки в точках возможного роста
    for (auto& p : tree_.grow_points()) {
        
        auto btn = Button::create("tree_icons/plus.png");
        btn->setTouchEnabled(true);
        btn->setPosition(p);
        
        visu_utils::AddOnClickListener(btn, [=] (Button* node) {
            
            tree_.add_segment(std::make_pair(node->getPosition(), node->getPosition() + Vec2(rand() % 10 > 5 ? 80 : -80, 100)));
        }, 1.1f);
        
        grow_buttons_->addChild(btn);
    }
}

//// вызывается при клике на плюсик добавления новой ветки
//void TreeVisu::OnStartAddingLimb() {
//    
//}
//
//// вызывается когда пользователь решил добавить ветку
//void TreeVisu::OnGrowLimb() {
//    
//}
