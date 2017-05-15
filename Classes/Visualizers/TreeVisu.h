#pragma once
#include "../TreeModel/TreeStructure.h"

class TreeVisu : public Layer {
public:
    //static Layer* CreateLayer(const TreeInterface& tree);
	static Layer* CreateLayer(TreeInterface tree_interface);
    virtual bool init() override;
    CREATE_FUNC(TreeVisu);
    
    void update(float) override;
    
private:
    //void Build(const Tree& tree);
	void Build(TreeInterface tree_interface);
    // рисует кнопки в точках возможного роста
    void DrawGrowButtons(float delta);
    void GrowButtonOnClick(const Vec2& src_pos, const Size& src_size);
    void OnAddLeaf(const Vec2& pos);
    void DrawLeafs(float delta);
    // вызывается при клике на плюсик добавления новой ветки
    void OnStartAddingBranch(const Vec2& pos);
    // вызывается когда пользователь решил добавить ветку
    void OnAddBranch(const Vec2& b, const Vec2& e);
    
    DrawNode* draw_node_;   // рисованное дерево
    Node* leafs_;
    Node* gui_root_;        // кнопочки на дереве
    Node* grow_buttons_;
    Node* top_level_gui_;
	std::map<Vec2, int> map_grows_to_id;

	TreeInterface tree_interface_;
};
