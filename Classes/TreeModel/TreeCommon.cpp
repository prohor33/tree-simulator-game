#include "stdafx.h"
#include "TreeCommon.h"

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
