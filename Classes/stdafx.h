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


typedef std::function<void(cocos2d::Node* node)> OnClickF;
typedef std::function<void(cocos2d::Node* node, cocos2d::Point p)> OnClickLocationF;
typedef std::function<void(cocos2d::ui::Button* node)> OnClickButtonF;
typedef std::function<void(cocos2d::Touch* touch, cocos2d::Event* event)> OnMoveF;
typedef std::function<void(bool ok)> OnResult;
