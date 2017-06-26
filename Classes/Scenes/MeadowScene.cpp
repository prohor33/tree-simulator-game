#include "stdafx.h"
#include "MeadowScene.h"
#include "SimpleAudioEngine.h"
#include "../Visualizers/TreeVisu.h"
#include "../Visualizers/MeadowVisu.h"
#include "../Game.h"
#include "../3rd/GestureRecognizer/PinchGestureRecognizer.h"
#include "../3rd/GestureRecognizer/PanGestureRecognizer.h"
#include "../Visualizers/Utils.h"


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
    
    scale_node_ = Node::create();
    
    // добавляем хэндлеры увеличения и перемещения
    // добавляется прямо к сцене, чтобы хэнлдить весь экран целиком
    // должна быть добавлена раньше всех
    AddScaling(scale_node_);
    
    background_ = Node::create();
    middleground_ = Node::create();
    foreground_ = Node::create();
    addChild(background_);
    
    addChild(middleground_);
    addChild(foreground_);
    
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
    foreground_->addChild(menu);


    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("TR Demo", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    foreground_->addChild(label);
    
    middleground_->addChild(scale_node_);
    
    gui_node_ = Node::create();
    middleground_->addChild(gui_node_);
    
    Size w_size = Director::getInstance()->getVisibleSize();
    Vec2 w_origin = Director::getInstance()->getVisibleOrigin();
    
    // попытка задника, который движется
//    scale_node_->addChild(visu_utils::LoadSpriteByWidth(2.f, "tree_back.png"));
    
    // трава
    DrawNode* draw_node = DrawNode::create();
    scale_node_->addChild(draw_node);
    const float grass_h = w_size.height / 10.f;
    Size grass_size(w_size.width, grass_h);
    const float grass_size_hidden = visu_utils::ScreenW(20.f);
    draw_node->drawSolidRect(w_origin - Vec2(grass_size_hidden, grass_size_hidden),
                             w_origin + grass_size + Vec2(grass_size_hidden, 0.f), field_color);
    
    // небо
    Size sky_size(w_size.width + 2 * grass_size_hidden, w_size.height - grass_h + grass_size_hidden);
    Vec2 sky_origin = w_origin + Vec2(0.f, grass_h) - Vec2(grass_size_hidden, 0.f);
    draw_node->drawSolidRect(sky_origin, sky_origin + sky_size, sky_color);
    
    Vec2 tree_p = w_origin + Vec2(w_size.width / 2.f, grass_h);
    
    tree_visu_ = TreeVisu::CreateLayer(tree_int, scale_node_, gui_node_);
    tree_visu_->setPosition(tree_p);
    tree_visu_->Build();
    scale_node_->addChild(tree_visu_);
    
    // ресурсы + прочая информация
    auto meadow_visu = MeadowVisu::CreateLayer(res_int, tree_int);
    foreground_->addChild(meadow_visu);

    // попытка неподвижного задника
//    background_->addChild(visu_utils::LoadSpriteFullScreen("tree_back.png"));
    
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

void MeadowScene::AddScaling(Node* scale_node) {
    PinchGestureRecognizer * pinch_rec = PinchGestureRecognizer::create();
    pinch_rec->setTarget([this, scale_node](CCGesture* gesture) {
        CCPinch* pinch = (CCPinch*) gesture;
        assert(!isnan(pinch->coef));
        
        tree_visu_->OnScaleOrMove();
        
//        if (pinch->coef < 1.f && (scale_node->getScale() * pinch->coef <= 1.f)) {
//            scale_node->setScale(1.f);
//            scale_node->setPosition(Vec2());
//            return;
//        }
        
        scale_node->setPosition(scale_node->getPosition() - (pinch->position - scale_node->getPosition()) * (pinch->coef - 1.f));
        visu_utils::AdditionalScale(scale_node, pinch->coef);
    });
    this->addChild(pinch_rec);
    
    
    PanGestureRecognizer* pan_rec = PanGestureRecognizer::create();
    pan_rec->setTarget([this, scale_node](CCGesture* gesture) {
        
        tree_visu_->OnScaleOrMove();
        
        CCPan* pan = (CCPan*) gesture;
        scale_node->setPosition(scale_node->getPosition() + pan->delta);
    });
    this->addChild(pan_rec);
    
    //    auto keyboard_listener = EventListenerKeyboard::create();
    //    keyboard_listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event) {
    //        switch (keyCode) {
    //            case EventKeyboard::KeyCode::KEY_PLUS:
    //            {
    //                visu_utils::AdditionalScale(this, 1.01f);
    //                return true;
    //            }
    //            case EventKeyboard::KeyCode::KEY_MINUS:
    //            {
    //                visu_utils::AdditionalScale(this, 1.f / 1.01f);
    //                return true;
    //            }
    //
    //            default:
    //                break;
    //        }
    //        return false;
    //    };
    //
    //    auto director = Director::getInstance();
    //    director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboard_listener, this);
}


