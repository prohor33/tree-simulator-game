#include "stdafx.h"
#include "TreeResources.h"

const ResourceAddingResult ResourceEntity::PlusRes(double val)
{
	if (max_res > 0 && cur_value + val > max_res)
	{
		cur_value = max_res;
		return ResourceAddingResult::ResourceResultMaxIsReached;
	}
	else
	{
		cur_value += val;
		return ResourceAddingResult::ResourceResultOK;
	}
}

const ResourceAddingResult ResourceEntity::MinusRes(double val)
{
	if (cur_value - val > max_res)
	{
		return ResourceAddingResult::ResourceResultMaxIsReached;
	}
	else
	{
		cur_value -= val;
		return ResourceAddingResult::ResourceResultOK;
	}
}

///////////////////////////////////////////////////////

double ResourceKeeper::GetProduction(const TreeElement& elem, TreeResourceType res_type)
{
	switch (elem.type)
	{
		// случай корня
	case TreePartType::Root:
	{
		switch (res_type)
		{
		case TreeResourceType::Water:
		{
			return (elem.length * root_water_production_per_length_unit);
			break;
		}
		default:
		{
			return 0;
			break;
		}
		}
		break;
	}
	// случай листа
	case TreePartType::Leaf:
	{
		switch (res_type)
		{
		case TreeResourceType::Glucose:
		{
			return (leaf_glucose_production_main_coef *
				std::min(elem.height * leaf_sunenergy_consumption_by_height, elem.width * leaf_glucose_production_by_water_width));
			break;
		}
		default:
		{
			return 0;
			break;
		}
		}
		break;
	}
	default:
	{
		return 0;
		break;
	}
	}
}

double ResourceKeeper::GetConsumption(const TreeElement& elem, TreeResourceType res_type)
{
	switch (elem.type)
	{
	// случай корня
	case TreePartType::Root:
	{
		switch (res_type)
		{
		case TreeResourceType::Glucose:
		{
			return (elem.length * root_glucose_consumption_per_length_unit);
			break;
		}
		default:
		{
			return 0;
			break;
		}
		}
		break;
	}
	// случай ветки
	case TreePartType::Branch:
	{
		switch (res_type)
		{
		case TreeResourceType::Glucose:
		{
			return (elem.length * elem.width * branch_glucose_consumption_by_area);
			break;
		}
		default:
		{
			return 0;
			break;
		}
		}
		break;
	}
	// случай листа
	case TreePartType::Leaf:
	{
		switch (res_type)
		{
		case TreeResourceType::SunEnergy:
		{
			return (elem.height * leaf_sunenergy_consumption_by_height);
			break;
		}
		case TreeResourceType::Water:
		{
			return (elem.width * leaf_glucose_production_by_water_width);
			break;
		}
		default:
		{
			return 0;
			break;
		}
		}
		break;
	}
	default:
	{
		return 0;
		break;
	}
	}

}

double ResourceKeeper::GetConduction(const TreeElement& elem, TreeResourceType res_type)
{
	switch (elem.type)
	{
	// случай ветки
	case TreePartType::Branch:
	{
		switch (res_type)
		{
		case TreeResourceType::Water:
		{
			return (elem.width * branch_water_conduction_per_width);
			break;
		}
		default:
		{
			return 0;
			break;
		}
		}
		break;
	}
	default:
	{
		return 0;
		break;
	}
	}
}

void ResourceKeeper::SetResourcesValues(double water_val, double glucose_val)
{
	water.SetExactValue(water_val);
	glucose.SetExactValue(glucose_val);
}

const double ResourceKeeper::GetCurrentResource(TreeResourceType type) const
{
	switch (type)
	{
	case TreeResourceType::Water:
	{
		return (water.GetValue());
		break;
	}
	case TreeResourceType::Glucose:
	{
		return (glucose.GetValue());
		break;
	}
	default:
	{
		return 0;
		break;
	}
	}
}

const ResourceAddingResult ResourceKeeper::AddResources(TreeResourceType type, double val)
{
	switch (type)
	{
	case TreeResourceType::Water:
	{
		return (val > 0 ? water.PlusRes(val) : water.MinusRes(-val));
		break;
	}
	case TreeResourceType::Glucose:
	{
		return (val > 0 ? glucose.PlusRes(val) : glucose.MinusRes(-val));
		break;
	}
	default:
	{
		return ResourceAddingResult::ResourceResultUndefined;
		break;
	}
	}
}

/////////////////////////////////////////////////////////////

bool ResourceInterface::MakeResources(double water_val, double glucose_val)
{
	resources_keeper.reset(new ResourceKeeper());
	resources_keeper->SetResourcesValues(water_val, glucose_val);

	return true;
}

const double ResourceInterface::GetCurrentResource(TreeResourceType type) const
{
	return resources_keeper->GetCurrentResource(type);
}

const ResourceAddingResult ResourceInterface::AddResources(TreeResourceType type, double val)
{
	return resources_keeper->AddResources(type, val);
}