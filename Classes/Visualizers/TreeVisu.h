#pragma once
#include "../TreeModel/Tree.h"

class TreeVisu : public Layer {
public:
    static Layer* CreateLayer(const Tree& tree);
    virtual bool init() override;
    CREATE_FUNC(TreeVisu);
    
    void update(float) override;
    
private:
    void Build(const Tree& tree);
    void DrawGrowButtons(float delta);
    
    DrawNode* draw_node_;   // рисованное дерево
    Node* gui_root_;        // кнопочки на дереве
    Node* grow_buttons_;
    Tree tree_;
};
