#pragma once
#include "../TreeModel/TreeStructure.h"

class TreeVisu : public Layer {
public:
    //static Layer* CreateLayer(const TreeInterface& tree);
	static Layer* CreateLayer(const TreePtr& tree_interface);
    virtual bool init() override;
    CREATE_FUNC(TreeVisu);
    
    void update(float) override;
    
private:
	void Build(const TreePtr& tree);
    // рисует кнопки в точках возможного роста
    void DrawGrowButtons(float delta);
    void GrowButtonOnClick(size_t button_i, const Vec2& src_pos, const Size& src_size);
    void OnAddLeaf(int parent_id);
    void DrawLeafs(float delta);
    // вызывается при клике на плюсик добавления новой ветки
    void OnStartAddingBranch(const Vec2& pos);
    // вызывается когда пользователь решил добавить ветку
    void OnAddBranch(int parent_id, const Vec2& b, const Vec2& e);
    
    DrawNode* draw_node_;   // рисованное дерево
    Node* leafs_;
    Node* gui_root_;        // кнопочки на дереве
    Node* grow_buttons_;
    std::vector<int> grow_buttons_ids_;
    Node* top_level_gui_;

	TreePtr tree_;
};
