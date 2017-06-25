#pragma once
#include "../TreeModel/TreeResources.h"
#include "../TreeModel/TreeStructure.h"

class MeadowVisu : public Layer {
public:
    //static Layer* CreateLayer(const TreeInterface& tree);
	static MeadowVisu* CreateLayer(const ResourcesPtr& resource, const TreePtr& tree);
    virtual bool init() override;
    CREATE_FUNC(MeadowVisu);
    
    void update(float) override;
    
private:
	void Build(const ResourcesPtr& resource, const TreePtr& tree);
    // обновляет количество ресурсов
    void UpdateInfo(float delta);
    
    Node* info_;        // количество ресурсов
    Label* water_res_lbl_;
    Label* water_res_lbl_arrow_;
    Label* glucose_res_lbl_;
    Label* glucose_res_lbl_arrow_;

	ResourcesPtr resource_;
    TreePtr tree_;
};
