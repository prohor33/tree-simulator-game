#ifndef TapGestureRecognizer_h
#define TapGestureRecognizer_h

#include "GestureRecognizer.h"

#define kTapMaxDuration 150
#define kTapMaxDurationBetweenTaps 220
#define kTapMaxDistance 20
#define kTapMaxDistanceBetweenTaps 20

class TapGestureRecognizer : public GestureRecognizer
{
public:
    bool init();
    ~TapGestureRecognizer();
    CREATE_FUNC(TapGestureRecognizer);
    
    virtual bool onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Event * pEvent){};
    virtual void onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
protected:
    CC_SYNTHESIZE(unsigned int, numberOfTapsRequired, NumberOfTapsRequired);
private:
    int taps;
    cocos2d::Point initialPosition, finalPosition;
    
    std::chrono::high_resolution_clock::time_point startTime, endTime;
    
    void stopGestureRecognition();
};

#endif
