#include "PanGestureRecognizer.h"

USING_NS_CC;

bool PanGestureRecognizer::init()
{
    return true;
}

bool PanGestureRecognizer::onTouchBegan(Touch * pTouch, Event * pEvent)
{
    if (isRecognizing) {
        isRecognizing = false;
        return false;
    }
    
    Point loc = pTouch->getLocation();
    if (!isPositionBetweenBounds(loc)) return false;
    
    isRecognizing = true;
    
    CCPan * pan = CCPan::create();
    pan->location = pTouch->getLocation();
    pan->delta = Point(0,0);
    
    gestureBegan(pan);
    return true;
}

void PanGestureRecognizer::onTouchMoved(Touch * pTouch, Event * pEvent)
{
    CCPan * pan = CCPan::create();
    pan->location = pTouch->getLocation();
    pan->delta = pTouch->getDelta();
    gestureRecognized(pan);
}

void PanGestureRecognizer::onTouchEnded(Touch * pTouch, Event * pEvent)
{
    CCPan * pan = CCPan::create();
    pan->location = pTouch->getLocation();
    pan->delta = pTouch->getDelta();
    pan->cancelPropagation = cancelsTouchesInView;
    
    gestureEnded(pan);
    
    isRecognizing = false;
    
    //cancel touch over other views if necessary
    if (pan->cancelPropagation) {
        stopTouchesPropagation(pEvent);
    }
}
