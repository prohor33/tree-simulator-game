#pragma once
#include "../TreeModel/TreeStructure.h"

class TreeVisu;

class ResourceConsVisu : public Layer {
public:
    //static Layer* CreateLayer(const TreeInterface& tree);
	static ResourceConsVisu* CreateLayer(const TreePtr& tree_interface, TreeVisu* tree_visu);
    virtual bool init() override;
    CREATE_FUNC(ResourceConsVisu);
    
private:
	void Build(const TreePtr& tree, TreeVisu* tree_visu);
    void Update(float dt);
    // показывает анимацию траты/появления ресурса
    void ShowResCons(int id, TreeResourceType res_type, double value);

	TreePtr tree_;
    TreeVisu* tree_visu_;
    // id -> res_type -> (prod - cons) которая еще не визуализирована
    std::map<int, std::map<TreeResourceType, double>> elements_prod_;
};
