#include "stdafx.h"
#include "ResourceConsVisu.h"
#include "TreeVisu.h"
#include "../TreeModel/TreeResources.h"
#include "Utils.h"
#include "../Game.h"
using namespace cocos2d::ui;

namespace {
    const int root_id = -101;
}

ResourceConsVisu* ResourceConsVisu::CreateLayer(const TreePtr& tree, TreeVisu* tree_visu) {
    ResourceConsVisu* layer = ResourceConsVisu::create();
    layer->Build(tree, tree_visu);
    return layer;
}

bool ResourceConsVisu::init() {
    if (!Layer::init())
        return false;
    
    return true;
}

void ResourceConsVisu::Build(const TreePtr& tree, TreeVisu* tree_visu) {
	tree_ = tree;
    tree_visu_ = tree_visu;

    const float update_interval = 5.0f;
    schedule(schedule_selector(ResourceConsVisu::Update), update_interval);
}

void ResourceConsVisu::Update(float dt) {
    std::vector<int> elements;
    tree_->GetElements(elements);
    elements.push_back(root_id); // корень
    
    std::vector<TreeResourceType> res_types = { TreeResourceType::Glucose, TreeResourceType::Water, TreeResourceType::SunEnergy };
    
    for (auto& id : elements) {
        for (auto& res_type : res_types) {
            
            double prod = 0.;
            double cons = 0.;
            if (id == root_id) {
                auto root = tree_->GetRoot();
                cons = ResourceKeeper::GetConsumption(root, res_type);
                prod = ResourceKeeper::GetProduction(root, res_type);
            } else {
                tree_->GetElementProduction(id, res_type, prod);
                tree_->GetElementConsumption(id, res_type, cons);
            }
            
            double res_delta = (prod - cons) * dt * Game::instance()->resources_coef();
            
            double res_value = elements_prod_[id][res_type] + res_delta;
            if (fabs(res_value) < 1.) {
                elements_prod_[id][res_type] = res_value;
                continue;
            }
            
            elements_prod_[id][res_type] = res_value - int(res_value);
            
            ShowResCons(id, res_type, res_value);
        }
    }
}

// показывает анимацию траты/появления ресурса
void ResourceConsVisu::ShowResCons(int id, TreeResourceType res_type, double value) {
    std::map<TreeResourceType, Color3B> res_colors;
    res_colors[TreeResourceType::Glucose] = Color3B::RED;
    res_colors[TreeResourceType::Water] = Color3B::BLUE;
    res_colors[TreeResourceType::SunEnergy] = Color3B::YELLOW;
    
    TreeElement el;
    if (id == root_id) {
        el = tree_->GetRoot();
    } else {
        el = tree_->GetElementByID(id);
    }
    
    std::string str = s_utils::to_str((int)value);
    str = std::string(value > 0 ? "+" : "") + str;
    const float font_size = res_type == TreeResourceType::SunEnergy ? 11.f : 25.f;
    auto l = Label::createWithSystemFont(str, "fonts/arial.ttf", font_size, Size::ZERO, TextHAlignment::CENTER);
    Vec2 p = (el.start_point + el.end_point) / 2.f;
    addChild(l);
    visu_utils::MoveToPosInOtherNode(l, tree_visu_, p);
    l->setColor(res_colors[res_type]);
    
    auto after_move = [=] () {
        this->removeChild(l);
    };
    
    const float move_time = 5.f;
    const float move_dist = 200.f;
    auto move = MoveBy::create(move_time, Vec2(0, value > 0 ? move_dist : -move_dist));
    auto fade = FadeTo::create(move_time, 0.f);
    auto together = Spawn::create(move, fade, nullptr, nullptr);
    l->runAction(Sequence::create(together, CallFunc::create(after_move), nullptr));
}




