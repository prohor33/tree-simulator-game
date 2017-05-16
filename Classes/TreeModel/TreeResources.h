#pragma once
#include "TreeCommon.h"


const double root_water_production_per_length_unit = 1.5;
const double root_glucose_consumption_per_length_unit = 1.3;

const double leaf_glucose_production_main_coef = 1.0;
const double leaf_glucose_production_by_water_width = 1.0;
const double leaf_sunenergy_consumption_by_height = 1.0;

const double branch_glucose_consumption_by_area = 0.1;
const double branch_water_conduction_per_width = 1.2;

enum ResourceAddingResult
{
	ResourceResultUndefined = 0,
	ResourceResultOK = 1,
	ResourceResultMaxIsReached = 2,
	ResourceResultNotEnough = 3
};

class ResourceEntity
{
public:
	ResourceEntity(TreeResourceType t) : res_type(t), cur_value(0), max_res(1000), min_res(0) {};
	ResourceEntity(TreeResourceType t, double val) : res_type(t), cur_value(val), max_res(-1), min_res(0) {};

	const ResourceAddingResult PlusRes(double val);
	const ResourceAddingResult MinusRes(double val);

	void SetMinPossibleRes(double val) { min_res = val; }
	void SetMaxPossibleRes(double val) { max_res = val; }

	void SetExactValue(double val) { cur_value = val; }
	void SetExactType(TreeResourceType t) { res_type = t; }

	const double GetValue() const { return cur_value; }
	const TreeResourceType GetResourceType() { return res_type; }

private:
	TreeResourceType res_type;
	double cur_value;
	double max_res, min_res;
};

class ResourceKeeper
{
public:
	ResourceKeeper() : water(TreeResourceType::Water), glucose(TreeResourceType::Glucose) {};

	// получить продукцию конкретного ресурса на конкретном элементе дерева
	static double GetProduction(const TreeElement& elem, TreeResourceType type);
	// получить потребление конкретного ресурса на конкретном элементе дерева
	static double GetConsumption(const TreeElement& elem, TreeResourceType type);
	// получить "проводимость" конкретного ресурса на конкретном элементе дерева
	static double GetConduction(const TreeElement& elem, TreeResourceType type);

	// установить первоначальные значения ресурсов
	void SetResourcesValues(double water_val, double glucose_val);
	// получить текущее состоянии ресурсов
	const double GetCurrentResource(TreeResourceType type) const;
	// добавить конкретный ресурс
	const ResourceAddingResult AddResources(TreeResourceType type, double val);

private:
	ResourceEntity water;
	ResourceEntity glucose;
};

class ResourceInterface
{
public:

	ResourceInterface() : resources_keeper(nullptr) {}

	bool MakeResources(double water_val, double glucose_val);

	// получить текущее состоянии ресурсов
	const double GetCurrentResource(TreeResourceType type) const;
	// добавить конкретный ресурс
	const ResourceAddingResult AddResources(TreeResourceType type, double val);

private:
	std::shared_ptr<ResourceKeeper> resources_keeper;
};


using ResourcesPtr = std::shared_ptr<ResourceInterface>;
