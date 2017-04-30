#include "stdafx.h"
#include "Utils.h"

USING_NS_CC;
using namespace cocos2d::ui;

namespace visu_utils {

Sprite* LoadSpriteByHeight(float h, const char* file, Vec2 p) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Sprite* s = Sprite::create(file);
    
    float coef = visible_size.height / s->getContentSize().height * h;
    Size size = s->getContentSize();
    size = size * coef;
    s->setScale(coef);
    
    p.x *= visible_size.width;
    p.y *= visible_size.height;
    p += origin;
    s->setPosition(p);
    return s;
}

void ChangeSizeByHeight(Node* s, float h) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float coef = visible_size.height / s->getContentSize().height * h;
    Size size = s->getContentSize();
    size = size * coef;
    s->setScale(coef);
}

void ChangeSizeByWidth(Node* s, float w) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float coef = visible_size.width / s->getContentSize().width * w;
    Size size = s->getContentSize();
    size = size * coef;
    s->setScale(coef);
}


Sprite* LoadSpriteByWidth(float w, const char* file, Vec2 p) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Sprite* s = Sprite::create(file);
    
    float coef = visible_size.width / s->getContentSize().width * w;
    Size size = s->getContentSize();
    size = size * coef;
    s->setScale(coef);
    
    p.x *= visible_size.width;
    p.y *= visible_size.height;
    p += origin;
    s->setPosition(p);
    return s;
}

void ToBottom(Node* s, float shift_from_bottom, bool pxl) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto p = s->getPosition();
    auto size = s->getBoundingBox().size;
    p.y = origin.y + size.height / 2 + shift_from_bottom * (pxl ? 1.f : visible_size.height);
    s->setPosition(p);
}

void ToTop(Node* s, float shift_from_top, bool pxl) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto p = s->getPosition();
    auto size = s->getBoundingBox().size;
    p.y = origin.y + visible_size.height - size.height / 2 - shift_from_top * (pxl ? 1.f : visible_size.height);
    s->setPosition(p);
}

void ToLeft(Node* s, float shift_from_left, bool pxl) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto p = s->getPosition();
    auto size = s->getBoundingBox().size;
    p.x = origin.x + size.width / 2 + shift_from_left * (pxl ? 1.f : visible_size.width);
    s->setPosition(p);
}

void ToRight(Node* s, float shift_from_right, bool pxl) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto p = s->getPosition();
    auto size = s->getBoundingBox().size;
    p.x = origin.x + visible_size.width - size.width / 2 - shift_from_right * (pxl ? 1.f : visible_size.width);
    s->setPosition(p);
}

void ChangeWidth(Node* s, float w) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    auto size = s->getContentSize();
    s->setScaleX(w * visible_size.width / size.width);
}

void ChangeHeight(Node* s, float h) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    auto size = s->getContentSize();
    s->setScaleY(h * visible_size.height / size.height);
}

Sprite* LoadSpriteFullScreen(const char* file, float target_s) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Sprite* s = Sprite::create(file);
    
    float coef_x = visible_size.width / s->getContentSize().width * target_s;
    float coef_y = visible_size.height / s->getContentSize().height * target_s;
    
    float coef = std::max<float>(coef_x, coef_y);
    Size size = s->getContentSize();
    size = size * coef;
    s->setScale(coef);
    
    Vec2 p(visible_size.width / 2, visible_size.height / 2);
    p += origin;
    s->setPosition(p);
    return s;
}

void MoveX(Node* s, float x) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    auto p = s->getPosition();
    p.x += x * visible_size.width;
    s->setPosition(p);
}

void MoveY(Node* s, float y) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    auto p = s->getPosition();
    p.y += y * visible_size.height;
    s->setPosition(p);
}

void MoveXPxl(Node* s, float x) {
    auto p = s->getPosition();
    p.x += x;
    s->setPosition(p);
}

void MoveYPxl(Node* s, float y) {
    auto p = s->getPosition();
    p.y += y;
    s->setPosition(p);
}

void MoveToXCenter(Node* s, float x) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    s->setPositionX(origin.x + visible_size.width * x);
}

