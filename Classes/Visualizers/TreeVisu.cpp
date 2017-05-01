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
    
    leafs_ = Node::create();
    addChild(leafs_);
    
    gui_root_ = Node::create();
    addChild(gui_root_);
    
    grow_buttons_ = Node::create();
    gui_root_->addChild(grow_buttons_);
    
    top_level_gui_ = Node::create();
    gui_root_->addChild(top_level_gui_);
}

void TreeVisu::update(float delta){
//    log("TreeVisu::update: %lf", delta);
    
    draw_node_->clear();
    
    // рисуем ветки
    for (auto& s : tree_.segments())
        draw_node_->drawSegment(s.first, s.second, 10, Color4F::ORANGE);
    
    DrawLeafs(delta);
    
    DrawGrowButtons(delta);
}

void TreeVisu::DrawLeafs(float delta) {
    while (leafs_->getChildrenCount() > tree_.leafs().size()) {
        // удаляем лишние
        leafs_->removeChild(leafs_->getChildren().at(0));
    }
    while (leafs_->getChildrenCount() < tree_.leafs().size()) {
        // добавляем недостающие
        auto l = visu_utils::LoadSpriteByWidth(0.1f, "tree_elements/leaf.png");        
        leafs_->addChild(l);
    }
    
    size_t i = 0;
    for (auto& p : tree_.leafs()) {
        CC_ASSERT(i < leafs_->getChildren().size());
        auto leaf = leafs_->getChildren().at(i);
        leaf->setPosition(p + leaf->getBoundingBox().size / 2.f);
        i++;
    }
}

// рисует кнопки в точках возможного роста
void TreeVisu::DrawGrowButtons(float delta) {
    
    while (grow_buttons_->getChildrenCount() > tree_.grow_points().size()) {
        // удаляем лишние
        grow_buttons_->removeChild(grow_buttons_->getChildren().at(0));
    }
    while (grow_buttons_->getChildrenCount() < tree_.grow_points().size()) {
        // добавляем недостающие
        auto btn = Button::create("tree_icons/plus.png");
        btn->setTouchEnabled(true);
        
        
        visu_utils::AddOnClickListener(btn, [=] (Button* node) {
            GrowButtonOnClick(node->getPosition(), node->getContentSize());
        }, 1.1f);
        
        grow_buttons_->addChild(btn);
    }
    
    size_t i = 0;
    for (auto& p : tree_.grow_points()) {
        CC_ASSERT(i < grow_buttons_->getChildren().size());
        grow_buttons_->getChildren().at(i)->setPosition(p);
        i++;
    }
}

void TreeVisu::GrowButtonOnClick(const Vec2& src_pos, const Size& src_size) {
    top_level_gui_->removeAllChildren();
    
    const float shift_vert = 10.f;
    const float padding = 5.f;
    
    DrawNode* back_node = DrawNode::create();
    top_level_gui_->addChild(back_node);
    Vec2 back_origin, back_dest;
    
    {
        auto btn = Button::create("tree_icons/branch.png");
        btn->setTouchEnabled(true);
        Vec2 btn_p = src_pos + Vec2(0.f, src_size.height / 2.f);
        btn_p += Vec2(padding, shift_vert) + btn->getContentSize() / 2.f;
        btn->setPosition(btn_p);
        back_dest = btn_p + btn->getContentSize() / 2.f  + Vec2(padding, padding);
        
        visu_utils::AddOnClickListener(btn, [=] (Button* node) {
            
        }, 1.1f);
        
        top_level_gui_->addChild(btn);
    }
    
    {
        auto btn = Button::create("tree_icons/leaf.png");
        btn->setTouchEnabled(true);
        Vec2 btn_p = src_pos + Vec2(0.f, src_size.height / 2.f);
        btn_p += Vec2(-padding - btn->getContentSize().width / 2.f, shift_vert + btn->getContentSize().height / 2.f);
        btn->setPosition(btn_p);
        back_origin = btn_p - btn->getContentSize() / 2.f - Vec2(padding, padding);
        
        visu_utils::AddOnClickListener(btn, [=] (Button* node) {
            top_level_gui_->removeAllChildren();
            OnAddLeaf(src_pos);
        }, 1.1f);
        
        top_level_gui_->addChild(btn);
    }
    
    back_node->drawSolidRect(back_origin, back_dest, Color4F::WHITE);
}

void TreeVisu::OnAddLeaf(const Vec2& pos) {
    tree_.add_leaf(pos);
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
