#include "LongPressGestureRecognizer.h"

USING_NS_CC;

bool LongPressGestureRecognizer::init()
{
    isRecognizing = false;
    currTouch = NULL;
    currEvent = NULL;
    
    setMinimumPressDuration(kLongPressMinDuration);
    
    return true;
}

LongPressGestureRecognizer::~LongPressGestureRecognizer()
{
    
}

void LongPressGestureRecognizer::timerDidEnd(float dt)
{
    // check if the current touch is near the original touch
    float distance = currLocation.getDistance(origLocation);
    if (distance > kLongPressDistanceTolerance) {
        stopGestureRecognition();
        return;
    }
    
    CCGesture * longPress = CCGesture::create();
    longPress->location = currLocation;
    longPress->cancelPropagation = cancelsTouchesInView;
    
    gestureRecognized(longPress);
    if (longPress->cancelPropagation) stopTouchesPropagation(currEvent); //cancel touch over other views
    
    stopGestureRecognition();
}

bool LongPressGestureRecognizer::onTouchBegan(Touch * pTouch, Event * pEvent)
{
    if (isRecognizing) {
        stopGestureRecognition();
        return false;
    }
    
    origLocation = currLocation = pTouch->getLocation();
    if (!isPositionBetweenBounds(origLocation)) return false;
    
    currEvent = pEvent;
    currTouch = pTouch;
    
    schedule(schedule_selector(LongPressGestureRecognizer::timerDidEnd), minimumPressDuration);
    
    isRecognizing = true;
    return true;
}

void LongPressGestureRecognizer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    currLocation = pTouch->getLocation();
}

void LongPressGestureRecognizer::onTouchEnded(Touch * pTouch, Event * pEvent)
{
    stopGestureRecognition();
}

void LongPressGestureRecognizer::stopGestureRecognition()
{
    if (!isRecognizing) return;
    
    currTouch = NULL;
    currEvent = NULL;
    unschedule(schedule_selector(LongPressGestureRecognizer::timerDidEnd));
    isRecognizing = false;
}
