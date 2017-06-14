#include "TapGestureRecognizer.h"

USING_NS_CC;

bool TapGestureRecognizer::init()
{
    setNumberOfTapsRequired(1);
    taps = 0;
    return true;
}

TapGestureRecognizer::~TapGestureRecognizer()
{
    
}

bool TapGestureRecognizer::onTouchBegan(Touch * pTouch, Event * pEvent)
{
    if (isRecognizing && taps==0) {
        stopGestureRecognition();
        return false;
    }
    
    initialPosition = pTouch->getLocation();
    
    if (!isPositionBetweenBounds(initialPosition)) return false;
    
    startTime = getCurrentTime();
    
    if (taps>0 && taps<numberOfTapsRequired) {
        float distance = distanceBetweenPoints(finalPosition, initialPosition); //distance between taps
        double duration = getTimeDifference(startTime, endTime); //duration between taps
        if (duration>kTapMaxDurationBetweenTaps || distance>kTapMaxDistanceBetweenTaps) {
            stopGestureRecognition();
        }
    }
    
    isRecognizing = true;
    return true;
}

void TapGestureRecognizer::onTouchEnded(Touch * pTouch, Event * pEvent)
{
    //calculate duration
    endTime = getCurrentTime();
    double duration = getTimeDifference(endTime, startTime); //duration of tap in milliseconds
    
    //calculate distance
    finalPosition = pTouch->getLocation();
    float distance = distanceBetweenPoints(initialPosition, finalPosition);
    
    //tap was successful
    if (duration<=kTapMaxDuration && distance<=kTapMaxDistance) {
        taps++;
        if (taps==numberOfTapsRequired) {
            CCGesture * tap = CCGesture::create();
            tap->location = initialPosition;
            tap->cancelPropagation = cancelsTouchesInView;
            
            gestureRecognized(tap);
            if (tap->cancelPropagation) stopTouchesPropagation(pEvent); //cancel touch over other views
            stopGestureRecognition();
        }
    }
    else {
        stopGestureRecognition();
    }
}

void TapGestureRecognizer::stopGestureRecognition()
{
    taps = 0;
    isRecognizing = false;
}