void MoveToYCenter(Node* s, float y) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    s->setPositionY(origin.y + visible_size.height * y);
}

void MoveToCenter(Node* s, float x, float y) {
    MoveToXCenter(s, x);
    MoveToYCenter(s, y);
}

void TransferToParent(Node* old_parent, Node* new_parent, Node* target) {
    auto p0 = old_parent->convertToWorldSpace(target->getPosition());
    p0 = new_parent->convertToNodeSpace(p0);
    target->retain();
    old_parent->removeChild(target);
    new_parent->addChild(target);
    target->release();
    target->setPosition(p0);
}

Vec2 GetPositionInParent(Node* old_parent, Node* new_parent, Vec2 p) {
    // здесь учитываются все преобразования, включая растяжени и тп
    auto p0 = old_parent->convertToWorldSpace(p);
    return new_parent->convertToNodeSpace(p0);
}

Vec2 GetPositionInParent(Node* old_parent, Vec2 new_parent_world_p, Vec2 p) {
    // передаем только положение, чтобы не учитывать растяжение и пр
    auto p0 = old_parent->convertToWorldSpace(p);
    return p0 - new_parent_world_p;
}

Vec2 GetPositionInParent(Vec2 old_parent_world_p, Node* new_parent, Vec2 p) {
    // передаем только положение, чтобы не учитывать растяжение и пр
    return new_parent->convertToNodeSpace(old_parent_world_p + p);
}

void RepeatSpriteToWidth(Sprite* s, float width) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    
    Texture2D::TexParams tRepeatParams;
    tRepeatParams.magFilter = tRepeatParams.minFilter = GL_LINEAR;
    tRepeatParams.wrapS = GL_REPEAT;
    tRepeatParams.wrapT = GL_REPEAT;
    s->getTexture()->setTexParameters(tRepeatParams);
    auto rect = s->getTextureRect();
    s->setTextureRect(Rect(rect.origin.x, rect.origin.y, visible_size.width * width / s->getScaleX(), rect.size.height));
}

void RepeatSpriteToHeight(Sprite* s, float height) {
    Size visible_size = Director::getInstance()->getVisibleSize();
    
    Texture2D::TexParams tRepeatParams;
    tRepeatParams.magFilter = tRepeatParams.minFilter = GL_LINEAR;
    tRepeatParams.wrapS = GL_REPEAT;
    tRepeatParams.wrapT = GL_REPEAT;
    s->getTexture()->setTexParameters(tRepeatParams);
    auto rect = s->getTextureRect();
    s->setTextureRect(Rect(rect.origin.x, rect.origin.y, rect.size.width, visible_size.height * height / s->getScaleY()));
}

void AddDebugPositionLogger(Sprite* s) {
    auto listener = EventListenerTouchOneByOne::create();
    // preventing other listeners from using it
    listener->setSwallowTouches(true);
    
    // trigger when you push down
    listener->onTouchBegan = [&](Touch* touch, Event* event){
        auto s = static_cast<Sprite*>(event->getCurrentTarget());
        
        //Get the position of the current point relative to the button
        Point locationInNode = s->convertToNodeSpace(touch->getLocation());
        Size size = s->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        
        //Check the click area
        if (!rect.containsPoint(locationInNode))
            return false;
        
        return true; // if you are consuming it
    };
    
    listener->onTouchMoved = [=](Touch* touch, Event* event) {
        // for debug
        
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        target->setPosition(target->getPosition() + touch->getDelta());
        auto p = target->getPosition();
        
        Size visible_size = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        log("pos: (%.2lf, %.2lf)", p.x, p.y);
    };
    
    listener->onTouchEnded = [=](Touch* touch, Event* event){
        return true;
    };
    
    s->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, s);
}


