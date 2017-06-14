#ifndef LongPressGestureRecognizer_h
#define LongPressGestureRecognizer_h

#include "GestureRecognizer.h"

#define kLongPressMinDuration 0.5
#define kLongPressDistanceTolerance 20

class LongPressGestureRecognizer : public GestureRecognizer
{
public:
    bool init();
    ~LongPressGestureRecognizer();
    CREATE_FUNC(LongPressGestureRecognizer);
    void timerDidEnd(float dt);
    
    virtual bool onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
protected:
    CC_SYNTHESIZE(float, minimumPressDuration, MinimumPressDuration);
private:
    cocos2d::Point origLocation, currLocation;
    cocos2d::Touch * currTouch;
    cocos2d::Event * currEvent;
    
    void stopGestureRecognition();
};

#endif
