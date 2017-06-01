#include "stdafx.h"
#include "LeafsVisu.h"
#include "TreeVisu.h"
#include "Utils.h"
using namespace cocos2d::ui;

LeafsVisu* LeafsVisu::CreateLayer(const TreePtr& tree, Node* top_level_gui, TreeVisu* tree_visu) {
    LeafsVisu* layer = LeafsVisu::create();
    layer->Build(tree, top_level_gui, tree_visu);
    return layer;
}

bool LeafsVisu::init() {
    if (!Layer::init())
        return false;
    
    this->scheduleUpdate();
    return true;
}

void LeafsVisu::Build(const TreePtr& tree, Node* top_level_gui, TreeVisu* tree_visu) {
	tree_ = tree;
    top_level_gui_ = top_level_gui;
    tree_visu_ = tree_visu;
    this->scheduleUpdate();
    
    leafs_ = Node::create();
    addChild(leafs_);
}

void LeafsVisu::update(float delta) {
    DrawLeafs(delta);
}

void LeafsVisu::DrawLeafs(float delta) {
	std::vector<std::pair<Vec2, Vec2>> local_leafs;
	tree_->GetLeafs(local_leafs);
    while (leafs_->getChildrenCount() > local_leafs.size()) {
        // удаляем лишние
        leafs_->removeChild(leafs_->getChildren().at(0));
    }
    while (leafs_->getChildrenCount() < local_leafs.size()) {
        // добавляем недостающие
        auto l = visu_utils::LoadSpriteByWidth(0.1f, "tree_elements/leaf.png");
        l->setAnchorPoint(Vec2());
        const float max_start_angle = 45.f;
        l->setRotation(max_start_angle * s_utils::rand_m1_1());
        const float rot_time = 1.f + s_utils::rand_m1_1() * 0.8f;
        const float rot_angle = 30.f + s_utils::rand_m1_1() * 10.f;
        auto rotate1 = RotateBy::create(rot_time, rot_angle);
        auto rotate2 = RotateBy::create(rot_time, -rot_angle);
        l->runAction(RepeatForever::create(Sequence::create(rotate1, rotate2, nullptr)));
        leafs_->addChild(l);
    }
    
    size_t i = 0;
    for (auto& p : local_leafs) {
        CC_ASSERT(i < leafs_->getChildren().size());
        auto leaf = leafs_->getChildren().at(i);
        leaf->setPosition(p.first);
        i++;
    }
}
void LeafsVisu::AddLeaf(int parent_id) {
	int new_id;
    tree_->AddLeaf(parent_id, Vec2(0.f, 0.f), new_id);
}


