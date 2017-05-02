#pragma once

USING_NS_CC;
class TreeInterface;


// Сцена поляны
class MeadowScene : public Scene {
public:
    static Scene* CreateScene(TreeInterface tree_int);
    
    // a selector callback
    void MenuCloseCallback(Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MeadowScene);
    
private:
    //void Build(const Tree& tree);
	void Build(TreeInterface tree_int);
};
