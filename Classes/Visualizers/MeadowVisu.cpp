#include "stdafx.h"
#include "MeadowVisu.h"
#include "Utils.h"
using namespace cocos2d::ui;


Layer* MeadowVisu::CreateLayer(const ResourcesPtr& resource, const TreePtr& tree) {
    MeadowVisu* layer = MeadowVisu::create();
    layer->Build(resource, tree);
    return layer;
}

bool MeadowVisu::init() {
    if (!Layer::init())
        return false;
    
    log("TreeVisu::init()");
    this->scheduleUpdate();
    
    return true;
}

void MeadowVisu::Build(const ResourcesPtr& resource, const TreePtr& tree) {
    this->scheduleUpdate();
	resource_ = resource;
    tree_ = tree;
    
    // рисуем ресурсы
    info_ = Node::create();
    // вода
    {
        auto l = Label::createWithSystemFont("Water:", "fonts/arial.ttf", 22.f, Size::ZERO, TextHAlignment::LEFT);
        visu_utils::ToTop(l, 10, true);
        info_->addChild(l);
        water_res_lbl_ = l;
    }
    {
        auto l = Label::createWithSystemFont("Water:", "fonts/arial.ttf", 22.f, Size::ZERO, TextHAlignment::LEFT);
        visu_utils::ToTop(l, 10, true);
        info_->addChild(l);
        water_res_lbl_arrow_ = l;
    }
    // глюкоза
    {
        auto l = Label::createWithSystemFont("Glucose:", "fonts/arial.ttf", 22.f, Size::ZERO, TextHAlignment::CENTER);
        visu_utils::ToTop(l, 35, true);
        info_->addChild(l);
        glucose_res_lbl_ = l;
    }
    {
        auto l = Label::createWithSystemFont("Water:", "fonts/arial.ttf", 22.f, Size::ZERO, TextHAlignment::LEFT);
        visu_utils::ToTop(l, 35, true);
        info_->addChild(l);
        glucose_res_lbl_arrow_ = l;
    }
    
    addChild(info_);
}

void MeadowVisu::update(float delta){
    UpdateInfo(delta);
    tree_->Update(delta);
}

// рисует количество ресурсов
void MeadowVisu::UpdateInfo(float delta) {
    auto to_str = [&](const double& n) -> std::string {
        std::ostringstream stm;
        stm.precision(0);
        stm << std::fixed << n;
        return stm.str();
    };
    
    double water = resource_->GetCurrentResource(TreeResourceType::Water);
    double glucose = resource_->GetCurrentResource(TreeResourceType::Glucose);
    
    double water_cons = 0.;
    double water_prod = 0.;
    tree_->GetCurrentConsumption(TreeResourceType::Water, water_cons);
    tree_->GetCurrentProduction(TreeResourceType::Water, water_prod);
    double glucose_cons = 0.;
    double glucose_prod = 0.;
    tree_->GetCurrentConsumption(TreeResourceType::Glucose, glucose_cons);
    tree_->GetCurrentProduction(TreeResourceType::Glucose, glucose_prod);
    
    const float shift1 = 10.f;
    const float shift2 = 35.f;
    
    // вода
    bool water_ok = water_cons <= water_prod;
    water_res_lbl_->setString("Water: " + to_str(water));
    visu_utils::ToRight(water_res_lbl_, shift2, true);
    
    water_res_lbl_arrow_->setString(water_ok ? "⬆" : "⬇");
    water_res_lbl_arrow_->setTextColor(water_ok ? Color4B::GREEN : Color4B::RED);
    visu_utils::ToRight(water_res_lbl_arrow_, shift1, true);
    
    
    // глюкоза
    bool glucose_ok = glucose_cons <= glucose_prod;
    glucose_res_lbl_->setString("Glucose: " + to_str(glucose));
    visu_utils::ToRight(glucose_res_lbl_, shift2, true);
    
    glucose_res_lbl_arrow_->setString(glucose_ok ? "⬆" : "⬇");
    glucose_res_lbl_arrow_->setTextColor(glucose_ok ? Color4B::GREEN : Color4B::RED);
    visu_utils::ToRight(glucose_res_lbl_arrow_, shift1, true);
}