void AddOnClickListener(Node* s, OnClickF on_click, double on_touch_coef, OnClickLocationF on_click_location) {
    auto listener = EventListenerTouchOneByOne::create();
    // preventing other listeners from using it
    listener->setSwallowTouches(true);
    
    // trigger when you push down
    listener->onTouchBegan = [=](Touch* touch, Event* event){
        auto s = static_cast<Node*>(event->getCurrentTarget());
        
        //Get the position of the current point relative to the button
        Point locationInNode = s->convertToNodeSpace(touch->getLocation());
        Size size = s->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        
        //Check the click area
        if (!rect.containsPoint(locationInNode))
            return false;
        
        if (on_click_location)
            on_click_location(s, locationInNode);
        
//        Sound::instance()->Play(menu_select_effect);
        
        float scale = s->getScale();
        scale *= on_touch_coef;
        s->setScale(scale);
        
        return true; // if you are consuming it
    };
    
    listener->onTouchMoved = [=](Touch* touch, Event* event) {
    };
    
    listener->onTouchEnded = [=](Touch* touch, Event* event){
        auto s = static_cast<Node*>(event->getCurrentTarget());
        
        float scale = s->getScale();
        scale /= on_touch_coef;
        s->setScale(scale);
        
        //Get the position of the current point relative to the button
        Point locationInNode = s->convertToNodeSpace(touch->getLocation());
        Size size = s->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        
        //Check the click area
        if (!rect.containsPoint(locationInNode))
            return false;
        
        if (on_click)
            on_click(event->getCurrentTarget());
        return true;
    };
    
    s->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, s);
}


void AddOnClickListener(Button* s, OnClickButtonF on_click, double on_touch_coef) {
    
    s->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type){
        switch (type) {
            case Widget::TouchEventType::BEGAN:
            {
//                Sound::instance()->Play(menu_select_effect);
                break;
            }
            case Widget::TouchEventType::CANCELED:
            {
                break;
            }
            case Widget::TouchEventType::ENDED:
            {
                on_click(static_cast<Button*>(sender));
                break;
            }
            default:
                break;
        }
    });
    
}

void AddOnMoveListener(Node* s, OnMoveF on_move, double on_touch_coef) {
    auto listener = EventListenerTouchOneByOne::create();
    // preventing other listeners from using it
    listener->setSwallowTouches(true);
    
    // trigger when you push down
    listener->onTouchBegan = [=](Touch* touch, Event* event){
        auto s = static_cast<Node*>(event->getCurrentTarget());
        
        //Get the position of the current point relative to the button
        Point locationInNode = s->convertToNodeSpace(touch->getLocation());
        Size size = s->getContentSize();
        
        // хак для лучшего попадания
        const float part = 0.5f;
        const float w_shift = size.width * part;
        const float h_shift = size.height * part;
        
        Rect rect = Rect(-w_shift, -h_shift, size.width + w_shift, size.height + h_shift);
        
        //Check the click area
        if (!rect.containsPoint(locationInNode))
            return false;
        
//        Sound::instance()->Play(menu_select_effect);
        
        float scale = s->getScale();
        scale *= on_touch_coef;
        s->setScale(scale);
        
        return true; // if you are consuming it
    };
    
    listener->onTouchMoved = [=](Touch* touch, Event* event) {
        on_move(touch, event);
    };
    
    listener->onTouchEnded = [=](Touch* touch, Event* event){
        auto s = static_cast<Node*>(event->getCurrentTarget());
        
        float scale = s->getScale();
        scale /= on_touch_coef;
        s->setScale(scale);
        
        //Get the position of the current point relative to the button
        Point locationInNode = s->convertToNodeSpace(touch->getLocation());
        Size size = s->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        
        //Check the click area
        if (!rect.containsPoint(locationInNode))
            return false;
        
        return true;
    };
    
    s->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, s);
}

float GetScreenRatio() {
    auto size = Director::getInstance()->getOpenGLView()->getFrameSize();
    return size.width / size.height;
}

void PositionInParent(Node* n, Node* parent, Vec2 p) {
    n->setPositionX(parent->getContentSize().width * p.x);
    n->setPositionY(parent->getContentSize().height * p.y);
}

void AdditionalScale(Node* s, float scale) {
    s->setScale(s->getScale() * scale);
}

    
} // visu_utils
