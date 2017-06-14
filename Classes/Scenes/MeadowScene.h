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
    void AddScaling(Node* scale_node);
    
    TreePtr tree_int_;
    ResourcesPtr res_int_;
    Node* background_;      // задник, недвижим
    Node* middleground_;    // все остальное, может быть увеличено
    Node* foreground_;      // передник, недвижим
};
