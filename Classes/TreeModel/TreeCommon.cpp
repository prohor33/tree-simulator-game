#include "stdafx.h"
#include "TreeCommon.h"



void TreeElement::MoveElement(const Vec2& move_vector)
{
	start_point += move_vector;
	end_point += move_vector;
}

void TreeElement::StretchElement(float coef, Vec2& stretch_vector)
{
	stretch_vector = (end_point - start_point) * coef;
	end_point += stretch_vector;
}

//////////////////////////////////////////////////////////

std::string to_str(TreeResourceType res_type) {
    switch (res_type) {
        case Water:
            return "Water";
        case SunEnergy:
            return "Sun";
        case Glucose:
            return "Glucose";
        case ResourceUndefined:
            return "";
    }
    return "";
}
