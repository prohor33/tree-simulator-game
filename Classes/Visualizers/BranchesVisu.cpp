#include "stdafx.h"
#include "BranchesVisu.h"
#include "TreeVisu.h"
#include "Utils.h"
using namespace cocos2d::ui;

namespace {
    const Color4F branch_color = Color4F(Color3B(169, 113, 0));
}

BranchesVisu* BranchesVisu::CreateLayer(const TreePtr& tree, Node* top_level_gui, TreeVisu* tree_visu) {
    BranchesVisu* layer = BranchesVisu::create();
    layer->Build(tree, top_level_gui, tree_visu);
    return layer;
}

bool BranchesVisu::init() {
    if (!Layer::init())
        return false;
    
    this->scheduleUpdate();
    return true;
}

void BranchesVisu::Build(const TreePtr& tree, Node* top_level_gui, TreeVisu* tree_visu) {
	tree_ = tree;
    top_level_gui_ = top_level_gui;
    tree_visu_ = tree_visu;
    this->scheduleUpdate();
    
    draw_node_ = DrawNode::create();
    addChild(draw_node_);
    
    gui_root_ = Node::create();
    addChild(gui_root_);
    
    grow_buttons_ = Node::create();
    gui_root_->addChild(grow_buttons_);
}

void BranchesVisu::update(float delta) {
    draw_node_->clear();
    
    DrawBranches(delta);
    DrawGrowButtons(delta);
    // рисует ветки в процессе добавления
    DrawTemporaryElements(delta);
}

void BranchesVisu::DrawBranches(float delta) {
    // рисуем ветки
    std::vector<int> branches_id;
    tree_->GetBranches(branches_id);
    
    for (auto& id : branches_id) {        
        auto b = tree_->GetElementByID(id);
        const float width_coef = 4.f;
        draw_node_->drawSegment(b.start_point, b.end_point, b.width * width_coef, branch_color);
    }
}

// рисует кнопки в точках возможного роста
void BranchesVisu::DrawGrowButtons(float delta) {
    
	std::vector<std::pair<Vec2, int>> grow_points;
	tree_->GetGrowPoints(grow_points);

    while (grow_buttons_->getChildrenCount() > grow_points.size()) {
        // удаляем лишние
        grow_buttons_->removeChild(grow_buttons_->getChildren().at(0));
    }
    while (grow_buttons_->getChildrenCount() < grow_points.size()) {
        // добавляем недостающие
        auto btn = Button::create("tree_icons/plus.png");
        visu_utils::ChangeSizeByWidth(btn, 0.05f);
        btn->setTouchEnabled(true);
        
        size_t btn_ind = grow_buttons_->getChildrenCount();
        visu_utils::AddOnClickListener(btn, [=] (Button* node) {
            GrowButtonOnClick(btn_ind, node->getPosition(), node->getContentSize());
        }, 1.1f);
        
        grow_buttons_->addChild(btn);
    }
    
    size_t i = 0;
    grow_buttons_ids_.resize(grow_points.size());
    for (auto& p : grow_points) {
        CC_ASSERT(i < grow_buttons_->getChildren().size());
        auto gb = grow_buttons_->getChildren().at(i);
        gb->setPosition(p.first);
        grow_buttons_ids_[i] = p.second;
        i++;
    }
}

void BranchesVisu::GrowButtonOnClick(size_t button_i, const Vec2& src_pos, const Size& src_size) {
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
            // новая ветка
            top_level_gui_->removeAllChildren();
            if (button_i < grow_buttons_ids_.size() && button_i < grow_buttons_->getChildrenCount())
                OnStartAddingBranch(grow_buttons_ids_[button_i], (Button*)grow_buttons_->getChildren().at(button_i));
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
            // клик на лист
            top_level_gui_->removeAllChildren();
            assert(button_i < grow_buttons_ids_.size());
            tree_visu_->AddLeaf(grow_buttons_ids_[button_i]);
        }, 1.1f);
        
        top_level_gui_->addChild(btn);
    }
    
    back_node->drawSolidRect(back_origin, back_dest, Color4F::WHITE);
}

// вызывается при клике на иконку ветку добавления
void BranchesVisu::OnStartAddingBranch(int parent_id, Button* node) {
    // добавляем еще одну временную кнопку сверху
    
    auto btn = Button::create("tree_icons/plus.png");
    btn->setTouchEnabled(true);
    btn->setPosition(node->getPosition());
    top_level_gui_->addChild(btn);
    btn->getEventDispatcher()->removeEventListenersForTarget(btn);
    
    auto btn_pos = node->getPosition();
    
    visu_utils::AddOnMoveListener(btn,
    [this, btn_pos] (Node* n, Point p) {
        // on start
        this->tmp_draw_branch_ = { btn_pos, btn_pos };
        
    }, [=] (cocos2d::Touch* touch, cocos2d::Event* event) {
        // on move
        if (this->tmp_draw_branch_.size() == 2)
            this->tmp_draw_branch_[1] += touch->getDelta();
    }, [=] (Node* n, Point p) {
        top_level_gui_->removeAllChildren();
        if (this->tmp_draw_branch_.size() == 2) {
            int id;
            tree_->AddBranch(parent_id, this->tmp_draw_branch_[1] - this->tmp_draw_branch_[0], id);
        }
        this->tmp_draw_branch_.clear();
    });
}

// рисует ветки в процессе добавления
void BranchesVisu::DrawTemporaryElements(float delta) {
    if (tmp_draw_branch_.size() == 2) {
        draw_node_->drawSegment(tmp_draw_branch_[0], tmp_draw_branch_[1], 4, branch_color);
    }
}