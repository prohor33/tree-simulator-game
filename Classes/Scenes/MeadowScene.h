#pragma once

USING_NS_CC;
class TreeInterface;
class ResourceInterface;

// Сцена поляны
class MeadowScene : public Scene {
public:
    static Scene* CreateScene(TreeInterface tree_int, ResourceInterface res_int);
    
    // a selector callback
    void MenuCloseCallback(Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MeadowScene);
    
private:
	void Build(TreeInterface tree_int, ResourceInterface res_int);
};
