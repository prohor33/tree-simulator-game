#pragma once
#include "../TreeModel/TreeStructure.h"

class TreeVisu;

class LeafsVisu : public Layer {
public:
	static LeafsVisu* CreateLayer(const TreePtr& tree_interface, Node* top_level_gui, TreeVisu* tree_visu);
    virtual bool init() override;
    CREATE_FUNC(LeafsVisu);
    
    void update(float) override;
    void AddLeaf(int parent_id);
    
private:
	void Build(const TreePtr& tree, Node* top_level_gui, TreeVisu* tree_visu);
    void DrawLeafs(float delta);
    
    Node* leafs_;
    Node* top_level_gui_;   // временные диалоги и прочее

	TreePtr tree_;
    TreeVisu* tree_visu_;
};
