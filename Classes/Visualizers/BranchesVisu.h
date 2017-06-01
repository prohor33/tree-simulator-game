#pragma once
#include "../TreeModel/TreeStructure.h"

class TreeVisu;

class BranchesVisu : public Layer {
public:
    //static Layer* CreateLayer(const TreeInterface& tree);
	static BranchesVisu* CreateLayer(const TreePtr& tree_interface, Node* top_level_gui, TreeVisu* tree_visu);
    virtual bool init() override;
    CREATE_FUNC(BranchesVisu);
    
    void update(float) override;
    
private:
	void Build(const TreePtr& tree, Node* top_level_gui, TreeVisu* tree_visu);
    void DrawBranches(float delta);
    // рисует кнопки в точках возможного роста
    void DrawGrowButtons(float delta);
    void GrowButtonOnClick(size_t button_i, const Vec2& src_pos, const Size& src_size);
    // вызывается при клике на иконку ветку добавления
    void OnStartAddingBranch(int parent_id, cocos2d::ui::Button* node);
    // рисует ветки в процессе добавления
    void DrawTemporaryElements(float delta);
    
    DrawNode* draw_node_;   // рисованное дерево
    Node* gui_root_;        // кнопочки на дереве
    Node* grow_buttons_;
    std::vector<int> grow_buttons_ids_; // идентификаторы вершин кнопок
    Node* top_level_gui_;   // временные диалоги и прочее
    
    // данные для временной отрисовки
    std::vector<Point> tmp_draw_branch_;

	TreePtr tree_;
    TreeVisu* tree_visu_;
};
