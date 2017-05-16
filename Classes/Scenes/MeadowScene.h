#pragma once
#include "../TreeModel/TreeStructure.h"
#include "../TreeModel/TreeResources.h"

USING_NS_CC;

// Сцена поляны
class MeadowScene : public Scene {
public:
    static Scene* CreateScene(const TreePtr& tree_int, const ResourcesPtr& res_int);
    
    // a selector callback
    void MenuCloseCallback(Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MeadowScene);
    
private:
	void Build(const TreePtr& tree_int, const ResourcesPtr& res_int);
    void UpdateTree(float dt);
    
    TreePtr tree_int_;
    ResourcesPtr res_int_;
};
