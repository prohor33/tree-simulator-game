#pragma once
#include "../TreeModel/TreeStructure.h"

class LeafsVisu;
class BranchesVisu;
class ResourceConsVisu;

class TreeVisu : public Layer {
public:
    //static Layer* CreateLayer(const TreeInterface& tree);
	static TreeVisu* CreateLayer(const TreePtr& tree_interface, Node* scale_node, Node* gui_node);
    virtual bool init() override;
    CREATE_FUNC(TreeVisu);
    
    void update(float) override;
    void AddLeaf(int parent_id);
    void OnScaleOrMove();   // увеличение/сдвиг
    
private:
	void Build(const TreePtr& tree);
    void BuildTreeRoot();

    void DrawTreeRoot(float delta);
    
    Node* tree_root_;
    Node* top_level_gui_;       // временные диалоги и прочее
    Node* tree_root_buttons_;
    Node* gui_node_;            // специальная нода для гуи, она не скейлится
    Node* scale_node_;          // та самая нода которая всех скейлит, с нее можно брать scale
    
	TreePtr tree_;
    LeafsVisu* leafs_visu_;
    BranchesVisu* branches_visu_;
    // визуализирует расход ресурсов элементами
    ResourceConsVisu* res_cons_visu_;
};
