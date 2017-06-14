#ifndef PanGestureRecognizer_h
#define PanGestureRecognizer_h

#include "GestureRecognizer.h"

//this class is used for storing information about the pan gesture
class CCPan : public CCGesture
{
public:
    CREATE_FUNC(CCPan);
    cocos2d::Point delta;
};

class PanGestureRecognizer : public GestureRecognizer {
public:
    bool init();
    CREATE_FUNC(PanGestureRecognizer);
    
    virtual bool onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
};

#endif
