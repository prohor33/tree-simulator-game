#include "stdafx.h"
#include "BranchesVisu.h"
#include "TreeVisu.h"
#include "Utils.h"
using namespace cocos2d::ui;

namespace {
    const Color4F branch_color = Color4F(Color3B(169, 113, 0));
    const float grow_points_size = 0.05f;
}

BranchesVisu* BranchesVisu::CreateLayer(const TreePtr& tree, Node* scale_node, Node* gui_node, Node* top_level_gui, TreeVisu* tree_visu) {
    BranchesVisu* layer = BranchesVisu::create();
    layer->scale_node_ = scale_node;
    layer->gui_node_ = gui_node;
    layer->top_level_gui_ = top_level_gui;
    layer->tree_visu_ = tree_visu;
    layer->tree_ = tree;
    layer->Build();
    return layer;
}

bool BranchesVisu::init() {
    if (!Layer::init())
        return false;
    
    new_branch_mover_src_ = nullptr;
    this->scheduleUpdate();
    return true;
}

void BranchesVisu::Build() {	
    this->scheduleUpdate();
    
    draw_node_ = DrawNode::create();
    addChild(draw_node_);
    
    select_branch_elements_ = Node::create();
    gui_node_->addChild(select_branch_elements_);
    
    grow_buttons_ = Node::create();
    select_branch_elements_->addChild(grow_buttons_);
    
    new_branch_mover_ = Node::create();
    select_branch_elements_->addChild(new_branch_mover_);
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
        visu_utils::ChangeSizeByWidth(btn, grow_points_size);
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
        visu_utils::MoveToPosInOtherNode(gb, this, p.first);
        grow_buttons_ids_[i] = p.second;
        i++;
    }
}

void BranchesVisu::GrowButtonOnClick(size_t button_i, const Vec2& src_pos, const Size& src_size) {
    top_level_gui_->removeAllChildren();
    
    const float dlg_size = 0.8f;
    const float btn_size = dlg_size * 0.07f;
    const float shift_vert = visu_utils::ScreenW(0.03f) * dlg_size;
    const float padding = visu_utils::ScreenW(0.015f) * dlg_size;
    
    DrawNode* back_node = DrawNode::create();
    top_level_gui_->addChild(back_node);
    Vec2 back_origin, back_dest;
    
    {
        auto btn = Button::create("tree_icons/branch.png");
        visu_utils::ChangeSizeByHeight(btn, btn_size);
        btn->setTouchEnabled(true);
        Vec2 btn_p = src_pos + Vec2(0.f, src_size.height / 2.f);
        btn_p += Vec2(padding, shift_vert) + btn->getBoundingBox().size / 2.f;
        btn->setPosition(btn_p);
        back_dest = btn_p + btn->getBoundingBox().size / 2.f  + Vec2(padding, padding);
        
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
        visu_utils::ChangeSizeByHeight(btn, btn_size);
        btn->setTouchEnabled(true);
        Vec2 btn_p = src_pos + Vec2(0.f, src_size.height / 2.f);
        btn_p += Vec2(-padding - btn->getBoundingBox().size.width / 2.f, shift_vert + btn->getBoundingBox().size.height / 2.f);
        btn->setPosition(btn_p);
        back_origin = btn_p - btn->getBoundingBox().size / 2.f - Vec2(padding, padding);
        
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
   
    auto btn = Button::create("tree_icons/move.png");
    visu_utils::ChangeSizeByWidth(btn, 0.1f);
    btn->setTouchEnabled(true);
    btn->setPosition(node->getPosition());
    new_branch_mover_->removeAllChildren();
    new_branch_mover_->addChild(btn);
    btn->getEventDispatcher()->removeEventListenersForTarget(btn);
    new_branch_mover_src_ = node;
    new_branch_mover_delta_ = Vec2();
    
    auto btn_pos = visu_utils::GetPositionInOtherNode(this, node);
    
    visu_utils::AddOnMoveListener(btn,
    [this, btn_pos] (Node* n, Point p) {
        // on start
        this->tmp_draw_branch_ = { btn_pos, btn_pos };
        
    }, [=] (cocos2d::Touch* touch, cocos2d::Event* event) {
        // on move
        if (this->tmp_draw_branch_.size() == 2) {
            auto delta = touch->getDelta() / scale_node_->getScale();
            this->tmp_draw_branch_[1] += delta;
            new_branch_mover_delta_ += touch->getDelta();
        }
    }, [=] (Node* n, Point p) {
        new_branch_mover_->removeAllChildren();
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
    if (new_branch_mover_->getChildrenCount() > 0 && new_branch_mover_src_) {
        // обновляем положение мувера при перемещении ноды
        new_branch_mover_->getChildren().at(0)->setPosition(new_branch_mover_src_->getPosition() + new_branch_mover_delta_);
    }
}
