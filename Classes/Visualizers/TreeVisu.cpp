#include "stdafx.h"
#include "TreeVisu.h"
#include "Utils.h"
#include "LeafsVisu.h"
#include "BranchesVisu.h"
#include "ResourceConsVisu.h"
using namespace cocos2d::ui;

namespace {
    const Color4F branch_color = Color4F(Color3B(169, 113, 0));
}

Layer* TreeVisu::CreateLayer(const TreePtr& tree) {
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

void TreeVisu::Build(const TreePtr& tree) {
    this->scheduleUpdate();
	tree_ = tree;
    
    top_level_gui_ = Node::create();
    
    leafs_visu_ = LeafsVisu::CreateLayer(tree_, top_level_gui_, this);
    addChild(leafs_visu_);
    
    tree_root_ = Node::create();
    addChild(tree_root_);

    branches_visu_ = BranchesVisu::CreateLayer(tree_, top_level_gui_, this);
    addChild(branches_visu_);
    
    tree_root_buttons_ = Node::create();
    addChild(tree_root_buttons_);
    
    gui_root_ = Node::create();
    addChild(gui_root_);
    
    gui_root_->addChild(top_level_gui_);
    
    BuildTreeRoot();
    
    res_cons_visu_ = ResourceConsVisu::CreateLayer(tree_, this);
    addChild(res_cons_visu_);
}

// обновление отрисовки
void TreeVisu::update(float delta) {
//    log("TreeVisu::update: %lf", delta);
    
    
    DrawTreeRoot(delta);
}

void TreeVisu::BuildTreeRoot() {
    Sprite* s = Sprite::create("tree_elements/root.png");
    tree_root_->addChild(s);
    
    auto btn_plus = Button::create("tree_icons/root_plus.png");
    visu_utils::AddOnClickListener(btn_plus, [=] (Button* node) {
        tree_->AddRoot();
    }, 1.1f);
    tree_root_buttons_->addChild(btn_plus);
    visu_utils::MoveYPxl(btn_plus, -20);
}

void TreeVisu::DrawTreeRoot(float delta) {
    double root_length;
    tree_->GetRoot(root_length);
    if (tree_root_->getChildrenCount() < 1)
        return;
    
    auto s = tree_root_->getChildren().at(0);
    s->setScale(root_length / 3.0f);
    s->setPosition(0, 0);
    visu_utils::MoveYPxl(s, -s->getContentSize().height * s->getScale() / 2.f);
}

void TreeVisu::AddLeaf(int parent_id) {
    leafs_visu_->AddLeaf(parent_id);
}





