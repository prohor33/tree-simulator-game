#pragma once

#include <vector>
#include <locale>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <functional>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#else
#define MOBILE
#endif
