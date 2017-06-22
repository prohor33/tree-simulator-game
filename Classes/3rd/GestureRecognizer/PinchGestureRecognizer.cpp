#include "PinchGestureRecognizer.h"

USING_NS_CC;

bool PinchGestureRecognizer::init() {
    lastDistance = -1.f;
    
    return true;
}

bool PinchGestureRecognizer::onTouchBegan(Touch * pTouch, Event * pEvent) {
    if (!isPositionBetweenBounds(pTouch->getLocation())) {
        return false;
    }
    
    touches.erase(std::remove_if(touches.begin(), touches.end(), [] (Touch* t) {
        if (t->getReferenceCount() <= 1) {
            t->release();
            return true;
        }
        return false;
    }), touches.end());
    
    if (touches.size() >= 2)
        return false;
    
    pTouch->retain();
    touches.push_back(pTouch);
    
    //start recognizing after that 2 fingers are touching
    if (touches.size() >= 2) {
        isRecognizing = true;
    }
    
    return true;
}

void PinchGestureRecognizer::onTouchMoved(Touch * pTouch, Event * pEvent) {
    if (!isRecognizing || touches.size() < 2) {
        return;
    }
    
    Touch* touch1 = (Touch*)touches[0];
    Touch* touch2 = (Touch*)touches[1];
    Point delta1 = touch1->getDelta();
    Point delta2 = touch2->getDelta();
    
//    if ((fabs(delta1.x)<kPinchThreshold && fabs(delta1.y)<kPinchThreshold) || (fabs(delta2.x)<kPinchThreshold && fabs(delta2.y)<kPinchThreshold)) {
//        return;
//    }
    
    float distance = distanceBetweenPoints(touch1->getLocation(), touch2->getLocation());
    if (distance > (float)1e6) {
        return;
    }
    distance = std::max(distance, 0.001f);
    if (lastDistance < 0.f || fabs(lastDistance) <  MATH_FLOAT_SMALL) {
        lastDistance = distance;
        return;
    }
    
    CCPinch* pinch = CCPinch::create();
    
    // decide type of pinch
    if (lastDistance <= distance) {
        pinch->type = kPinchGestureRecognizerTypeOpen;
    } else {
        pinch->type = kPinchGestureRecognizerTypeClose;
    }
    if (fabs(lastDistance) > MATH_FLOAT_SMALL) {
        pinch->coef = fabs(distance / lastDistance);
    } else {
        pinch->coef = 1.f;
    }
    pinch->coef = std::min(pinch->coef, 10.f);
    pinch->coef = std::max(pinch->coef, 0.1f);
    assert(!isnan(pinch->coef));
    pinch->position = (touch1->getLocation() + touch2->getLocation()) / 2.f;
    lastDistance = distance;
    
    gestureRecognized(pinch);
}

void PinchGestureRecognizer::onTouchEnded(Touch * pTouch, Event * pEvent) {
    
    touches.erase(std::remove_if(touches.begin(), touches.end(), [=] (Touch* t) {
        return t == pTouch;
    }), touches.end());
    
    if (touches.size() < 2) {
        isRecognizing = false;
        lastDistance = -1.f;
    }
    
    //touches(pTouch);
    
    //cancel touch over other views if necessary
    if (cancelsTouchesInView) {
        //stopTouchesPropagation(createSetWithTouch(pTouch), pEvent);
    }
}
