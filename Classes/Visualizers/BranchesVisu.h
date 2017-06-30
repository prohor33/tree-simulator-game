#pragma once
#include "../TreeModel/TreeStructure.h"

class TreeVisu;

class BranchesVisu : public Layer {
public:
    //static Layer* CreateLayer(const TreeInterface& tree);
	static BranchesVisu* CreateLayer(const TreePtr& tree_interface, Node* scale_node, Node* gui_node, Node* top_level_gui, TreeVisu* tree_visu);
    virtual bool init() override;
    CREATE_FUNC(BranchesVisu);
    
    void update(float) override;
    
private:
	void Build();
    void DrawBranches(float delta);
    // рисует кнопки в точках возможного роста
    void DrawGrowButtons(float delta);
    void GrowButtonOnClick(size_t button_i, const Vec2& src_pos, const Size& src_size);
    // вызывается при клике на иконку ветку добавления
    void OnStartAddingBranch(int parent_id, cocos2d::ui::Button* node);
    // рисует ветки в процессе добавления
    void DrawTemporaryElements(float delta);
    void BuildBranches();
    
    DrawNode* draw_node_;   // рисованное дерево
    Node* grow_buttons_;
    std::vector<int> grow_buttons_ids_; // идентификаторы вершин кнопок
    Node* top_level_gui_;       // временные диалоги и прочее (затирается при муве)
    Node* gui_node_;            // специальная нода для гуи, она не скейлится
    Node* scale_node_;          // та самая нода которая всех скейлит, с нее можно брать scale
    Node* select_branch_elements_;  // элементы выделенной ветки
    
    Node* new_branch_mover_;    // перемещалка ветки
    cocos2d::ui::Button* new_branch_mover_src_;
    Vec2 new_branch_mover_delta_;
    
    // данные для временной отрисовки
    std::vector<Point> tmp_draw_branch_;

	TreePtr tree_;
    TreeVisu* tree_visu_;
};
