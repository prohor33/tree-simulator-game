#pragma once

#include "GestureRecognizer.h"
#include <vector>

#define kPinchThreshold 2.0

typedef enum {
    kPinchGestureRecognizerTypeClose    = 1 << 0,
    kPinchGestureRecognizerTypeOpen     = 1 << 1
} PinchGestureRecognizerType;

//this class is used for storing information about the pinch gesture
class CCPinch : public CCGesture {
public:
    bool init() {return true;}
    CREATE_FUNC(CCPinch);
    PinchGestureRecognizerType type;
    float coef;
    cocos2d::Point position;
};

class PinchGestureRecognizer : public GestureRecognizer {
public:
    bool init();
    CREATE_FUNC(PinchGestureRecognizer);
    
    virtual bool onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    
private:
    float lastDistance;
    std::vector<cocos2d::Touch*> touches;
};
