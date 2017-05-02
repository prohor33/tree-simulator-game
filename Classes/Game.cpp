#include "stdafx.h"
#include "Game.h"
#include "Scenes/MeadowScene.h"
#include "TreeModel/Tree.h"

USING_NS_CC;

std::shared_ptr<Game> Game::instance_ = nullptr;

Game* Game::instance() {
    if (!instance_.get())
        instance_ = std::shared_ptr<Game>(new Game());
    return instance_.get();
}

Game::Game() {}

void Game::Start() {
    
    auto director = Director::getInstance();
    
    // for debug
    Size w_size = Director::getInstance()->getVisibleSize();
    
    //Vec2 p0;
    //Vec2 p1(0, w_size.height / 15.f);   // вверх
    //Vec2 p2 = p1 + Vec2(w_size.width / 8.f, w_size.height / 18.f);   // вправо вверх
    //Vec2 p3 = p1 + Vec2(-w_size.width / 6.f, w_size.height / 12.f);   // влево вверх
    
    //Tree tree;
    //tree.add_segment(std::make_pair(p0, p1));
    //tree.add_segment(std::make_pair(p1, p2));
    //tree.add_segment(std::make_pair(p1, p3));

	TreeInterface tree_interface;
	tree_interface.MakeTree(Vec2(0, 0));

	int first_id, second_id, third_id, fourth_id, fifth_id;
	tree_interface.AddBranch(0, Vec2(0, w_size.height / 15.f), first_id);

	tree_interface.AddBranch(first_id, Vec2(w_size.width / 8.f, w_size.height / 18.f), second_id);
	tree_interface.AddBranch(first_id, Vec2(-w_size.width / 6.f, w_size.height / 12.f), third_id);


	tree_interface.AddBranch(third_id, Vec2(w_size.width / 18.f, w_size.height / 24.f), fourth_id);
	tree_interface.AddBranch(second_id, Vec2(-w_size.width / 12.f, w_size.height / 16.f), fifth_id);

    // create a scene. it's an autorelease object
    auto scene = MeadowScene::CreateScene(tree_interface);
    director->runWithScene(scene);
}

void Game::Exit() {
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
