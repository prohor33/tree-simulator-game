#pragma once

USING_NS_CC;
class Tree;


// Сцена поляны
class MeadowScene : public Scene {
public:
    static Scene* CreateScene(const Tree& tree);
    
    // a selector callback
    void MenuCloseCallback(Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MeadowScene);
    
private:
    void Build(const Tree& tree);
};
