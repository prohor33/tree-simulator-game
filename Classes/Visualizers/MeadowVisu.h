#pragma once
#include "../TreeModel/TreeResources.h"

class MeadowVisu : public Layer {
public:
    //static Layer* CreateLayer(const TreeInterface& tree);
	static Layer* CreateLayer(const ResourcesPtr& resource);
    virtual bool init() override;
    CREATE_FUNC(MeadowVisu);
    
    void update(float) override;
    
private:
	void Build(const ResourcesPtr& resource);
    // обновляет количество ресурсов
    void UpdateInfo(float delta);
    
    Node* info_;        // количество ресурсов
    Label* water_res_lbl_;
    Label* sun_res_lbl_;
    Label* glucose_res_lbl_;

	ResourcesPtr resource_;
};
