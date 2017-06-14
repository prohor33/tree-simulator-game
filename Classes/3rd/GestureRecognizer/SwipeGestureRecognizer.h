#ifndef SwipeGestureRecognizer_h
#define SwipeGestureRecognizer_h

#include "GestureRecognizer.h"
#include <chrono>

#define kSwipeMaxDuration 300
#define kSwipeMinDistance 60

typedef enum {
    kSwipeGestureRecognizerDirectionRight = 1 << 0,
    kSwipeGestureRecognizerDirectionLeft  = 1 << 1,
    kSwipeGestureRecognizerDirectionUp    = 1 << 2,
    kSwipeGestureRecognizerDirectionDown  = 1 << 3
} SwipeGestureRecognizerDirection;

//this class is used for storing information about the swipe gesture
class CCSwipe : public CCGesture
{
public:
    CREATE_FUNC(CCSwipe);
    SwipeGestureRecognizerDirection direction;
};

class SwipeGestureRecognizer : public GestureRecognizer
{
public:
    bool init();
    ~SwipeGestureRecognizer();
    CREATE_FUNC(SwipeGestureRecognizer);
    
    virtual bool onTouchBegan(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
    virtual void onTouchMoved(cocos2d::Touch * pTouch, cocos2d::Event * pEvent){};
    virtual void onTouchEnded(cocos2d::Touch * pTouch, cocos2d::Event * pEvent);
protected:
    CC_SYNTHESIZE(int, direction, Direction);
    
private:
    cocos2d::Point initialPosition;
    //struct cocos2d::cc_timeval startTime;
    std::chrono::high_resolution_clock::time_point startTime;
    bool checkSwipeDirection(cocos2d::Point p1, cocos2d::Point p2, int & dir);
};

#endif
