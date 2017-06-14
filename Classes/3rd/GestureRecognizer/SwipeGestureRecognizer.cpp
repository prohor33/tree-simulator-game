#include "SwipeGestureRecognizer.h"
#include <iostream>

USING_NS_CC;

bool SwipeGestureRecognizer::init()
{
    direction = 0;
    return true;
}

SwipeGestureRecognizer::~SwipeGestureRecognizer()
{
    
}

bool SwipeGestureRecognizer::checkSwipeDirection(Point p1, Point p2, int & dir)
{
    bool right = p2.x-p1.x>=kSwipeMinDistance;
    bool left = p1.x-p2.x>=kSwipeMinDistance;
    bool down = p1.y-p2.y>=kSwipeMinDistance;
    bool up = p2.y-p1.y>=kSwipeMinDistance;
    
    if (right) {
        if ((direction & kSwipeGestureRecognizerDirectionRight) == kSwipeGestureRecognizerDirectionRight) {
            dir = kSwipeGestureRecognizerDirectionRight;
            return true;
        }
    }
    else if (left) {
        if ((direction & kSwipeGestureRecognizerDirectionLeft) == kSwipeGestureRecognizerDirectionLeft) {
            dir = kSwipeGestureRecognizerDirectionLeft;
            return true;
        }
    }
    else if (up) {
        if ((direction & kSwipeGestureRecognizerDirectionUp) == kSwipeGestureRecognizerDirectionUp) {
            dir = kSwipeGestureRecognizerDirectionUp;
            return true;
        }
    }
    else if (down) {
        if ((direction & kSwipeGestureRecognizerDirectionDown) == kSwipeGestureRecognizerDirectionDown) {
            dir = kSwipeGestureRecognizerDirectionDown;
            return true;
        }
    }
    
    return false;
}

bool SwipeGestureRecognizer::onTouchBegan(Touch * pTouch, Event * pEvent)
{
    if (isRecognizing) {
        isRecognizing = false;
        return false;
    }
    
    initialPosition = pTouch->getLocation();
    if (!isPositionBetweenBounds(initialPosition)) return false;
    
    startTime = getCurrentTime();
    isRecognizing = true;
    return true;
}

void SwipeGestureRecognizer::onTouchEnded(Touch * pTouch, Event * pEvent)
{
    Point finalPosition = pTouch->getLocation();
    if (!isPositionBetweenBounds(finalPosition)) {
        isRecognizing = false;
        return;
    }
    
    //distance between initial and final point of touch
    float distance = finalPosition.getDistance(initialPosition);
    
    std::chrono::high_resolution_clock::time_point currentTime = getCurrentTime();
    double duration = getTimeDifference(currentTime, startTime);
    
    //check that minimum distance has been reached
    //check that maximum duration hasn't been reached
    //check if the direction of the swipe is correct
    int dir = 0;
    if (distance>=kSwipeMinDistance && duration<=kSwipeMaxDuration && checkSwipeDirection(initialPosition,finalPosition,dir)) {
        CCSwipe * swipe = CCSwipe::create();
        swipe->direction = (SwipeGestureRecognizerDirection)dir;
        swipe->location = initialPosition;
        swipe->cancelPropagation = cancelsTouchesInView;
        
        gestureRecognized(swipe);
        if (swipe->cancelPropagation) stopTouchesPropagation(pEvent); //cancel touch over other views
    }
    
    isRecognizing = false;
}
