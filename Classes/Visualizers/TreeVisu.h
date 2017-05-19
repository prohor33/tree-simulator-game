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
    void BuildTreeRoot();
    // рисует кнопки в точках возможного роста
    void DrawGrowButtons(float delta);
    void GrowButtonOnClick(size_t button_i, const Vec2& src_pos, const Size& src_size);
    void OnAddLeaf(int parent_id);
    void DrawLeafs(float delta);
    // вызывается при клике на иконку ветку добавления
    void OnStartAddingBranch(int parent_id, cocos2d::ui::Button* node);
    // вызывается когда пользователь решил добавить ветку
    void OnAddBranch(int parent_id, const Vec2& b, const Vec2& e);
    // рисует ветки в процессе добавления
    void DrawTemporaryElements(float delta);
    void DrawTreeRoot(float delta);
    
    DrawNode* draw_node_;   // рисованное дерево
    Node* leafs_;
    Node* tree_root_;
    Node* gui_root_;        // кнопочки на дереве
    Node* grow_buttons_;
    Node* tree_root_buttons_;
    std::vector<int> grow_buttons_ids_; // идентификаторы вершин кнопок
    Node* top_level_gui_;   // временные диалоги и прочее
    
    // данные для временной отрисовки
    std::vector<Point> tmp_draw_branch_;

	TreePtr tree_;
};
