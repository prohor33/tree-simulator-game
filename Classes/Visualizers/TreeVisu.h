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
    // рисует кнопки в точках возможного роста
    void DrawGrowButtons(float delta);
    void GrowButtonOnClick(const Vec2& src_pos, const Size& src_size);
    void OnAddLeaf(const Vec2& pos);
    void DrawLeafs(float delta);
    
    DrawNode* draw_node_;   // рисованное дерево
    Node* leafs_;
    Node* gui_root_;        // кнопочки на дереве
    Node* grow_buttons_;
    Node* top_level_gui_;
    Tree tree_;
};
