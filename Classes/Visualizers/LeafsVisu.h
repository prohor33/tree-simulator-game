#pragma once
#include "../TreeModel/TreeStructure.h"

class TreeVisu;

class LeafsVisu : public Layer {
public:
	static LeafsVisu* CreateLayer(const TreePtr& tree_interface, Node* scale_node, Node* gui_node, Node* top_level_gui, TreeVisu* tree_visu);
    virtual bool init() override;
    CREATE_FUNC(LeafsVisu);
    
    void update(float) override;
    void AddLeaf(int parent_id);
    
private:
	void Build();
    void DrawLeafs(float delta);
    
    Node* leafs_;
    Node* top_level_gui_;   // временные диалоги и прочее
    Node* gui_node_;            // специальная нода для гуи, она не скейлится
    Node* scale_node_;          // та самая нода которая всех скейлит, с нее можно брать scale

	TreePtr tree_;
    TreeVisu* tree_visu_;
};
