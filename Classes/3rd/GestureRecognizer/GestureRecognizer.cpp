#include "GestureRecognizer.h"

USING_NS_CC;

GestureRecognizer::GestureRecognizer()
{
    isRecognizing = false;
    
    dispatcher = Director::getInstance()->getEventDispatcher();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GestureRecognizer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GestureRecognizer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GestureRecognizer::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    setCancelsTouchesInView(false);
}

GestureRecognizer::~GestureRecognizer()
{
    
}

void GestureRecognizer::setTarget(Ref * tar, SEL_CallFuncGR sel)
{
    target = tar;
    selector = sel;
}

void GestureRecognizer::setTarget(const std::function<void(CCGesture *)> &callback)
{
    this->callback = callback;
}

void GestureRecognizer::setTargetForBegan(Ref * tar, SEL_CallFuncGR sel)
{
    targetForBegan = tar;
    selectorForBegan = sel;
}

void GestureRecognizer::setTargetForBegan(const std::function<void(CCGesture *)> &callback)
{
    this->callbackForBegan = callback;
}

void GestureRecognizer::setTargetForEnded(Ref * tar, SEL_CallFuncGR sel)
{
    targetForEnded = tar;
    selectorForEnded = sel;
}

void GestureRecognizer::setTargetForEnded(const std::function<void(CCGesture *)> &callback)
{
    this->callbackForEnded = callback;
}

float GestureRecognizer::distanceBetweenPoints(Point p1, Point p2)
{
    return p2.getDistance(p1);
}


void GestureRecognizer::stopTouchesPropagation(Event * pEvent)
{
    pEvent->stopPropagation();
}

void GestureRecognizer::setParent(Node*p)
{
    Layer::setParent(p);
    
    if (p!=NULL) {
        Size size = p->getContentSize();
        setContentSize(size);
        setPosition(p->getPosition());
        frame = p->boundingBox();
    }
}

bool GestureRecognizer::isPositionBetweenBounds(Point pos)
{
    return frame.containsPoint(pos);
}

void GestureRecognizer::gestureRecognized(CCGesture * gesture)
{
    if (callback) callback(gesture);
    else if (target && selector) (target->*selector)(gesture); //call selector
}

void GestureRecognizer::gestureBegan(CCGesture * gesture)
{
    if (callbackForBegan) callbackForBegan(gesture);
    else if (targetForBegan && selectorForBegan) (targetForBegan->*selectorForBegan)(gesture); //call selector
}

void GestureRecognizer::gestureEnded(CCGesture * gesture)
{
    if (callbackForEnded) callbackForEnded(gesture);
    else if (targetForEnded && selectorForEnded) (targetForEnded->*selectorForEnded)(gesture); //call selector
}
