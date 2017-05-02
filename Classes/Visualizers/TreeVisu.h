#pragma once
#include "../TreeModel/Tree.h"

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
    
    DrawNode* draw_node_;   // рисованное дерево
    Node* leafs_;
    Node* gui_root_;        // кнопочки на дереве
    Node* grow_buttons_;
    Node* top_level_gui_;
	std::map<Vec2, int> map_grows_to_id;
    //Tree tree_;

	TreeInterface tree_interface_;
};
