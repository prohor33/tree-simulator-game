#include "stdafx.h"
#include "MeadowScene.h"
#include "SimpleAudioEngine.h"
#include "../Visualizers/TreeVisu.h"
#include "../Visualizers/MeadowVisu.h"
#include "../Game.h"

namespace {
    const Color4F field_color = Color4F(Color3B(0, 223, 3));
    const Color4F sky_color = Color4F(Color3B(58, 195, 255));
}


Scene* MeadowScene::CreateScene(const TreePtr& tree_int, const ResourcesPtr& res_int) {
    MeadowScene* scene = MeadowScene::create();
    scene->Build(tree_int, res_int);
    return scene;
}

void MeadowScene::Build(const TreePtr& tree_int, const ResourcesPtr& res_int) {
    tree_int_ = tree_int;
    res_int_ = res_int;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MeadowScene::MenuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("TR Demo", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    
    
    Size w_size = Director::getInstance()->getVisibleSize();
    Vec2 w_origin = Director::getInstance()->getVisibleOrigin();
    
    // трава
    DrawNode* draw_node = DrawNode::create();
    this->addChild(draw_node);
    const float grass_h = w_size.height / 10.f;
    Size grass_size(w_size.width, grass_h);
    draw_node->drawSolidRect(w_origin, w_origin + grass_size, field_color);
    
    // небо
    Size sky_size(w_size.width, w_size.height - grass_h);
    Vec2 sky_origin = w_origin + Vec2(0.f, grass_h);
    draw_node->drawSolidRect(sky_origin, sky_origin + sky_size, sky_color);

    Vec2 tree_p = w_origin + Vec2(w_size.width / 2.f, grass_h);
    
    auto tree_visu = TreeVisu::CreateLayer(tree_int);
    tree_visu->setPosition(tree_p);
    this->addChild(tree_visu);
    
    // ресурсы + прочая информация
    auto meadow_visu = MeadowVisu::CreateLayer(res_int, tree_int);
    this->addChild(meadow_visu);

    
    const float tree_update_interval = 0.1;
    schedule(schedule_selector(MeadowScene::UpdateTree), tree_update_interval);
}

void MeadowScene::UpdateTree(float dt) {
    
    std::vector<TreeResourceType> res_types = {
        TreeResourceType::Water,
        TreeResourceType::SunEnergy,
        TreeResourceType::Glucose
    };
    
    for (auto& res_t : res_types) {
        double prod;
        tree_int_->GetCurrentProduction(res_t, prod);
        double cons;
        tree_int_->GetCurrentConsumption(res_t, cons);
        
//        log("%s production: %lf", to_str(res_t).c_str(), prod);
//        log("%s consumption: %lf", to_str(res_t).c_str(), cons);
        
        double res_delta = (prod - cons) * dt * Game::instance()->resources_coef();
//        log("%s res_delta: %lf", to_str(res_t).c_str(), res_delta);
        auto result = res_int_->AddResources(res_t, res_delta);
        
//        log("%s current: %lf\n", to_str(res_t).c_str(), res_int_->GetCurrentResource(res_t));
        
        if (result == ResourceAddingResult::ResourceResultNotEnough) {
            log("Warning: not enough %s", to_str(res_t).c_str());
        }
    }
}

void MeadowScene::MenuCloseCallback(Ref* pSender) {
    Game::instance()->Exit();
}


