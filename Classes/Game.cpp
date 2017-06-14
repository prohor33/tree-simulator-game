#include "stdafx.h"
#include "Game.h"
#include "Scenes/MeadowScene.h"
#include "TreeModel/TreeStructure.h"
#include "TreeModel/TreeResources.h"

USING_NS_CC;

std::shared_ptr<Game> Game::instance_ = nullptr;

Game* Game::instance() {
    if (!instance_.get())
        instance_ = std::shared_ptr<Game>(new Game());
    return instance_.get();
}

Game::Game() : game_speed_(1.) {}

void Game::Start() {
    
    auto director = Director::getInstance();
    
    // for debug
    Size w_size = Director::getInstance()->getVisibleSize();

    TreePtr tree_interface = std::make_shared<TreeInterface>();
	tree_interface->MakeTree(Vec2(0, 0));

	int first_id, second_id, third_id, fourth_id, fifth_id;
	tree_interface->AddBranch(0, Vec2(0, w_size.height / 15.f), first_id);

	tree_interface->AddBranch(first_id, Vec2(w_size.width / 8.f, w_size.height / 18.f), second_id);
	tree_interface->AddBranch(first_id, Vec2(-w_size.width / 6.f, w_size.height / 12.f), third_id);


	tree_interface->AddBranch(third_id, Vec2(w_size.width / 18.f, w_size.height / 24.f), fourth_id);
	tree_interface->AddBranch(second_id, Vec2(-w_size.width / 12.f, w_size.height / 16.f), fifth_id);


    ResourcesPtr resource_interface = std::make_shared<ResourceInterface>();
	resource_interface->MakeResources(1.0, 1.0);

    // create a scene. it's an autorelease object
    auto scene = MeadowScene::CreateScene(tree_interface, resource_interface);
    director->runWithScene(scene);
}

void Game::Exit() {
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

// скорость игры
double Game::game_speed() const {
    return game_speed_;
}

// коэффициент получения и траты ресурсов
double Game::resources_coef() const {
    return game_speed() * 0.1;
}

