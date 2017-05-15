#include "stdafx.h"
#include "MeadowVisu.h"
#include "Utils.h"
using namespace cocos2d::ui;


Layer* MeadowVisu::CreateLayer(ResourceInterface resource) {
    MeadowVisu* layer = MeadowVisu::create();
    layer->Build(resource);
    return layer;
}

bool MeadowVisu::init() {
    if (!Layer::init())
        return false;
    
    log("TreeVisu::init()");
    this->scheduleUpdate();
    return true;
}

void MeadowVisu::Build(ResourceInterface resource) {
    this->scheduleUpdate();
	resource_ = resource;
    
    // рисуем ресурсы
    info_ = Node::create();
    // вода
    {
        auto l = Label::createWithSystemFont("Water:", "fonts/arial.ttf", 22.f, Size::ZERO, TextHAlignment::LEFT);
        visu_utils::ToTop(l, 10, true);
        info_->addChild(l);
        water_res_lbl_ = l;
    }
    // солнце
    {
        auto l = Label::createWithSystemFont("Sun:", "fonts/arial.ttf", 22.f, Size::ZERO, TextHAlignment::CENTER);
        visu_utils::ToRight(l, 10, true);
        visu_utils::ToTop(l, 35, true);
        info_->addChild(l);
        sun_res_lbl_ = l;
    }
    // глюкоза
    {
        auto l = Label::createWithSystemFont("Glucose:", "fonts/arial.ttf", 22.f, Size::ZERO, TextHAlignment::CENTER);
        visu_utils::ToRight(l, 10, true);
        visu_utils::ToTop(l, 60, true);
        info_->addChild(l);
        glucose_res_lbl_ = l;
    }
    
    addChild(info_);
}

void MeadowVisu::update(float delta){
    UpdateInfo(delta);
}

// рисует количество ресурсов
void MeadowVisu::UpdateInfo(float delta) {
    water_res_lbl_->setString("Water: " + s_utils::to_str(resource_.GetCurrentResource(TreeResourceType::Water)));
    visu_utils::ToRight(water_res_lbl_, 10, true);
    
    sun_res_lbl_->setString("Sun: " + s_utils::to_str(resource_.GetCurrentResource(TreeResourceType::Water)));
    visu_utils::ToRight(sun_res_lbl_, 10, true);
    
    glucose_res_lbl_->setString("Glucose: " + s_utils::to_str(resource_.GetCurrentResource(TreeResourceType::Water)));
    visu_utils::ToRight(glucose_res_lbl_, 10, true);
}
