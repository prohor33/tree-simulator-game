#include "stdafx.h"
#include "MeadowScene.h"
#include "SimpleAudioEngine.h"
#include "../Visualizers/TreeVisu.h"
#include "../TreeModel/TreeStructure.h"
#include "../TreeModel/TreeResources.h"

Scene* MeadowScene::CreateScene(TreeInterface tree_int, ResourceInterface res_int) {
    MeadowScene* scene = MeadowScene::create();
    scene->Build(tree_int, res_int);
    return scene;
}

void MeadowScene::Build(TreeInterface tree_int, ResourceInterface res_int) {
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MeadowScene::MenuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("TR Demo", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    
    
    Size w_size = Director::getInstance()->getVisibleSize();
    Vec2 w_origin = Director::getInstance()->getVisibleOrigin();
    
    // трава
    DrawNode* draw_node = DrawNode::create();
    this->addChild(draw_node);
    const float grass_h = w_size.height / 10.f;
    Size grass_size(w_size.width, grass_h);
    draw_node->drawSolidRect(w_origin, w_origin + grass_size, Color4F::GREEN);

    Vec2 tree_p = w_origin + Vec2(w_size.width / 2.f, grass_h);
    
    auto tree_visu = TreeVisu::CreateLayer(tree_int);
    tree_visu->setPosition(tree_p);
    this->addChild(tree_visu, 1);
}


void MeadowScene::MenuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
